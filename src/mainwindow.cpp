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

    // Set up the canvas from the UI file
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
    m_canvas->setDocument(m_document);
    updateLayersList();

    // Initialize color buttons
    updateFillColorButton(QColor(255, 255, 255, 0)); // Transparent
    updateStrokeColorButton(QColor(0, 0, 0)); // Black

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
    // Set application style
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Apply additional dark theme to the application
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

    // Ensure canvas is properly set up
    if (m_canvas) {
        m_canvas->createCairoSurface();
    } else {
        qDebug() << "Failed to initialize canvas";
    }
}

void MainWindow::setupToolbars()
{
    // File toolbar
    m_fileToolBar = addToolBar("File");
    m_fileToolBar->setObjectName("fileToolBar");
    m_fileToolBar->addAction(ui->actionNew);
    m_fileToolBar->addAction(ui->actionOpen);
    m_fileToolBar->addAction(ui->actionSave);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(ui->actionExport_SVG);
    m_fileToolBar->addAction(ui->actionImport_SVG);

    // Edit toolbar
    m_editToolBar = addToolBar("Edit");
    m_editToolBar->setObjectName("editToolBar");
    m_editToolBar->addAction(ui->actionUndo);
    m_editToolBar->addAction(ui->actionRedo);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(ui->actionCut);
    m_editToolBar->addAction(ui->actionCopy);
    m_editToolBar->addAction(ui->actionPaste);

    // Tools toolbar
    m_toolsToolBar = addToolBar("Tools");
    m_toolsToolBar->setObjectName("toolsToolBar");
    m_toolsToolBar->addAction(ui->actionSelect);
    m_toolsToolBar->addAction(ui->actionRectangle);
    m_toolsToolBar->addAction(ui->actionEllipse);
    m_toolsToolBar->addAction(ui->actionLine);
    m_toolsToolBar->addAction(ui->actionBezier);

    // View toolbar
    m_viewToolBar = addToolBar("View");
    m_viewToolBar->setObjectName("viewToolBar");
    m_viewToolBar->addAction(ui->actionZoom_In);
    m_viewToolBar->addAction(ui->actionZoom_Out);
    m_viewToolBar->addAction(ui->actionFit_to_View);
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(ui->actionShow_Grid);
    m_viewToolBar->addAction(ui->actionSnap_to_Grid);
}

void MainWindow::setupDockWidgets()
{
    // Layers dock widget
    m_layersDock = new QDockWidget("Layers", this);
    m_layersDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_layersDock);

    m_layersList = new QListWidget();
    m_layersDock->setWidget(m_layersList);

    // Properties dock widget
    m_propertiesDock = new QDockWidget("Properties", this);
    m_propertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

    m_propertiesWidget = new QWidget();
    m_propertiesDock->setWidget(m_propertiesWidget);

    // Set up properties widget
    QVBoxLayout *propertiesLayout = new QVBoxLayout(m_propertiesWidget);

    // Shape properties group
    QGroupBox *shapeGroup = new QGroupBox("Shape Properties");
    QVBoxLayout *shapeLayout = new QVBoxLayout(shapeGroup);

    // Fill color
    QHBoxLayout *fillLayout = new QHBoxLayout();
    fillLayout->addWidget(new QLabel("Fill:"));
    m_fillColorButton = new QPushButton();
    m_fillColorButton->setFixedSize(24, 24);
    fillLayout->addWidget(m_fillColorButton);
    shapeLayout->addLayout(fillLayout);

    // Stroke color
    QHBoxLayout *strokeLayout = new QHBoxLayout();
    strokeLayout->addWidget(new QLabel("Stroke:"));
    m_strokeColorButton = new QPushButton();
    m_strokeColorButton->setFixedSize(24, 24);
    strokeLayout->addWidget(m_strokeColorButton);
    shapeLayout->addLayout(strokeLayout);

    // Stroke width
    QHBoxLayout *widthLayout = new QHBoxLayout();
    widthLayout->addWidget(new QLabel("Width:"));
    m_strokeWidthSpinBox = new QSpinBox();
    m_strokeWidthSpinBox->setRange(1, 50);
    m_strokeWidthSpinBox->setValue(2);
    widthLayout->addWidget(m_strokeWidthSpinBox);
    shapeLayout->addLayout(widthLayout);

    propertiesLayout->addWidget(shapeGroup);
    propertiesLayout->addStretch();
}

