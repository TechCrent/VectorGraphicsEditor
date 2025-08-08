#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QKeySequence>
#include <QIcon>
#include <QStyle>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_canvas(nullptr)
    , m_document(new Document())
{
    ui->setupUi(this);

    // Set up the canvas from the UI file or create one if missing
    m_canvas = ui->canvasWidget;
    if (!m_canvas) {
        qDebug() << "Canvas widget not found in UI, creating manually";
        m_canvas = new Canvas(this);
        setCentralWidget(m_canvas);
    }

    // Get UI components
    m_layersList = ui->layersList;
    m_fillColorButton = ui->fillColorBtn;
    m_strokeColorButton = ui->strokeColorBtn;
    m_strokeWidthSpinBox = ui->strokeWidthSpinBox;
    m_layersDock = ui->layersDock;
    m_propertiesDock = ui->propertiesDock;

    setupUI();
    setupActions();
    setupMenusAndToolbars();
    setupStatusBar();
    connectSignals();

    // Set up the document
    Layer *defaultLayer = new Layer("Layer 1");
    m_document->addLayer(defaultLayer);
    m_document->setActiveLayer(defaultLayer);

    if (m_canvas) {
        m_canvas->setDocument(m_document);
    } else {
        qDebug() << "Warning: Canvas is null, cannot attach document";
    }

    updateLayersList();

    // Initialize color buttons
    updateFillColorButton(QColor(255, 255, 255, 0)); // Transparent
    updateStrokeColorButton(QColor(0, 0, 0));        // Black

    setWindowTitle("Vector Graphics Editor - Professional");
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_document;
}

void MainWindow::setupUI()
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(43, 43, 43));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(74, 74, 74));
    darkPalette.setColor(QPalette::AlternateBase, QColor(90, 90, 90));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(74, 74, 74));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(74, 144, 226));
    darkPalette.setColor(QPalette::Highlight, QColor(74, 144, 226));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);

#ifdef ENABLE_CAIRO
    if (m_canvas) {
        m_canvas->createCairoSurface();
    } else {
        qDebug() << "Failed to initialize canvas";
    }
#endif
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::setupActions()
{
    // File
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openDocument);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveDocument);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveDocument);
    connect(ui->actionImport_SVG, &QAction::triggered, this, &MainWindow::importSVG);
    connect(ui->actionExport_SVG, &QAction::triggered, this, &MainWindow::exportSVG);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // Edit
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionDelete, &QAction::triggered, this, [this]() {
        if (m_canvas) {
            QKeyEvent delEvent(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
            QApplication::sendEvent(m_canvas, &delEvent);
        }
    });

    // View
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionFit_to_View, &QAction::triggered, this, &MainWindow::fitToView);
    connect(ui->actionShow_Grid, &QAction::triggered, this, &MainWindow::showGrid);
    connect(ui->actionSnap_to_Grid, &QAction::triggered, this, &MainWindow::snapToGrid);

    // Tools
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::selectTool);
    connect(ui->actionRectangle, &QAction::triggered, this, &MainWindow::rectangleTool);
    connect(ui->actionEllipse, &QAction::triggered, this, &MainWindow::ellipseTool);
    connect(ui->actionLine, &QAction::triggered, this, &MainWindow::lineTool);
    connect(ui->actionBezier, &QAction::triggered, this, &MainWindow::bezierTool);
    connect(ui->actionPen, &QAction::triggered, this, &MainWindow::penTool);
    connect(ui->actionText, &QAction::triggered, this, &MainWindow::textTool);

    // Help
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

    // Layers
    connect(ui->actionNew_Layer, &QAction::triggered, this, &MainWindow::addLayer);
    connect(ui->actionDelete_Layer, &QAction::triggered, this, &MainWindow::removeLayer);
}

