#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <cairo.h>

class Shape;
class Document;
class Line;
class Rectangle;
class Ellipse;
class Bezier;

class Canvas : public QWidget
{
    Q_OBJECT

public:
    enum Tool {
        Tool_Select,
        Tool_Rectangle,
        Tool_Ellipse,
        Tool_Line,
        Tool_Bezier
    };

    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    // Document management
    void setDocument(Document *document);
    Document* getDocument() const;

    // Tool management
    void setTool(Tool tool);
    Tool getTool() const;

    // Zoom and view
    void setZoom(double zoom);
    double getZoom() const;
    void fitToView();
    void zoomIn();
    void zoomOut();

    // Cairo surface
    cairo_surface_t* createCairoSurface();

    // SVG and editing operations
    void loadSVG(const QString &filename);
    void saveSVG(const QString &filename);
    void importSVG(const QString &filename);

    // Clipboard operations
    void cutSelection();
    void copySelection();
    void pasteSelection();

    // View options
    void toggleGrid();
    void toggleSnapToGrid();

    // Shape styling
    void setFillColor(const QColor &color);
    void setStrokeColor(const QColor &color);
    void setStrokeWidth(int width);

protected:
    // Event handlers
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    // Drawing helpers
    void drawBackground(QPainter &painter);
    void drawWithCairo(QPainter &painter);     // Cairo backend
    void drawSelectionHandles(QPainter &painter);
    void drawGrid(QPainter &painter);

    // Coordinate helpers
    QPointF screenToWorld(const QPoint &screenPos) const;
    QPointF worldToScreen(const QPointF &worldPos) const;

    // Tool handling
    void handleSelectTool(QMouseEvent *event);
    void handleRectangleTool(QMouseEvent *event);
    void handleEllipseTool(QMouseEvent *event);
    void handleLineTool(QMouseEvent *event);
    void handleBezierTool(QMouseEvent *event);

    // Shape selection
    void selectShapeAt(const QPointF &point);
    void clearSelection();

    // Grid snapping
    QPointF snapToGrid(const QPointF &point) const;

private:
    Document *m_document;             // Current document
    Tool m_currentTool;               // Active tool
    double m_zoom;                    // Zoom factor
    QPointF m_panOffset;              // Canvas pan offset

    Shape *m_selectedShape;           // Currently selected shape
    bool m_isSelecting;               // Selection state
    bool m_isDrawing;                 // Drawing state

    QPointF m_selectionStart;         // Selection start point
    QPointF m_drawStart;              // Drawing start point
    QPointF m_drawCurrent;            // Drawing current point

    Shape *m_currentShape;            // Shape being drawn
    QList<QPointF> m_bezierPoints;    // Points for Bezier curves

    cairo_surface_t *m_cairoSurface;  // Cairo surface
    cairo_t *m_cairoContext;          // Cairo context

    bool m_showGrid;                  // Grid visibility
    int m_gridSize;                   // Grid size
    bool m_snapToGrid;                // Snap-to-grid state

signals:
    void shapeSelected(Shape *shape);
    void shapeCreated(Shape *shape);
    void canvasChanged();
};

#endif // CANVAS_H
