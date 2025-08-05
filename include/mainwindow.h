#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QSpinBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include "canvas.h"
#include "document.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void newDocument();
    void openDocument();
    void saveDocument();
    void exportSVG();
    void importSVG();
    
    // Edit operations
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    
    // Tool operations
    void selectTool();
    void rectangleTool();
    void ellipseTool();
    void lineTool();
    void bezierTool();
    void updateToolActions();
    
    // View operations
    void zoomIn();
    void zoomOut();
    void fitToView();
    void showGrid();
    void snapToGrid();
    
    // Layer operations
    void addLayer();
    void removeLayer();
    void updateLayersList();
    
    // Property operations
    void chooseFillColor();
    void chooseStrokeColor();
    void strokeWidthChanged(int width);
    
    // Canvas signals
    void shapeSelected(Shape* shape);
    void shapeCreated(Shape* shape);
    void canvasChanged();
    
    // Help
    void showAbout();

private:
    void setupUI();
    void setupToolbars();
    void setupDockWidgets();
    void setupMenus();
    void setupStatusBar();
    void connectSignals();

private:
    Ui::MainWindow *ui;
    Canvas *m_canvas;
    Document *m_document;
    
    // Toolbars
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_toolsToolBar;
    QToolBar *m_viewToolBar;
    
    // Dock widgets
    QDockWidget *m_layersDock;
    QDockWidget *m_propertiesDock;
    
    // Layer panel
    QListWidget *m_layersList;
    
    // Property panel
    QWidget *m_propertiesWidget;
    QPushButton *m_fillColorButton;
    QPushButton *m_strokeColorButton;
    QSpinBox *m_strokeWidthSpinBox;
    
    // Actions
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exportAction;
    QAction *m_importAction;
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    QAction *m_selectAction;
    QAction *m_rectangleAction;
    QAction *m_ellipseAction;
    QAction *m_lineAction;
    QAction *m_bezierAction;
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_fitToViewAction;
    QAction *m_showGridAction;
    QAction *m_snapToGridAction;
};

#endif // MAINWINDOW_H 