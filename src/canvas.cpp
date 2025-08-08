#include "canvas.h"
#include "document.h"
#include "shape.h"
#include "rectangle.h"
#include "ellipse.h"
#include "line.h"
#include "bezier.h"
#include "svg_parser.h"
#include "text.h"

#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QInputDialog>
#include <QResizeEvent>
#include <QClipboard>
#include <QApplication>
#include <QSvgGenerator>
#include <QDebug>
#include <cmath>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_document(nullptr)
    , m_currentTool(Tool_Select)
    , m_zoom(1.0)
    , m_panOffset(0, 0)
    , m_selectedShape(nullptr)
    , m_isSelecting(false)
    , m_isDrawing(false)
    , m_currentShape(nullptr)
#ifdef ENABLE_CAIRO
    , m_cairoSurface(nullptr)
    , m_cairoContext(nullptr)
#endif
    , m_showGrid(true)
    , m_gridSize(20)
    , m_snapToGrid(false)

{
	m_strokePen = QPen(Qt::black, 2);   // Default stroke color and width
	m_fillBrush = Qt::NoBrush;          // Default fill color (transparent)

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);

#ifdef ENABLE_CAIRO
    createCairoSurface();
#endif
}

Canvas::~Canvas()
{
#ifdef ENABLE_CAIRO
    if (m_cairoContext) cairo_destroy(m_cairoContext);
    if (m_cairoSurface) cairo_surface_destroy(m_cairoSurface);
#endif
}

void Canvas::setDocument(Document *document)
{
    m_document = document;
    update();
}

Document* Canvas::getDocument() const
{
    return m_document;
}

void Canvas::setTool(Tool tool)
{
    m_currentTool = tool;
    m_bezierPoints.clear();
    update();
}

Canvas::Tool Canvas::getTool() const
{
    return m_currentTool;
}

void Canvas::setZoom(double zoom)
{
    m_zoom = qBound(0.1, zoom, 10.0);
    update();
}

double Canvas::getZoom() const
{
    return m_zoom;
}

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

void Canvas::zoomIn()
{
    setZoom(m_zoom * 1.2);
}

