#include "bezier.h"
#include <QPainterPath>
#include <cmath>
#include <cairo.h>

Bezier::Bezier()
    : Shape()
    , m_closed(false)
{
}

void Bezier::draw(cairo_t *cr)
{
    if (!isVisible() || m_points.isEmpty() || !cr) return;
    
    QPen pen = getPen();
    QBrush brush = getBrush();
    
    cairo_save(cr);
    
    cairo_move_to(cr, m_points[0].x(), m_points[0].y());

    for (int i = 1; i < m_points.size(); i += 3) {
        if (i + 2 < m_points.size()) {
            cairo_curve_to(cr,
                          m_points[i].x(), m_points[i].y(),
                          m_points[i + 1].x(), m_points[i + 1].y(),
                          m_points[i + 2].x(), m_points[i + 2].y());
        } else if (i + 1 < m_points.size()) {
            QPointF current = m_points[i - 1];
            QPointF control = m_points[i];
            QPointF end = m_points[i + 1];

            QPointF cp1 = current + (control - current) * (2.0/3.0);
            QPointF cp2 = end + (control - end) * (2.0/3.0);

            cairo_curve_to(cr, cp1.x(), cp1.y(), cp2.x(), cp2.y(), end.x(), end.y());
        } else {
            cairo_line_to(cr, m_points[i].x(), m_points[i].y());
        }
    }

    if (m_closed) {
        cairo_close_path(cr);
    }

    if (brush.style() != Qt::NoBrush && m_closed) {
        QColor fillColor = brush.color();
        cairo_set_source_rgba(cr, fillColor.redF(), fillColor.greenF(), fillColor.blueF(), fillColor.alphaF());
        cairo_fill_preserve(cr);
    }

    if (pen.style() != Qt::NoPen) {
        QColor strokeColor = pen.color();
        cairo_set_source_rgba(cr, strokeColor.redF(), strokeColor.greenF(), strokeColor.blueF(), strokeColor.alphaF());
        cairo_set_line_width(cr, pen.widthF());
        cairo_stroke(cr);
    }

    cairo_restore(cr);
}

bool Bezier::contains(const QPointF &point) const
{
    if (m_points.isEmpty()) return false;

    QPainterPath path;

    if (m_points.size() >= 2) {
        path.moveTo(m_points[0]);

        for (int i = 1; i < m_points.size(); i += 3) {
            if (i + 2 < m_points.size()) {
                path.cubicTo(m_points[i], m_points[i + 1], m_points[i + 2]);
            } else if (i + 1 < m_points.size()) {
                path.quadTo(m_points[i], m_points[i + 1]);
            } else {
                path.lineTo(m_points[i]);
            }
        }

        if (m_closed) {
            path.closeSubpath();
        }
    }

    return path.contains(point);
}

Bezier* Bezier::clone() const
{
    Bezier *clone = new Bezier();
    clone->setPen(getPen());
    clone->setBrush(getBrush());
    clone->setVisible(isVisible());
    clone->m_points = m_points;
    clone->m_closed = m_closed;
    return clone;
}

void Bezier::addPoint(const QPointF &point)
{
    m_points.append(point);

    if (m_points.size() == 1) {
        setPosition(point);
        setSize(QSizeF(0, 0));
    } else {
        QPointF minPoint = m_points[0];
        QPointF maxPoint = m_points[0];

        for (const QPointF &p : m_points) {
            minPoint.setX(qMin(minPoint.x(), p.x()));
            minPoint.setY(qMin(minPoint.y(), p.y()));
            maxPoint.setX(qMax(maxPoint.x(), p.x()));
            maxPoint.setY(qMax(maxPoint.y(), p.y()));
        }

        setPosition(minPoint);
        setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
    }
}

void Bezier::setPoint(int index, const QPointF &point)
{
    if (index >= 0 && index < m_points.size()) {
        m_points[index] = point;

        if (!m_points.isEmpty()) {
            QPointF minPoint = m_points[0];
            QPointF maxPoint = m_points[0];

            for (const QPointF &p : m_points) {
                minPoint.setX(qMin(minPoint.x(), p.x()));
                minPoint.setY(qMin(minPoint.y(), p.y()));
                maxPoint.setX(qMax(maxPoint.x(), p.x()));
                maxPoint.setY(qMax(maxPoint.y(), p.y()));
            }

            setPosition(minPoint);
            setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
        }
    }
}

QPointF Bezier::getPoint(int index) const
{
    if (index >= 0 && index < m_points.size()) {
        return m_points[index];
    }
    return QPointF();
}

QList<QPointF> Bezier::getPoints() const
{
    return m_points.toList();
}

int Bezier::getPointCount() const
{
    return m_points.size();
}

void Bezier::clearPoints()
{
    m_points.clear();
    setPosition(QPointF());
    setSize(QSizeF());
}

void Bezier::setClosed(bool closed)
{
    m_closed = closed;
}

bool Bezier::isClosed() const
{
    return m_closed;
}