void MainWindow::setupMenus()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(ui->actionNew);
    fileMenu->addAction(ui->actionOpen);
    fileMenu->addAction(ui->actionSave);
    fileMenu->addAction(ui->actionSave_As);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->actionExport_SVG);
    fileMenu->addAction(ui->actionImport_SVG);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->actionExit);

    // Edit menu
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

    // View menu
    QMenu *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction(ui->actionZoom_In);
    viewMenu->addAction(ui->actionZoom_Out);
    viewMenu->addAction(ui->actionFit_to_View);
    viewMenu->addSeparator();
    viewMenu->addAction(ui->actionShow_Grid);
    viewMenu->addAction(ui->actionSnap_to_Grid);

    // Tools menu
    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction(ui->actionSelect);
    toolsMenu->addAction(ui->actionRectangle);
    toolsMenu->addAction(ui->actionEllipse);
    toolsMenu->addAction(ui->actionLine);
    toolsMenu->addAction(ui->actionBezier);

    // Layer menu
    QMenu *layerMenu = menuBar()->addMenu("&Layer");
    layerMenu->addAction(ui->actionNew_Layer);
    layerMenu->addAction(ui->actionDelete_Layer);

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(ui->actionAbout);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals()
{
    // Color buttons from UI
    connect(m_fillColorButton, &QPushButton::clicked, this, &MainWindow::chooseFillColor);
    connect(m_strokeColorButton, &QPushButton::clicked, this, &MainWindow::chooseStrokeColor);
    connect(m_strokeWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::strokeWidthChanged);

    // Layer buttons from UI
    connect(ui->addLayerBtn, &QPushButton::clicked, this, &MainWindow::addLayer);
    connect(ui->removeLayerBtn, &QPushButton::clicked, this, &MainWindow::removeLayer);

    // Canvas signals
    if (m_canvas) {
        connect(m_canvas, &Canvas::shapeSelected, this, &MainWindow::shapeSelected);
        connect(m_canvas, &Canvas::shapeCreated, this, &MainWindow::shapeCreated);
        connect(m_canvas, &Canvas::canvasChanged, this, &MainWindow::canvasChanged);
    } else {
        qDebug() << "Canvas is null, cannot connect signals";
    }
}

// File operations
void MainWindow::newDocument()
{
    // TODO: Implement new document
    statusBar()->showMessage("New document created", 2000);
}

void MainWindow::openDocument()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Document", "", "SVG Files (*.svg);;All Files (*)");
    if (!filename.isEmpty()) {
        // TODO: Implement open document
        statusBar()->showMessage("Opened: " + filename, 2000);
    }
}

void MainWindow::saveDocument()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Document", "", "SVG Files (*.svg);;All Files (*)");
    if (!filename.isEmpty()) {
        // TODO: Implement save document
        statusBar()->showMessage("Saved: " + filename, 2000);
    }
}

void MainWindow::exportSVG()
{
    QString filename = QFileDialog::getSaveFileName(this, "Export SVG", "", "SVG Files (*.svg)");
    if (!filename.isEmpty()) {
        // TODO: Implement export SVG
        statusBar()->showMessage("Exported: " + filename, 2000);
    }
}

void MainWindow::importSVG()
{
    QString filename = QFileDialog::getOpenFileName(this, "Import SVG", "", "SVG Files (*.svg)");
    if (!filename.isEmpty()) {
        // TODO: Implement import SVG
        statusBar()->showMessage("Imported: " + filename, 2000);
    }
}

// Edit operations
void MainWindow::undo()
{
    if (m_document->canUndo()) {
        m_document->undo();
        statusBar()->showMessage("Undo", 1000);
    }
}

void MainWindow::redo()
{
    if (m_document->canRedo()) {
        m_document->redo();
        statusBar()->showMessage("Redo", 1000);
    }
}

void MainWindow::cut()
{
    // TODO: Implement cut
    statusBar()->showMessage("Cut", 1000);
}

