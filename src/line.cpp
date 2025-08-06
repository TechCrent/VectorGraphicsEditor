#include "line.h"
#include <QLineF>
#include <QPainter>
#include <cmath>
#include <cairo.h>

Line::Line(const QPointF &start, const QPointF &end)
    : Shape()
    , m_startPoint(start)
    , m_endPoint(end)
    , m_lineWidth(1.0)
{
    QPointF minPoint(qMin(start.x(), end.x()), qMin(start.y(), end.y()));
    QPointF maxPoint(qMax(start.x(), end.x()), qMax(start.y(), end.y()));

    setPosition(minPoint);
    setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
}

// ====================
// QPainter Drawing
// ====================
void Line::draw(QPainter &painter)
{
    if (!isVisible()) return;

    QPen pen = getPen();
    if (pen.style() != Qt::NoPen) {
        painter.save();
        painter.setPen(pen);
        painter.drawLine(m_startPoint, m_endPoint);
        painter.restore();
    }
}

// ====================
// Cairo Drawing
// ====================
void Line::draw(cairo_t *cr)
{
    if (!isVisible() || !cr) return;

    QPen pen = getPen();
    if (pen.style() != Qt::NoPen) {
        cairo_save(cr);

        cairo_move_to(cr, m_startPoint.x(), m_startPoint.y());
        cairo_line_to(cr, m_endPoint.x(), m_endPoint.y());

        QColor strokeColor = pen.color();
        cairo_set_source_rgba(cr, strokeColor.redF(), strokeColor.greenF(), strokeColor.blueF(), strokeColor.alphaF());
        cairo_set_line_width(cr, pen.widthF());
        cairo_stroke(cr);

        cairo_restore(cr);
    }
}

// ====================
// Hit Testing
// ====================
bool Line::contains(const QPointF &point) const
{
    const double tolerance = 5.0;
    QLineF line(m_startPoint, m_endPoint);

    QPointF lineVector = m_endPoint - m_startPoint;
    QPointF pointVector = point - m_startPoint;
    double lineLengthSquared = lineVector.x() * lineVector.x() + lineVector.y() * lineVector.y();

    if (lineLengthSquared == 0) {
        return QLineF(point, m_startPoint).length() <= tolerance;
    }

    double t = (pointVector.x() * lineVector.x() + pointVector.y() * lineVector.y()) / lineLengthSquared;
    t = qBound(0.0, t, 1.0);
    QPointF closestPoint = m_startPoint + t * lineVector;
    double distance = QLineF(point, closestPoint).length();

    return distance <= tolerance;
}

// ====================
// Clone
// ====================
Line* Line::clone() const
{
    Line *clone = new Line(m_startPoint, m_endPoint);
    clone->setPen(getPen());
    clone->setBrush(getBrush());
    clone->setVisible(isVisible());
    clone->setLineWidth(m_lineWidth);
    return clone;
}

// ====================
// Setters & Getters
// ====================
void Line::setStartPoint(const QPointF &point)
{
    m_startPoint = point;

    QPointF minPoint(qMin(m_startPoint.x(), m_endPoint.x()), qMin(m_startPoint.y(), m_endPoint.y()));
    QPointF maxPoint(qMax(m_startPoint.x(), m_endPoint.x()), qMax(m_startPoint.y(), m_endPoint.y()));

    setPosition(minPoint);
    setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
}

void Line::setEndPoint(const QPointF &point)
{
    m_endPoint = point;

    QPointF minPoint(qMin(m_startPoint.x(), m_endPoint.x()), qMin(m_startPoint.y(), m_endPoint.y()));
    QPointF maxPoint(qMax(m_startPoint.x(), m_endPoint.x()), qMax(m_startPoint.y(), m_endPoint.y()));

    setPosition(minPoint);
    setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
}

QPointF Line::getStartPoint() const { return m_startPoint; }
QPointF Line::getEndPoint() const { return m_endPoint; }

void Line::setLineWidth(double width) { m_lineWidth = width; }
double Line::getLineWidth() const { return m_lineWidth; }
