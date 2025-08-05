#include "line.h"
#include <QLineF>
#include <cmath>

Line::Line(const QPointF &start, const QPointF &end)
    : Shape()
    , m_startPoint(start)
    , m_endPoint(end)
    , m_lineWidth(1.0)
{
    // Calculate position and size based on start and end points
    QPointF minPoint = QPointF(qMin(start.x(), end.x()), qMin(start.y(), end.y()));
    QPointF maxPoint = QPointF(qMax(start.x(), end.x()), qMax(start.y(), end.y()));
    
    setPosition(minPoint);
    setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
}

void Line::draw(void *cr)
{
    if (!isVisible()) return;
    
    // Temporarily disable Cairo drawing
    // TODO: Implement Qt-based drawing instead of Cairo
    Q_UNUSED(cr)
}

bool Line::contains(const QPointF &point) const
{
    QLineF line(m_startPoint, m_endPoint);
    
    // Calculate distance from point to line manually
    QPointF lineVector = m_endPoint - m_startPoint;
    QPointF pointVector = point - m_startPoint;
    
    double lineLengthSquared = lineVector.x() * lineVector.x() + lineVector.y() * lineVector.y();
    if (lineLengthSquared == 0) {
        // Line is actually a point
        return (point - m_startPoint).manhattanLength() <= m_lineWidth / 2;
    }
    
    double t = (pointVector.x() * lineVector.x() + pointVector.y() * lineVector.y()) / lineLengthSquared;
    t = qBound(0.0, t, 1.0);
    
    QPointF closestPoint = m_startPoint + t * lineVector;
    double distance = (point - closestPoint).manhattanLength();
    
    // Consider the line width for hit testing
    return distance <= m_lineWidth / 2;
}

Line* Line::clone() const
{
    Line *clone = new Line(m_startPoint, m_endPoint);
    clone->setPen(getPen());
    clone->setBrush(getBrush());
    clone->setVisible(isVisible());
    clone->setLineWidth(m_lineWidth);
    return clone;
}

void Line::setStartPoint(const QPointF &point)
{
    m_startPoint = point;
    
    // Update position and size
    QPointF minPoint = QPointF(qMin(m_startPoint.x(), m_endPoint.x()), 
                               qMin(m_startPoint.y(), m_endPoint.y()));
    QPointF maxPoint = QPointF(qMax(m_startPoint.x(), m_endPoint.x()), 
                               qMax(m_startPoint.y(), m_endPoint.y()));
    
    setPosition(minPoint);
    setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
}

void Line::setEndPoint(const QPointF &point)
{
    m_endPoint = point;
    
    // Update position and size
    QPointF minPoint = QPointF(qMin(m_startPoint.x(), m_endPoint.x()), 
                               qMin(m_startPoint.y(), m_endPoint.y()));
    QPointF maxPoint = QPointF(qMax(m_startPoint.x(), m_endPoint.x()), 
                               qMax(m_startPoint.y(), m_endPoint.y()));
    
    setPosition(minPoint);
    setSize(QSizeF(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y()));
}

QPointF Line::getStartPoint() const
{
    return m_startPoint;
}

QPointF Line::getEndPoint() const
{
    return m_endPoint;
}

void Line::setLineWidth(double width)
{
    m_lineWidth = width;
}

double Line::getLineWidth() const
{
    return m_lineWidth;
} 