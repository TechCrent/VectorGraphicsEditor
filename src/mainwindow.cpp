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
        // Fallback: create canvas manually if not found in UI
        m_canvas = new Canvas(this);
        setCentralWidget(m_canvas);
    }
    
    setupUI();
    setupToolbars();
    setupDockWidgets();
    setupMenus();
    setupStatusBar();
    connectSignals();
    
    // Set up the document
    Layer *defaultLayer = new Layer("Layer 1");
    m_document->addLayer(defaultLayer);
    m_canvas->setDocument(m_document);
    
    setWindowTitle("Vector Graphics Editor");
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
    
    // Set up central widget
    if (!m_canvas) {
        m_canvas = new Canvas(this);
        setCentralWidget(m_canvas);
    }
}

void MainWindow::setupToolbars()
{
    // File toolbar
    m_fileToolBar = addToolBar("File");
    m_fileToolBar->setObjectName("fileToolBar");
    
    m_newAction = new QAction("New", this);
    m_newAction->setShortcut(QKeySequence::New);
    m_fileToolBar->addAction(m_newAction);
    
    m_openAction = new QAction("Open", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_fileToolBar->addAction(m_openAction);
    
    m_saveAction = new QAction("Save", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_fileToolBar->addAction(m_saveAction);
    
    m_fileToolBar->addSeparator();
    
    m_exportAction = new QAction("Export SVG", this);
    m_fileToolBar->addAction(m_exportAction);
    
    m_importAction = new QAction("Import SVG", this);
    m_fileToolBar->addAction(m_importAction);
    
    // Edit toolbar
    m_editToolBar = addToolBar("Edit");
    m_editToolBar->setObjectName("editToolBar");
    
    m_undoAction = new QAction("Undo", this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_editToolBar->addAction(m_undoAction);
    
    m_redoAction = new QAction("Redo", this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_editToolBar->addAction(m_redoAction);
    
    m_editToolBar->addSeparator();
    
    m_cutAction = new QAction("Cut", this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_editToolBar->addAction(m_cutAction);
    
    m_copyAction = new QAction("Copy", this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_editToolBar->addAction(m_copyAction);
    
    m_pasteAction = new QAction("Paste", this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_editToolBar->addAction(m_pasteAction);
    
    // Tools toolbar
    m_toolsToolBar = addToolBar("Tools");
    m_toolsToolBar->setObjectName("toolsToolBar");
    
    m_selectAction = new QAction("Select", this);
    m_selectAction->setCheckable(true);
    m_selectAction->setChecked(true);
    m_toolsToolBar->addAction(m_selectAction);
    
    m_rectangleAction = new QAction("Rectangle", this);
    m_rectangleAction->setCheckable(true);
    m_toolsToolBar->addAction(m_rectangleAction);
    
    m_ellipseAction = new QAction("Ellipse", this);
    m_ellipseAction->setCheckable(true);
    m_toolsToolBar->addAction(m_ellipseAction);
    
    m_lineAction = new QAction("Line", this);
    m_lineAction->setCheckable(true);
    m_toolsToolBar->addAction(m_lineAction);
    
    m_bezierAction = new QAction("Bezier", this);
    m_bezierAction->setCheckable(true);
    m_toolsToolBar->addAction(m_bezierAction);
    
    // View toolbar
    m_viewToolBar = addToolBar("View");
    m_viewToolBar->setObjectName("viewToolBar");
    
    m_zoomInAction = new QAction("Zoom In", this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    m_viewToolBar->addAction(m_zoomInAction);
    
    m_zoomOutAction = new QAction("Zoom Out", this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    m_viewToolBar->addAction(m_zoomOutAction);
    
    m_fitToViewAction = new QAction("Fit to View", this);
    m_viewToolBar->addAction(m_fitToViewAction);
    
    m_viewToolBar->addSeparator();
    
    m_showGridAction = new QAction("Show Grid", this);
    m_showGridAction->setCheckable(true);
    m_viewToolBar->addAction(m_showGridAction);
    
    m_snapToGridAction = new QAction("Snap to Grid", this);
    m_snapToGridAction->setCheckable(true);
    m_viewToolBar->addAction(m_snapToGridAction);
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
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exportAction);
    fileMenu->addAction(m_importAction);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", QKeySequence::Quit, this, &QWidget::close);
    
    // Edit menu
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction("&Undo", QKeySequence::Undo, this, nullptr);
    editMenu->addAction("&Redo", QKeySequence::Redo, this, nullptr);
    editMenu->addSeparator();
    editMenu->addAction("Cu&t", QKeySequence::Cut, this, nullptr);
    editMenu->addAction("&Copy", QKeySequence::Copy, this, nullptr);
    editMenu->addAction("&Paste", QKeySequence::Paste, this, nullptr);
    editMenu->addAction("&Delete", QKeySequence::Delete, this, nullptr);
    editMenu->addSeparator();
    editMenu->addAction("Select &All", QKeySequence::SelectAll, this, nullptr);
    
    // View menu
    QMenu *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction(m_zoomInAction);
    viewMenu->addAction(m_zoomOutAction);
    viewMenu->addAction(m_fitToViewAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_showGridAction);
    viewMenu->addAction(m_snapToGridAction);
    
    // Tools menu
    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction(m_selectAction);
    toolsMenu->addAction(m_rectangleAction);
    toolsMenu->addAction(m_ellipseAction);
    toolsMenu->addAction(m_lineAction);
    toolsMenu->addAction(m_bezierAction);
    
    // Layer menu
    QMenu *layerMenu = menuBar()->addMenu("&Layer");
    layerMenu->addAction("Add Layer", this, &MainWindow::addLayer);
    layerMenu->addAction("Remove Layer", this, &MainWindow::removeLayer);
    
    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About", this, &MainWindow::showAbout);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals()
{
    // File actions
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newDocument);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openDocument);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveDocument);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportSVG);
    connect(m_importAction, &QAction::triggered, this, &MainWindow::importSVG);
    
    // Edit actions
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::cut);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::copy);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::paste);
    
    // Tool actions
    connect(m_selectAction, &QAction::triggered, this, &MainWindow::selectTool);
    connect(m_rectangleAction, &QAction::triggered, this, &MainWindow::rectangleTool);
    connect(m_ellipseAction, &QAction::triggered, this, &MainWindow::ellipseTool);
    connect(m_lineAction, &QAction::triggered, this, &MainWindow::lineTool);
    connect(m_bezierAction, &QAction::triggered, this, &MainWindow::bezierTool);
    
    // View actions
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(m_fitToViewAction, &QAction::triggered, this, &MainWindow::fitToView);
    connect(m_showGridAction, &QAction::triggered, this, &MainWindow::showGrid);
    connect(m_snapToGridAction, &QAction::triggered, this, &MainWindow::snapToGrid);
    
    // Color buttons
    connect(m_fillColorButton, &QPushButton::clicked, this, &MainWindow::chooseFillColor);
    connect(m_strokeColorButton, &QPushButton::clicked, this, &MainWindow::chooseStrokeColor);
    connect(m_strokeWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::strokeWidthChanged);
    
    // Canvas signals
    connect(m_canvas, &Canvas::shapeSelected, this, &MainWindow::shapeSelected);
    connect(m_canvas, &Canvas::shapeCreated, this, &MainWindow::shapeCreated);
    connect(m_canvas, &Canvas::canvasChanged, this, &MainWindow::canvasChanged);
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
    m_canvas->setTool(Canvas::Select);
    updateToolActions();
    statusBar()->showMessage("Select tool", 1000);
}

void MainWindow::rectangleTool()
{
    m_canvas->setTool(Canvas::Rectangle);
    updateToolActions();
    statusBar()->showMessage("Rectangle tool", 1000);
}

void MainWindow::ellipseTool()
{
    m_canvas->setTool(Canvas::Ellipse);
    updateToolActions();
    statusBar()->showMessage("Ellipse tool", 1000);
}

void MainWindow::lineTool()
{
    m_canvas->setTool(Canvas::Line);
    updateToolActions();
    statusBar()->showMessage("Line tool", 1000);
}

void MainWindow::bezierTool()
{
    m_canvas->setTool(Canvas::Bezier);
    updateToolActions();
    statusBar()->showMessage("Bezier tool", 1000);
}

void MainWindow::updateToolActions()
{
    m_selectAction->setChecked(m_canvas->getTool() == Canvas::Select);
    m_rectangleAction->setChecked(m_canvas->getTool() == Canvas::Rectangle);
    m_ellipseAction->setChecked(m_canvas->getTool() == Canvas::Ellipse);
    m_lineAction->setChecked(m_canvas->getTool() == Canvas::Line);
    m_bezierAction->setChecked(m_canvas->getTool() == Canvas::Bezier);
}

// View operations
void MainWindow::zoomIn()
{
    m_canvas->zoomIn();
    statusBar()->showMessage(QString("Zoom: %1%").arg(m_canvas->getZoom() * 100), 1000);
}

void MainWindow::zoomOut()
{
    m_canvas->zoomOut();
    statusBar()->showMessage(QString("Zoom: %1%").arg(m_canvas->getZoom() * 100), 1000);
}

void MainWindow::fitToView()
{
    m_canvas->fitToView();
    statusBar()->showMessage("Fit to view", 1000);
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
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Fill Color");
    if (color.isValid()) {
        m_fillColorButton->setStyleSheet(QString("background-color: %1; border: 1px solid black;").arg(color.name()));
        // TODO: Apply fill color to selected shape
        statusBar()->showMessage("Fill color changed", 1000);
    }
}

void MainWindow::chooseStrokeColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Stroke Color");
    if (color.isValid()) {
        m_strokeColorButton->setStyleSheet(QString("background-color: %1; border: 1px solid black;").arg(color.name()));
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
                      "Vector Graphics Editor\n\n"
                      "A simple vector graphics editor built with Qt and Cairo.\n\n"
                      "Version 1.0");
} 