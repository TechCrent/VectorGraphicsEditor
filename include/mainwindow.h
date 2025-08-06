#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
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
    void newDocument();
    void openDocument();
    void saveDocument();
    void exportSVG();
    void importSVG();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectTool();
    void rectangleTool();
    void ellipseTool();
    void lineTool();
    void bezierTool();
    void updateToolActions();
    void zoomIn();
    void zoomOut();
    void fitToView();
    void showGrid();
    void snapToGrid();
    void addLayer();
    void removeLayer();
    void updateLayersList();
    void chooseFillColor();
    void chooseStrokeColor();
    void strokeWidthChanged(int width);
    void shapeSelected(Shape* shape);
    void shapeCreated(Shape* shape);
    void canvasChanged();
    void showAbout();
private:
    void setupUI();
    void setupActions();
    void setupMenusAndToolbars();
    void setupStatusBar();
    void connectSignals();
    void updateFillColorButton(const QColor &color);
    void updateStrokeColorButton(const QColor &color);
    void setupToolbars();
    void setupDockWidgets();
    void setupMenus();
private:
    Ui::MainWindow *ui;
    Canvas *m_canvas;
    Document *m_document;
    QDockWidget *m_layersDock;
    QDockWidget *m_propertiesDock;
    QListWidget *m_layersList;
    QPushButton *m_fillColorButton;
    QPushButton *m_strokeColorButton;
    QSpinBox *m_strokeWidthSpinBox;
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_toolsToolBar;
    QToolBar *m_viewToolBar;
    QWidget *m_propertiesWidget;
};

#endif // MAINWINDOW_H