void MainWindow::setupMenusAndToolbars()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(ui->actionNew);
    fileMenu->addAction(ui->actionOpen);
    fileMenu->addAction(ui->actionSave);
    fileMenu->addAction(ui->actionSave_As);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->actionImport_SVG);
    fileMenu->addAction(ui->actionExport_SVG);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->actionExit);

    QMenu *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(ui->actionUndo);
    editMenu->addAction(ui->actionRedo);
    editMenu->addSeparator();
    editMenu->addAction(ui->actionCut);
    editMenu->addAction(ui->actionCopy);
    editMenu->addAction(ui->actionPaste);
    editMenu->addAction(ui->actionDelete);
    editMenu->addSeparator();
    editMenu->addAction(ui->actionSelect_All);

    QMenu *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction(ui->actionZoom_In);
    viewMenu->addAction(ui->actionZoom_Out);
    viewMenu->addAction(ui->actionFit_to_View);
    viewMenu->addAction(ui->actionActual_Size);
    viewMenu->addSeparator();
    viewMenu->addAction(ui->actionShow_Grid);
    viewMenu->addAction(ui->actionSnap_to_Grid);
    viewMenu->addSeparator();
    viewMenu->addAction(ui->actionShow_Rulers);
    viewMenu->addAction(ui->actionShow_Guides);

    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction(ui->actionSelect);
    toolsMenu->addSeparator();
    toolsMenu->addAction(ui->actionRectangle);
    toolsMenu->addAction(ui->actionEllipse);
    toolsMenu->addAction(ui->actionLine);
    toolsMenu->addAction(ui->actionBezier);
    toolsMenu->addSeparator();
    toolsMenu->addAction(ui->actionText);
    toolsMenu->addAction(ui->actionPen);

    QMenu *layerMenu = menuBar()->addMenu("&Layer");
    layerMenu->addAction(ui->actionNew_Layer);
    layerMenu->addAction(ui->actionDuplicate_Layer);
    layerMenu->addAction(ui->actionDelete_Layer);
    layerMenu->addSeparator();
    layerMenu->addAction(ui->actionMove_Layer_Up);
    layerMenu->addAction(ui->actionMove_Layer_Down);

    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(ui->actionAbout);
    helpMenu->addAction(ui->actionAbout_Qt);
}

void MainWindow::connectSignals()
{
    // Colors
    connect(m_fillColorButton, &QPushButton::clicked, this, &MainWindow::chooseFillColor);
    connect(m_strokeColorButton, &QPushButton::clicked, this, &MainWindow::chooseStrokeColor);
    connect(m_strokeWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::strokeWidthChanged);

    // Layers
    connect(ui->addLayerBtn, &QPushButton::clicked, this, &MainWindow::addLayer);
    connect(ui->removeLayerBtn, &QPushButton::clicked, this, &MainWindow::removeLayer);

    // Tools
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::selectTool);
    connect(ui->actionRectangle, &QAction::triggered, this, &MainWindow::rectangleTool);
    connect(ui->actionEllipse, &QAction::triggered, this, &MainWindow::ellipseTool);
    connect(ui->actionLine, &QAction::triggered, this, &MainWindow::lineTool);
    connect(ui->actionBezier, &QAction::triggered, this, &MainWindow::bezierTool);
    connect(ui->actionPen, &QAction::triggered, this, &MainWindow::penTool);
    connect(ui->actionText, &QAction::triggered, this, &MainWindow::textTool);

    // View
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionFit_to_View, &QAction::triggered, this, &MainWindow::fitToView);
    connect(ui->actionShow_Grid, &QAction::triggered, this, &MainWindow::showGrid);
    connect(ui->actionSnap_to_Grid, &QAction::triggered, this, &MainWindow::snapToGrid);

    // Canvas
    if (m_canvas) {
        connect(m_canvas, &Canvas::shapeSelected, this, &MainWindow::shapeSelected);
        connect(m_canvas, &Canvas::shapeCreated, this, &MainWindow::shapeCreated);
        connect(m_canvas, &Canvas::canvasChanged, this, &MainWindow::canvasChanged);
    } else {
        qDebug() << "Canvas is null, cannot connect signals";
    }
}

