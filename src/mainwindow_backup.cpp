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

    if (m_canvas) {
        m_canvas->createCairoSurface();
    } else {
        qDebug() << "Failed to initialize canvas";
    }
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals()
{
    connect(m_fillColorButton, &QPushButton::clicked, this, &MainWindow::chooseFillColor);
    connect(m_strokeColorButton, &QPushButton::clicked, this, &MainWindow::chooseStrokeColor);
    connect(m_strokeWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::strokeWidthChanged);

    connect(ui->addLayerBtn, &QPushButton::clicked, this, &MainWindow::addLayer);
    connect(ui->removeLayerBtn, &QPushButton::clicked, this, &MainWindow::removeLayer);

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
    updateLayersList();

    if (m_canvas) {
        m_canvas->update();
    } else {
        qDebug() << "Canvas is null, cannot update after new document";
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
        m_canvas->setTool(Canvas::Select);
        updateToolActions();
        statusBar()->showMessage("Select tool", 1000);
    }
}

void MainWindow::rectangleTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Rectangle);
        updateToolActions();
        statusBar()->showMessage("Rectangle tool", 1000);
    }
}

void MainWindow::ellipseTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Ellipse);
        updateToolActions();
        statusBar()->showMessage("Ellipse tool", 1000);
    }
}

void MainWindow::lineTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Line);
        updateToolActions();
        statusBar()->showMessage("Line tool", 1000);
    }
}

void MainWindow::bezierTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Bezier);
        updateToolActions();
        statusBar()->showMessage("Bezier tool", 1000);
    }
}

void MainWindow::updateToolActions()
{
    if (!m_canvas) return;
    ui->actionSelect->setChecked(m_canvas->getTool() == Canvas::Select);
    ui->actionRectangle->setChecked(m_canvas->getTool() == Canvas::Rectangle);
    ui->actionEllipse->setChecked(m_canvas->getTool() == Canvas::Ellipse);
    ui->actionLine->setChecked(m_canvas->getTool() == Canvas::Line);
    ui->actionBezier->setChecked(m_canvas->getTool() == Canvas::Bezier);
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
