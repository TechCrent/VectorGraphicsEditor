#include "canvas.h"
#include "rectangle.h"
#include "ellipse.h"
#include "line.h"
#include "bezier.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <cmath>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_document(nullptr)
    , m_currentTool(Select)
    , m_zoom(1.0)
    , m_panOffset(0, 0)
    , m_selectedShape(nullptr)
    , m_isSelecting(false)
    , m_isDrawing(false)
    , m_currentShape(nullptr)
    , m_cairoSurface(nullptr)
    , m_cairoContext(nullptr)
    , m_showGrid(true)
    , m_gridSize(20)
    , m_snapToGrid(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

Canvas::~Canvas()
{
    if (m_cairoContext) cairo_destroy(m_cairoContext);
    if (m_cairoSurface) cairo_surface_destroy(m_cairoSurface);
}

void Canvas::setDocument(Document *document)
{
    m_document = document;
    if (m_document) {
        // connect(m_document, &Document::documentChanged, this, &Canvas::update);
    }
    update();
}

Document* Canvas::getDocument() const { return m_document; }
void Canvas::setTool(Tool tool) { m_currentTool = tool; update(); }
Canvas::Tool Canvas::getTool() const { return m_currentTool; }
void Canvas::setZoom(double zoom) { m_zoom = qBound(0.1, zoom, 10.0); update(); }
double Canvas::getZoom() const { return m_zoom; }

void Canvas::fitToView()
{
    if (!m_document) return;
    QSizeF docSize = m_document->getSize();
    QSize widgetSize = size();
    double scaleX = widgetSize.width() / docSize.width();
    double scaleY = widgetSize.height() / docSize.height();
    double scale = qMin(scaleX, scaleY) * 0.9;
    setZoom(scale);
    m_panOffset = QPointF(0, 0);
    update();
}

void Canvas::zoomIn() { setZoom(m_zoom * 1.2); }
void Canvas::zoomOut() { setZoom(m_zoom / 1.2); }

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.translate(m_panOffset);
    painter.scale(m_zoom, m_zoom);
    
    drawBackground(painter);
    if (m_showGrid) drawGrid(painter);
    if (m_document) drawShapes(painter);
    if (m_selectedShape) drawSelectionHandles(painter);
    if (m_isDrawing && m_currentShape) drawShape(m_currentShape, painter);
    
    // Draw Bezier points
    if (m_currentTool == Bezier && !m_bezierPoints.isEmpty()) {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(QBrush(Qt::blue));
        for (const QPointF &point : m_bezierPoints) {
            painter.drawEllipse(point, 3, 3);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    
    switch (m_currentTool) {
        case Select: handleSelectTool(event); break;
        case Rectangle: handleRectangleTool(event); break;
        case Ellipse: handleEllipseTool(event); break;
        case Line: handleLineTool(event); break;
        case Bezier: handleBezierTool(event); break;
    }
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    if (m_snapToGrid) worldPos = snapToGrid(worldPos);
    
    if (m_isDrawing && m_currentShape) {
        m_drawCurrent = worldPos;
        if (m_currentTool == Rectangle || m_currentTool == Ellipse) {
            QSizeF size = QSizeF(qAbs(m_drawCurrent.x() - m_drawStart.x()),
                                qAbs(m_drawCurrent.y() - m_drawStart.y()));
            m_currentShape->setSize(size);
        } else if (m_currentTool == Line) {
            // Line *line = dynamic_cast<Line*>(m_currentShape);
            // if (line) line->setEndPoint(m_drawCurrent);
        }
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isDrawing && m_currentShape) {
        m_isDrawing = false;
        if (m_document) {
            m_document->addShape(m_currentShape);
            emit shapeCreated(m_currentShape);
        }
        m_currentShape = nullptr;
        update();
    }
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            if (m_selectedShape && m_document) {
                m_document->removeShape(m_selectedShape);
                m_selectedShape = nullptr;
                update();
            }
            break;
        case Qt::Key_Escape:
            if (m_currentTool == Bezier) m_bezierPoints.clear();
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (m_currentTool == Bezier && m_bezierPoints.size() >= 2) {
                // Bezier *bezier = new Bezier();
                // for (const QPointF &point : m_bezierPoints) {
                //     bezier->addPoint(point);
                // }
                // if (m_document) {
                //     m_document->addShape(bezier);
                //     emit shapeCreated(bezier);
                // }
                m_bezierPoints.clear();
                update();
            }
            break;
    }
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        double zoomFactor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
        setZoom(m_zoom * zoomFactor);
    } else {
        QPointF delta = event->angleDelta() / 120.0;
        m_panOffset += delta * 10.0;
        update();
    }
}

void Canvas::drawBackground(QPainter &painter)
{
    if (m_document) {
        painter.fillRect(QRectF(QPointF(0, 0), m_document->getSize()), 
                        m_document->getBackgroundColor());
    } else {
        painter.fillRect(rect(), Qt::white);
    }
}

void Canvas::drawShapes(QPainter &painter)
{
    if (!m_document) return;
    
    for (Layer *layer : m_document->getLayers()) {
        if (!layer->isVisible()) continue;
        for (Shape *shape : layer->getShapes()) {
            if (shape->isVisible()) drawShape(shape, painter);
        }
    }
}

void Canvas::drawShape(Shape *shape, QPainter &painter)
{
    if (!shape) return;
    
    // Temporarily disable Cairo drawing
    // TODO: Implement Qt-based drawing instead of Cairo
    Q_UNUSED(painter)
}