void MainWindow::newDocument()
{
    m_document->clear();
    Layer *defaultLayer = new Layer("Layer 1");
    m_document->addLayer(defaultLayer);
    m_document->setActiveLayer(defaultLayer);
    updateLayersList();

    if (m_canvas) {
        m_canvas->update();
    }
    statusBar()->showMessage("New document created", 2000);
}

void MainWindow::openDocument()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Document", "", "SVG Files (*.svg);;All Files (*)");
    if (!filename.isEmpty() && m_canvas) {
        m_canvas->loadSVG(filename);
        statusBar()->showMessage("Opened: " + filename, 2000);
    }
}

void MainWindow::saveDocument()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Document", "", "SVG Files (*.svg);;All Files (*)");
    if (!filename.isEmpty() && m_canvas) {
        m_canvas->saveSVG(filename);
        statusBar()->showMessage("Saved: " + filename, 2000);
    }
}

void MainWindow::exportSVG()
{
    QString filename = QFileDialog::getSaveFileName(this, "Export SVG", "", "SVG Files (*.svg)");
    if (!filename.isEmpty() && m_canvas) {
        m_canvas->saveSVG(filename);
        statusBar()->showMessage("Exported: " + filename, 2000);
    }
}

void MainWindow::importSVG()
{
    QString filename = QFileDialog::getOpenFileName(this, "Import SVG", "", "SVG Files (*.svg)");
    if (!filename.isEmpty() && m_canvas) {
        m_canvas->importSVG(filename);
        statusBar()->showMessage("Imported: " + filename, 2000);
    }
}

void MainWindow::undo()
{
    if (m_document->canUndo()) {
        m_document->undo();
        if (m_canvas) m_canvas->update();
        statusBar()->showMessage("Undo", 1000);
    }
}

void MainWindow::redo()
{
    if (m_document->canRedo()) {
        m_document->redo();
        if (m_canvas) m_canvas->update();
        statusBar()->showMessage("Redo", 1000);
    }
}

void MainWindow::cut()
{
    if (m_canvas) {
        m_canvas->cutSelection();
        statusBar()->showMessage("Cut", 1000);
    }
}

void MainWindow::copy()
{
    if (m_canvas) {
        m_canvas->copySelection();
        statusBar()->showMessage("Copy", 1000);
    }
}

void MainWindow::paste()
{
    if (m_canvas) {
        m_canvas->pasteSelection();
        statusBar()->showMessage("Paste", 1000);
    }
}

void MainWindow::selectTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Select);
        updateToolActions();
        statusBar()->showMessage("Select tool", 1000);
    }
}

void MainWindow::rectangleTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Rectangle);
        updateToolActions();
        statusBar()->showMessage("Rectangle tool", 1000);
    }
}

void MainWindow::ellipseTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Ellipse);
        updateToolActions();
        statusBar()->showMessage("Ellipse tool", 1000);
    }
}

void MainWindow::lineTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Line);
        updateToolActions();
        statusBar()->showMessage("Line tool", 1000);
    }
}

void MainWindow::bezierTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Bezier);
        updateToolActions();
        statusBar()->showMessage("Bezier tool", 1000);
    }
}

void MainWindow::penTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Pen);
        updateToolActions();
        statusBar()->showMessage("Pen tool", 1000);
    }
}

void MainWindow::textTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Tool_Text);
        updateToolActions();
        statusBar()->showMessage("Text tool", 1000);
    }
}

void MainWindow::updateToolActions()
{
    if (!m_canvas) return;
    ui->actionSelect->setChecked(m_canvas->getTool() == Canvas::Tool_Select);
    ui->actionRectangle->setChecked(m_canvas->getTool() == Canvas::Tool_Rectangle);
    ui->actionEllipse->setChecked(m_canvas->getTool() == Canvas::Tool_Ellipse);
    ui->actionLine->setChecked(m_canvas->getTool() == Canvas::Tool_Line);
    ui->actionBezier->setChecked(m_canvas->getTool() == Canvas::Tool_Bezier);
    ui->actionPen->setChecked(m_canvas->getTool() == Canvas::Tool_Pen);
    ui->actionText->setChecked(m_canvas->getTool() == Canvas::Tool_Text);
}

