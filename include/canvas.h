#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <cairo.h>
#include "document.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    enum Tool {
        Select,
        Rectangle,
        Ellipse,
        Line,
        Bezier
    };
    Canvas(QWidget *parent = nullptr);
    ~Canvas();
    void setDocument(Document *document);
    Document* getDocument() const;
    void setTool(Tool tool);
    Tool getTool() const;
    void setZoom(double zoom);
    double getZoom() const;
    void fitToView();
    void zoomIn();
    void zoomOut();
    cairo_surface_t* createCairoSurface(); // Moved to public
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void drawBackground(QPainter &painter);
    void drawShapes(QPainter &painter);
    void drawShape(Shape *shape, QPainter &painter);
    void drawSelectionHandles(QPainter &painter);
    void drawGrid(QPainter &painter);
    QPointF screenToWorld(const QPoint &screenPos) const;
    QPoint worldToScreen(const QPointF &worldPos) const;
    void handleSelectTool(QMouseEvent *event);
    void handleRectangleTool(QMouseEvent *event);
    void handleEllipseTool(QMouseEvent *event);
    void handleLineTool(QMouseEvent *event);
    void handleBezierTool(QMouseEvent *event);
    void selectShapeAt(const QPointF &point);
    void clearSelection();
    void updateSelectionRect(const QPointF &point);
    void drawWithCairo(QPainter &painter);
private:
    Document *m_document;
    Tool m_currentTool;
    double m_zoom;
    QPointF m_panOffset;
    Shape *m_selectedShape;
    QRectF m_selectionRect;
    bool m_isSelecting;
    QPointF m_selectionStart;
    bool m_isDrawing;
    QPointF m_drawStart;
    QPointF m_drawCurrent;
    Shape *m_currentShape;
    QList<QPointF> m_bezierPoints;
    cairo_surface_t *m_cairoSurface;
    cairo_t *m_cairoContext;
    bool m_showGrid;
    double m_gridSize;
    bool m_snapToGrid;
    QPointF snapToGrid(const QPointF &point) const;
signals:
    void shapeSelected(Shape *shape);
    void shapeCreated(Shape *shape);
    void canvasChanged();
};

#endif // CANVAS_H