void MainWindow::copy()
{
    // TODO: Implement copy
    statusBar()->showMessage("Copy", 1000);
}

void MainWindow::paste()
{
    // TODO: Implement paste
    statusBar()->showMessage("Paste", 1000);
}

// Tool operations
void MainWindow::selectTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Select);
        updateToolActions();
        statusBar()->showMessage("Select tool", 1000);
    } else {
        qDebug() << "Cannot select tool: canvas is null";
    }
}

void MainWindow::rectangleTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Rectangle);
        updateToolActions();
        statusBar()->showMessage("Rectangle tool", 1000);
    } else {
        qDebug() << "Cannot select rectangle tool: canvas is null";
    }
}

void MainWindow::ellipseTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Ellipse);
        updateToolActions();
        statusBar()->showMessage("Ellipse tool", 1000);
    } else {
        qDebug() << "Cannot select ellipse tool: canvas is null";
    }
}

void MainWindow::lineTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Line);
        updateToolActions();
        statusBar()->showMessage("Line tool", 1000);
    } else {
        qDebug() << "Cannot select line tool: canvas is null";
    }
}

void MainWindow::bezierTool()
{
    if (m_canvas) {
        m_canvas->setTool(Canvas::Bezier);
        updateToolActions();
        statusBar()->showMessage("Bezier tool", 1000);
    } else {
        qDebug() << "Cannot select bezier tool: canvas is null";
    }
}

void MainWindow::updateToolActions()
{
    qDebug() << "Updating tool actions";
    if (!m_canvas) {
        qDebug() << "Canvas is null, cannot update tool actions";
        return;
    }
    if (ui->actionSelect) {
        ui->actionSelect->setChecked(m_canvas->getTool() == Canvas::Select);
        qDebug() << "Select action checked:" << ui->actionSelect->isChecked();
    } else {
        qDebug() << "actionSelect is null";
    }
    if (ui->actionRectangle) {
        ui->actionRectangle->setChecked(m_canvas->getTool() == Canvas::Rectangle);
        qDebug() << "Rectangle action checked:" << ui->actionRectangle->isChecked();
    } else {
        qDebug() << "actionRectangle is null";
    }
    if (ui->actionEllipse) {
        ui->actionEllipse->setChecked(m_canvas->getTool() == Canvas::Ellipse);
        qDebug() << "Ellipse action checked:" << ui->actionEllipse->isChecked();
    } else {
        qDebug() << "actionEllipse is null";
    }
    if (ui->actionLine) {
        ui->actionLine->setChecked(m_canvas->getTool() == Canvas::Line);
        qDebug() << "Line action checked:" << ui->actionLine->isChecked();
    } else {
        qDebug() << "actionLine is null";
    }
    if (ui->actionBezier) {
        ui->actionBezier->setChecked(m_canvas->getTool() == Canvas::Bezier);
        qDebug() << "Bezier action checked:" << ui->actionBezier->isChecked();
    } else {
        qDebug() << "actionBezier is null";
    }
}

// View operations
void MainWindow::zoomIn()
{
    if (m_canvas) {
        m_canvas->zoomIn();
        statusBar()->showMessage(QString("Zoom: %1%").arg(m_canvas->getZoom() * 100), 1000);
    } else {
        qDebug() << "Cannot zoom in: canvas is null";
    }
}

void MainWindow::zoomOut()
{
    if (m_canvas) {
        m_canvas->zoomOut();
        statusBar()->showMessage(QString("Zoom: %1%").arg(m_canvas->getZoom() * 100), 1000);
    } else {
        qDebug() << "Cannot zoom out: canvas is null";
    }
}

void MainWindow::fitToView()
{
    if (m_canvas) {
        m_canvas->fitToView();
        statusBar()->showMessage("Fit to view", 1000);
    } else {
        qDebug() << "Cannot fit to view: canvas is null";
    }
}

void MainWindow::showGrid()
{
    // TODO: Implement show grid
    statusBar()->showMessage("Grid toggled", 1000);
}

void MainWindow::snapToGrid()
{
    // TODO: Implement snap to grid
    statusBar()->showMessage("Snap to grid toggled", 1000);
}

// Layer operations
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
    // TODO: Implement remove layer
    statusBar()->showMessage("Remove layer", 2000);
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

