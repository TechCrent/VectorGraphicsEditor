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

class Shape; // Forward declaration for shape pointer usage

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddLayerClicked();
    void onRemoveLayerClicked();
    void onLayerDoubleClicked(QListWidgetItem* item);
    void onLayerItemChanged(QListWidgetItem* item);
    void onLayerItemClicked(QListWidgetItem* item);
    void onLayerItemDoubleClicked(QListWidgetItem* item);


    // Document Management
    void newDocument();
    void openDocument();
    void saveDocument();
    void exportSVG();
    void importSVG();

    // Edit
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();

    // Tools
    void selectTool();
    void rectangleTool();
    void ellipseTool();
    void lineTool();
    void bezierTool();
    void penTool();
    void textTool();
    void updateToolActions();

    // View
    void zoomIn();
    void zoomOut();
    void fitToView();
    void showGrid();
    void snapToGrid();

    // Layers
    void addLayer();
    void removeLayer();
    void updateLayersList();

    // Properties
    void chooseFillColor();
    void chooseStrokeColor();
    void strokeWidthChanged(int width);

    // Canvas Interaction
    void shapeSelected(Shape* shape);
    void shapeCreated(Shape* shape);
    void canvasChanged();

    // Help
    void showAbout();

private:
    // Initialization helpers
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

    // UI Components
    QDockWidget *m_layersDock;
    QDockWidget *m_propertiesDock;
    QListWidget *m_layersList;
    QPushButton *m_fillColorButton;
    QPushButton *m_strokeColorButton;
    QSpinBox *m_strokeWidthSpinBox;
};

#endif // MAINWINDOW_H