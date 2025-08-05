#include "rectangle.h"
#include <QRectF>
#include <cmath>

Rectangle::Rectangle(const QPointF &pos, const QSizeF &size)
    : Shape()
    , m_cornerRadius(0.0)
{
    setPosition(pos);
    setSize(size);
}

void Rectangle::draw(void *cr)
{
    if (!isVisible()) return;
    
    // Temporarily disable Cairo drawing
    // TODO: Implement Qt-based drawing instead of Cairo
    Q_UNUSED(cr)
}

bool Rectangle::contains(const QPointF &point) const
{
    QPointF pos = getPosition();
    QSizeF size = getSize();
    QRectF rect(pos, size);
    
    // Simple rectangle containment for now
    // TODO: Implement proper rounded rectangle containment
    return rect.contains(point);
}

Rectangle* Rectangle::clone() const
{
    Rectangle *clone = new Rectangle(getPosition(), getSize());
    clone->setPen(getPen());
    clone->setBrush(getBrush());
    clone->setVisible(isVisible());
    clone->setCornerRadius(m_cornerRadius);
    return clone;
}

void Rectangle::setCornerRadius(double radius)
{
    m_cornerRadius = radius;
}

double Rectangle::getCornerRadius() const
{
    return m_cornerRadius;
} 