// Property operations
void MainWindow::chooseFillColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Fill Color", QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        updateFillColorButton(color);
        // TODO: Apply fill color to selected shape
        statusBar()->showMessage("Fill color changed", 1000);
    }
}

void MainWindow::chooseStrokeColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Stroke Color");
    if (color.isValid()) {
        updateStrokeColorButton(color);
        // TODO: Apply stroke color to selected shape
        statusBar()->showMessage("Stroke color changed", 1000);
    }
}

void MainWindow::strokeWidthChanged(int width)
{
    // TODO: Apply stroke width to selected shape
    statusBar()->showMessage(QString("Stroke width: %1").arg(width), 1000);
}

// Event handlers
void MainWindow::shapeSelected(Shape *shape)
{
    // TODO: Update properties panel with shape properties
    statusBar()->showMessage("Shape selected", 1000);
}

void MainWindow::shapeCreated(Shape *shape)
{
    // TODO: Update layers list
    statusBar()->showMessage("Shape created", 1000);
}

void MainWindow::canvasChanged()
{
    // TODO: Update UI to reflect canvas changes
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

void MainWindow::setupActions()
{
    // Actions are already defined in the UI file
    // Connect them to the appropriate slots
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openDocument);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveDocument);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);

    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionFit_to_View, &QAction::triggered, this, &MainWindow::fitToView);

    connect(ui->actionShow_Grid, &QAction::triggered, this, &MainWindow::showGrid);
    connect(ui->actionSnap_to_Grid, &QAction::triggered, this, &MainWindow::snapToGrid);

    // Tool actions
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::selectTool);
    connect(ui->actionRectangle, &QAction::triggered, this, &MainWindow::rectangleTool);
    connect(ui->actionEllipse, &QAction::triggered, this, &MainWindow::ellipseTool);
    connect(ui->actionLine, &QAction::triggered, this, &MainWindow::lineTool);
    connect(ui->actionBezier, &QAction::triggered, this, &MainWindow::bezierTool);

    // Layer actions
    connect(ui->actionNew_Layer, &QAction::triggered, this, &MainWindow::addLayer);
    connect(ui->actionDelete_Layer, &QAction::triggered, this, &MainWindow::removeLayer);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

    // Set up action group for exclusive tools
    QActionGroup *toolGroup = new QActionGroup(this);
    toolGroup->addAction(ui->actionSelect);
    toolGroup->addAction(ui->actionRectangle);
    toolGroup->addAction(ui->actionEllipse);
    toolGroup->addAction(ui->actionLine);
    toolGroup->addAction(ui->actionBezier);
    toolGroup->setExclusive(true);
}

void MainWindow::setupMenusAndToolbars()
{
    // Menus and toolbars are already set up in the UI file
    // Just customize them if needed
    ui->mainToolBar->setIconSize(QSize(24, 24));
    ui->toolsToolBar->setIconSize(QSize(32, 32));
}

void MainWindow::updateFillColorButton(const QColor &color)
{
    if (m_fillColorButton) {
        QString styleSheet = QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: %2;"
            "    border: 2px solid #cccccc;"
            "    border-radius: 4px;"
            "    padding: 8px;"
            "    font-weight: normal;"
            "}"
            "QPushButton:hover {"
            "    border-color: #4a90e2;"
            "}"
        ).arg(color.name()).arg(color.lightness() > 128 ? "#000000" : "#ffffff");

        m_fillColorButton->setStyleSheet(styleSheet);
        m_fillColorButton->setText(color.alpha() == 0 ? "No Fill" : "Fill Color");
    }
}

void MainWindow::updateStrokeColorButton(const QColor &color)
{
    if (m_strokeColorButton) {
        QString styleSheet = QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: %2;"
            "    border: 2px solid #cccccc;"
            "    border-radius: 4px;"
            "    padding: 8px;"
            "    font-weight: normal;"
            "}"
            "QPushButton:hover {"
            "    border-color: #4a90e2;"
            "}"
        ).arg(color.name()).arg(color.lightness() > 128 ? "#000000" : "#ffffff");

        m_strokeColorButton->setStyleSheet(styleSheet);
    }
}