void Canvas::zoomOut()
{
    setZoom(m_zoom / 1.2);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    if (m_showGrid) drawGrid(painter);

	#ifdef ENABLE_CAIRO
    drawWithCairo(painter);
	#else
    if (m_document) {
        painter.save();
        painter.translate(m_panOffset * m_zoom);
        painter.scale(m_zoom, m_zoom);
        const QList<Shape*> shapes = m_document->getShapes();
        for (Shape *shape : shapes) {
            if (shape) shape->draw(painter);
        }
        painter.restore();
    }
#endif

    // Draw current shape during drawing
    if (m_isDrawing && m_currentShape) {
        painter.save();
        painter.translate(m_panOffset * m_zoom);
        painter.scale(m_zoom, m_zoom);
        m_currentShape->draw(painter);
        painter.restore();
    }

    if (m_selectedShape) drawSelectionHandles(painter);

    if (m_currentTool == Tool_Bezier && !m_bezierPoints.isEmpty()) {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(Qt::blue);

        for (const QPointF &point : m_bezierPoints) {
            painter.drawEllipse(point, 3, 3);
        }

        if (m_bezierPoints.size() >= 2) {
            QPainterPath path(m_bezierPoints[0]);
            for (int i = 1; i < m_bezierPoints.size(); ++i) {
                path.lineTo(m_bezierPoints[i]);
            }
            painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
            painter.drawPath(path);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    if (m_snapToGrid) worldPos = snapToGrid(worldPos);

    switch (m_currentTool) {
        case Tool_Select:    handleSelectTool(event); break;
        case Tool_Rectangle: handleRectangleTool(event); break;
        case Tool_Ellipse:   handleEllipseTool(event); break;
        case Tool_Line:      handleLineTool(event); break;
        case Tool_Bezier:    handleBezierTool(event); break;
		case Tool_Pen: handlePenTool(event); break;
		case Tool_Text: handleTextTool(event); break;
        default: break; // Ignore unimplemented tools (Tool_Pen, Tool_Text)
    }

    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    if (m_snapToGrid) worldPos = snapToGrid(worldPos);

    if (m_isDrawing && m_currentShape) {
        m_drawCurrent = worldPos;

        if (m_currentTool == Tool_Rectangle || m_currentTool == Tool_Ellipse) {
            QSizeF size(qAbs(m_drawCurrent.x() - m_drawStart.x()),
                        qAbs(m_drawCurrent.y() - m_drawStart.y()));
            QPointF topLeft(qMin(m_drawStart.x(), m_drawCurrent.x()),
                            qMin(m_drawStart.y(), m_drawCurrent.y()));
            m_currentShape->setPosition(topLeft);
            m_currentShape->setSize(size);
        }
        else if (m_currentTool == Tool_Line) {
            if (auto line = dynamic_cast<Line*>(m_currentShape)) {
                line->setEndPoint(m_drawCurrent);
            }
        }
		else if (m_currentTool == Tool_Pen) {
    		if (auto bezier = dynamic_cast<Bezier*>(m_currentShape)) {
        	bezier->addPoint(screenToWorld(event->pos()));
    		}
		}
		update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (m_isDrawing && m_currentShape) {
        m_isDrawing = false;
        if (m_document) {
            m_document->addShape(m_currentShape);
            emit shapeCreated(m_currentShape);
        } else {
            delete m_currentShape;
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
            if (m_currentTool == Tool_Bezier) {
                m_bezierPoints.clear();
                update();
            }
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (m_currentTool == Tool_Bezier && m_bezierPoints.size() >= 2) {
                auto *bezier = new Bezier();
                for (const QPointF &point : m_bezierPoints) {
                    bezier->addPoint(point);
                }
				bezier->setPen(m_strokePen);   // ✅
				bezier->setBrush(m_fillBrush); // ✅

                if (m_document) {
                    m_document->addShape(bezier);
                    emit shapeCreated(bezier);
                } else {
                    delete bezier;
                }
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
    painter.fillRect(rect(), Qt::white);
}

void Canvas::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(Qt::lightGray, 1));
    for (int x = 0; x < width(); x += m_gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += m_gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}

#ifdef ENABLE_CAIRO
void Canvas::drawWithCairo(QPainter &painter)
{
    if (!m_document) return;

    cairo_save(m_cairoContext);
    cairo_set_source_rgb(m_cairoContext, 1.0, 1.0, 1.0);
    cairo_paint(m_cairoContext);

    cairo_translate(m_cairoContext, m_panOffset.x(), m_panOffset.y());
    cairo_scale(m_cairoContext, m_zoom, m_zoom);

    const QList<Shape*> shapes = m_document->getShapes();
    for (Shape *shape : shapes) {
        if (shape) shape->draw(m_cairoContext);
    }

    cairo_restore(m_cairoContext);

    QImage image(cairo_image_surface_get_data(m_cairoSurface),
                 cairo_image_surface_get_width(m_cairoSurface),
                 cairo_image_surface_get_height(m_cairoSurface),
                 QImage::Format_ARGB32_Premultiplied);
    painter.drawImage(0, 0, image);
}
#endif

void Canvas::drawSelectionHandles(QPainter &painter)
{
    if (!m_selectedShape) return;

    QRectF bounds = m_selectedShape->getBoundingRect();
    painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
    painter.drawRect(bounds);
}

QPointF Canvas::screenToWorld(const QPoint &screenPos) const
{
    return QPointF(screenPos.x() / m_zoom, screenPos.y() / m_zoom) - m_panOffset;
}

QPointF Canvas::worldToScreen(const QPointF &worldPos) const
{
    return (worldPos + m_panOffset) * m_zoom;
}

void Canvas::handleSelectTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selectShapeAt(screenToWorld(event->pos()));
    }
}

void Canvas::handleRectangleTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());
		m_currentShape = new Rectangle();
		m_currentShape->setPosition(m_drawStart);
		m_currentShape->setSize(QSizeF(0, 0));
		m_currentShape->setPen(m_strokePen);   // ✅ Use selected stroke
		m_currentShape->setBrush(m_fillBrush); // ✅ Use selected fill

    }
}

void Canvas::handleEllipseTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());
		m_currentShape = new Ellipse();
		m_currentShape->setPosition(m_drawStart);
		m_currentShape->setSize(QSizeF(0, 0));
		m_currentShape->setPen(m_strokePen);   // ✅ Use selected stroke
		m_currentShape->setBrush(m_fillBrush); // ✅ Use selected fill

    }
}

void Canvas::handleLineTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());
		auto *line = new Line();
		line->setStartPoint(m_drawStart);
		line->setEndPoint(m_drawStart);
		line->setPen(m_strokePen);             // ✅ Use selected stroke
		// Fill color not used for line
		m_currentShape = line;
    }
}

void Canvas::handleBezierTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bezierPoints.append(screenToWorld(event->pos()));
    }
}