void MainWindow::zoomIn()
{
    if (m_canvas) {
        m_canvas->zoomIn();
        statusBar()->showMessage(QString("Zoom: %1%").arg(m_canvas->getZoom() * 100), 1000);
    }
}

void MainWindow::zoomOut()
{
    if (m_canvas) {
        m_canvas->zoomOut();
        statusBar()->showMessage(QString("Zoom: %1%").arg(m_canvas->getZoom() * 100), 1000);
    }
}

void MainWindow::fitToView()
{
    if (m_canvas) {
        m_canvas->fitToView();
        statusBar()->showMessage("Fit to view", 1000);
    }
}

void MainWindow::showGrid()
{
    if (m_canvas) {
        m_canvas->toggleGrid();
        statusBar()->showMessage("Grid toggled", 1000);
    }
}

void MainWindow::snapToGrid()
{
    if (m_canvas) {
        m_canvas->toggleSnapToGrid();
        statusBar()->showMessage("Snap to grid toggled", 1000);
    }
}

void MainWindow::addLayer()
{
    QString layerName = QString("Layer %1").arg(m_document->getLayers().size() + 1);
    Layer *newLayer = new Layer(layerName);
    m_document->addLayer(newLayer);
    updateLayersList();
    statusBar()->showMessage("Added layer: " + layerName, 2000);
}

void MainWindow::removeLayer()
{
    QListWidgetItem *selectedItem = m_layersList->currentItem();
    if (!selectedItem) return;

    Layer *layer = selectedItem->data(Qt::UserRole).value<Layer*>();
    if (layer) {
        m_document->removeLayer(layer);
        updateLayersList();
        statusBar()->showMessage("Layer removed", 2000);
    }
}

void MainWindow::updateLayersList()
{
    m_layersList->clear();
    for (Layer *layer : m_document->getLayers()) {
        QListWidgetItem *item = new QListWidgetItem(layer->getName());
        item->setData(Qt::UserRole, QVariant::fromValue(layer));
        m_layersList->addItem(item);
    }
}

void MainWindow::chooseFillColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Fill Color", QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        updateFillColorButton(color);
        if (m_canvas) m_canvas->setFillColor(color);
        statusBar()->showMessage("Fill color changed", 1000);
    }
}

void MainWindow::chooseStrokeColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Stroke Color");
    if (color.isValid()) {
        updateStrokeColorButton(color);
        if (m_canvas) m_canvas->setStrokeColor(color);
        statusBar()->showMessage("Stroke color changed", 1000);
    }
}

void MainWindow::strokeWidthChanged(int width)
{
    if (m_canvas) m_canvas->setStrokeWidth(width);
    statusBar()->showMessage(QString("Stroke width: %1").arg(width), 1000);
}

void MainWindow::shapeSelected(Shape *shape)
{
    Q_UNUSED(shape);
    statusBar()->showMessage("Shape selected", 1000);
}

void MainWindow::shapeCreated(Shape *shape)
{
    Q_UNUSED(shape);
    statusBar()->showMessage("Shape created", 1000);
}

void MainWindow::canvasChanged()
{
    statusBar()->showMessage("Canvas changed", 1000);
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Vector Graphics Editor",
                      "Vector Graphics Editor - Professional\n\n"
                      "A modern vector graphics editor built with Qt6 and Cairo.\n\n"
                      "Features:\n"
                      "• Modern dark theme interface\n"
                      "• Professional tool layout\n"
                      "• Layer management\n"
                      "• Vector shape tools\n"
                      "• SVG import/export\n\n"
                      "Version 2.0");
}

void MainWindow::updateFillColorButton(const QColor &color)
{
    if (!m_fillColorButton) return;

    QPixmap pixmap(24, 24);
    pixmap.fill(color);
    m_fillColorButton->setIcon(QIcon(pixmap));
}

void MainWindow::updateStrokeColorButton(const QColor &color)
{
    if (!m_strokeColorButton) return;

    QPixmap pixmap(24, 24);
    pixmap.fill(color);
    m_strokeColorButton->setIcon(QIcon(pixmap));
}