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
    void setupActions();
    void setupMenusAndToolbars();
    void setupStatusBar();
    void connectSignals();
    void updateFillColorButton(const QColor &color);
    void updateStrokeColorButton(const QColor &color);

private:
    Ui::MainWindow *ui;
    Canvas *m_canvas;
    Document *m_document;
    
    // Dock widgets (from UI file)
    QDockWidget *m_layersDock;
    QDockWidget *m_propertiesDock;
    
    // Layer panel (from UI file)
    QListWidget *m_layersList;
    
    // Property panel (from UI file)
    QPushButton *m_fillColorButton;
    QPushButton *m_strokeColorButton;
    QSpinBox *m_strokeWidthSpinBox;
    
    // Actions are now defined in the UI file
};

#endif // MAINWINDOW_H 