void Canvas::drawSelectionHandles(QPainter &painter)
{
    if (!m_selectedShape) return;
    
    QRectF bounds = QRectF(m_selectedShape->getPosition(), m_selectedShape->getSize());
    painter.setPen(QPen(Qt::blue, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(bounds);
}

void Canvas::drawGrid(QPainter &painter)
{
    if (!m_document) return;
    
    QSizeF docSize = m_document->getSize();
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    
    for (double x = 0; x <= docSize.width(); x += m_gridSize) {
        painter.drawLine(QPointF(x, 0), QPointF(x, docSize.height()));
    }
    for (double y = 0; y <= docSize.height(); y += m_gridSize) {
        painter.drawLine(QPointF(0, y), QPointF(docSize.width(), y));
    }
}

QPointF Canvas::screenToWorld(const QPoint &screenPos) const
{
    return (QPointF(screenPos) - m_panOffset) / m_zoom;
}

QPoint Canvas::worldToScreen(const QPointF &worldPos) const
{
    return (worldPos * m_zoom + m_panOffset).toPoint();
}

void Canvas::handleSelectTool(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    if (event->button() == Qt::LeftButton) {
        Shape *shape = m_document ? m_document->getShapeAt(worldPos) : nullptr;
        if (shape) {
            selectShapeAt(worldPos);
        } else {
            clearSelection();
        }
    }
}

void Canvas::handleRectangleTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_isDrawing) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());
        m_drawCurrent = m_drawStart;
        // m_currentShape = new Rectangle(m_drawStart, QSizeF(0, 0));
        m_currentShape = nullptr; // Temporarily disable
    }
}

void Canvas::handleEllipseTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_isDrawing) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());
        m_drawCurrent = m_drawStart;
        // m_currentShape = new Ellipse(m_drawStart, QSizeF(0, 0));
        m_currentShape = nullptr; // Temporarily disable
    }
}

void Canvas::handleLineTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_isDrawing) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());
        m_drawCurrent = m_drawStart;
        // m_currentShape = new Line(m_drawStart, m_drawStart);
        m_currentShape = nullptr; // Temporarily disable
    }
}

void Canvas::handleBezierTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF worldPos = screenToWorld(event->pos());
        m_bezierPoints.append(worldPos);
        update();
    }
}

void Canvas::selectShapeAt(const QPointF &point)
{
    if (m_document) {
        Shape *shape = m_document->getShapeAt(point);
        if (shape) {
            if (m_selectedShape) m_selectedShape->setSelected(false);
            m_selectedShape = shape;
            m_selectedShape->setSelected(true);
            emit shapeSelected(shape);
        }
    }
}

void Canvas::clearSelection()
{
    if (m_selectedShape) {
        m_selectedShape->setSelected(false);
        m_selectedShape = nullptr;
    }
}

void Canvas::updateSelectionRect(const QPointF &point)
{
    m_selectionRect = QRectF(m_selectionStart, point).normalized();
}

cairo_surface_t* Canvas::createCairoSurface()
{
    // Clean up existing surface and context
    if (m_cairoContext) {
        cairo_destroy(m_cairoContext);
        m_cairoContext = nullptr;
    }
    if (m_cairoSurface) {
        cairo_surface_destroy(m_cairoSurface);
        m_cairoSurface = nullptr;
    }
    
    // Create new surface and context
    m_cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width(), height());
    if (m_cairoSurface) {
        m_cairoContext = cairo_create(m_cairoSurface);
    }
    
    return m_cairoSurface;
}

void Canvas::drawWithCairo(QPainter &painter)
{
    if (!m_cairoSurface || !m_cairoContext) {
        return;
    }
    
    // Clear the Cairo surface
    cairo_save(m_cairoContext);
    cairo_set_operator(m_cairoContext, CAIRO_OPERATOR_CLEAR);
    cairo_paint(m_cairoContext);
    cairo_restore(m_cairoContext);
    
    // Set up transformation matrix
    cairo_scale(m_cairoContext, m_zoom, m_zoom);
    cairo_translate(m_cairoContext, m_panOffset.x(), m_panOffset.y());
    
    // Draw all shapes with Cairo
    if (m_document) {
        for (Layer* layer : m_document->getLayers()) {
            if (layer && layer->isVisible()) {
                for (Shape* shape : layer->getShapes()) {
                    if (shape && shape->isVisible()) {
                        shape->draw(m_cairoContext);
                    }
                }
            }
        }
    }
    
    // Convert Cairo surface to QImage and draw it
    cairo_surface_flush(m_cairoSurface);
    unsigned char* data = cairo_image_surface_get_data(m_cairoSurface);
    int width = cairo_image_surface_get_width(m_cairoSurface);
    int height = cairo_image_surface_get_height(m_cairoSurface);
    int stride = cairo_image_surface_get_stride(m_cairoSurface);
    
    QImage image(data, width, height, stride, QImage::Format_ARGB32_Premultiplied);
    painter.drawImage(0, 0, image);
}

QPointF Canvas::snapToGrid(const QPointF &point) const
{
    if (!m_snapToGrid) return point;
    double x = qRound(point.x() / m_gridSize) * m_gridSize;
    double y = qRound(point.y() / m_gridSize) * m_gridSize;
    return QPointF(x, y);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Recreate Cairo surface with new size
    createCairoSurface();

    update();
}