void Canvas::handlePenTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_drawStart = screenToWorld(event->pos());

        auto* bezier = new Bezier();  // Reuse Bezier for now
        bezier->addPoint(m_drawStart);
        bezier->setPen(m_strokePen);   // ✅ Use selected stroke pen
        bezier->setBrush(m_fillBrush); // ✅ Use selected fill brush
        m_currentShape = bezier;
    }
}

void Canvas::handleTextTool(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 🔹 Get world position for where to place the text
        QPointF position = screenToWorld(event->pos());

        // 🔹 Show text input dialog
        bool ok;
        QString userText = QInputDialog::getText(this, tr("Enter Text"),
                                                 tr("Text:"), QLineEdit::Normal,
                                                 "", &ok);

        // 🔹 If user confirms and input is not empty
        if (ok && !userText.isEmpty()) {
            auto* text = new Text();
            text->setPosition(position);
            text->setPen(m_strokePen);   // ✅ Use current stroke color
            text->setBrush(m_fillBrush); // ✅ Use current fill color
            text->setText(userText);     // ✅ Use custom user input

            if (m_document) {
                m_document->addShape(text);
                emit shapeCreated(text);
            } else {
                delete text;
            }
        }
    }
}


void Canvas::selectShapeAt(const QPointF &point)
{
    if (!m_document) return;

    const QList<Shape*> shapes = m_document->getShapes();
    for (int i = shapes.size() - 1; i >= 0; --i) {
        Shape* shape = shapes[i];
        if (shape->contains((point - m_panOffset) / m_zoom)){
            m_selectedShape = shape;
            emit shapeSelected(m_selectedShape);
            return;
        }
    }
    m_selectedShape = nullptr;
}

void Canvas::clearSelection()
{
    m_selectedShape = nullptr;
}

QPointF Canvas::snapToGrid(const QPointF &point) const
{
    double x = std::round(point.x() / m_gridSize) * m_gridSize;
    double y = std::round(point.y() / m_gridSize) * m_gridSize;
    return QPointF(x, y);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
#ifdef ENABLE_CAIRO
    createCairoSurface();
#endif
    update();
}

#ifdef ENABLE_CAIRO
cairo_surface_t* Canvas::createCairoSurface()
{
    if (m_cairoSurface) cairo_surface_destroy(m_cairoSurface);
    if (m_cairoContext) cairo_destroy(m_cairoContext);

    m_cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width(), height());
    m_cairoContext = cairo_create(m_cairoSurface);
    return m_cairoSurface;
}
#endif

void Canvas::loadSVG(const QString &filename)
{
    if (!m_document) return;
    SVGParser parser;
    parser.importFromFile(filename, m_document);
    update();
    emit canvasChanged();
}

void Canvas::importSVG(const QString &filename)
{
    if (!m_document) return;
    SVGParser parser;
    parser.importFromFile(filename, m_document);
    update();
    emit canvasChanged();
}

void Canvas::saveSVG(const QString &filename)
{
	if (!m_document) return;
	SVGParser parser;
	parser.exportToFile(filename, m_document);
}


void Canvas::cutSelection()
{
    if (!m_selectedShape || !m_document) return;
    copySelection();
    m_document->removeShape(m_selectedShape);
    m_selectedShape = nullptr;
    update();
}

void Canvas::copySelection()
{
    if (!m_selectedShape) return;
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText("Shape copied (placeholder)");
}

void Canvas::pasteSelection()
{
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard->text().contains("Shape copied")) {
        qDebug() << "Pasted shape (placeholder)";
    }
}

void Canvas::toggleGrid()
{
    m_showGrid = !m_showGrid;
    update();
}

void Canvas::toggleSnapToGrid()
{
    m_snapToGrid = !m_snapToGrid;
}

void Canvas::setFillColor(const QColor &color)
{
    m_fillBrush = QBrush(color); // ✅ Save for new shapes

    if (m_selectedShape) {
        m_selectedShape->setBrush(m_fillBrush);
    }
    update();
}

void Canvas::setStrokeColor(const QColor &color)
{
    m_strokePen.setColor(color); // ✅ Save for new shapes

    if (m_selectedShape) {
        QPen pen = m_selectedShape->getPen();
        pen.setColor(color);
        m_selectedShape->setPen(pen);
    }
    update();
}

void Canvas::setStrokeWidth(int width)
{
    if (m_selectedShape) {
        QPen pen = m_selectedShape->getPen();
        pen.setWidth(width);
        m_selectedShape->setPen(pen);
    }
    update();
}