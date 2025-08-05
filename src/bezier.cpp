#include "bezier.h"
#include <QPainterPath>
#include <cmath>

Bezier::Bezier()
    : Shape()
    , m_closed(false)
{
}

void Bezier::draw(void *cr)
{
    if (!isVisible() || m_points.isEmpty()) return;
    
    // Temporarily disable Cairo drawing
    // TODO: Implement Qt-based drawing instead of Cairo
    Q_UNUSED(cr)
}

bool Bezier::contains(const QPointF &point) const
{
    if (m_points.isEmpty()) return false;
    
    // Create a QPainterPath for hit testing
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
    
    // Check if point is inside the path
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
    
    // Update position and size based on bounding box
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
        
        // Update position and size
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