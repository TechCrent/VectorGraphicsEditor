#include "ellipse.h"
#include <QRectF>
#include <cmath>

Ellipse::Ellipse(const QPointF &pos, const QSizeF &size)
    : Shape()
    , m_startAngle(0.0)
    , m_endAngle(360.0)
{
    setPosition(pos);
    setSize(size);
}

void Ellipse::draw(void *cr)
{
    if (!isVisible()) return;
    
    // Temporarily disable Cairo drawing
    // TODO: Implement Qt-based drawing instead of Cairo
    Q_UNUSED(cr)
}

bool Ellipse::contains(const QPointF &point) const
{
    QPointF pos = getPosition();
    QSizeF size = getSize();
    
    // Calculate center and radii
    double centerX = pos.x() + size.width() / 2;
    double centerY = pos.y() + size.height() / 2;
    double radiusX = size.width() / 2;
    double radiusY = size.height() / 2;
    
    // Check if point is inside ellipse
    double dx = point.x() - centerX;
    double dy = point.y() - centerY;
    double normalizedDistance = (dx * dx) / (radiusX * radiusX) + (dy * dy) / (radiusY * radiusY);
    
    return normalizedDistance <= 1.0;
}

Ellipse* Ellipse::clone() const
{
    Ellipse *clone = new Ellipse(getPosition(), getSize());
    clone->setPen(getPen());
    clone->setBrush(getBrush());
    clone->setVisible(isVisible());
    clone->setStartAngle(m_startAngle);
    clone->setEndAngle(m_endAngle);
    return clone;
}

void Ellipse::setStartAngle(double angle)
{
    m_startAngle = angle;
}

void Ellipse::setEndAngle(double angle)
{
    m_endAngle = angle;
}

double Ellipse::getStartAngle() const
{
    return m_startAngle;
}

double Ellipse::getEndAngle() const
{
    return m_endAngle;
} 