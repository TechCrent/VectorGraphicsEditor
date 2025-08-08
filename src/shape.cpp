#include "shape.h"
#include <cmath>

Shape::Shape()
    : m_position(0, 0)
    , m_size(100, 100)
    , m_pen(Qt::black, 1)
    , m_brush(Qt::white)
    , m_visible(true)
    , m_selected(false)
    , m_rotation(0.0)
{
}

// ========================
// Property Setters/Getters
// ========================
void Shape::setPosition(const QPointF &pos)
{
    m_position = pos;
}

QPointF Shape::getPosition() const
{
    return m_position;
}

void Shape::setSize(const QSizeF &size)
{
    m_size = size;
}

QSizeF Shape::getSize() const
{
    return m_size;
}

void Shape::setPen(const QPen &pen)
{
    m_pen = pen;
}

QPen Shape::getPen() const
{
    return m_pen;
}

void Shape::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

QBrush Shape::getBrush() const
{
    return m_brush;
}

void Shape::setVisible(bool visible)
{
    m_visible = visible;
}

bool Shape::isVisible() const
{
    return m_visible;
}

void Shape::setSelected(bool selected)
{
    m_selected = selected;
}

bool Shape::isSelected() const
{
    return m_selected;
}

void Shape::move(const QPointF &offset)
{
    m_position += offset;
}


// ========================
// Transformations
// ========================
void Shape::scale(double factor)
{
    // Store the center before scaling
    QPointF center = getBoundingRect().center();

    // Scale the size
    m_size *= factor;

    // Compute the new top-left so the center stays the same
    QPointF newTopLeft(center.x() - m_size.width() / 2.0,
                       center.y() - m_size.height() / 2.0);

    setPosition(newTopLeft);
}


void Shape::rotate(double angle)
{
    m_rotation += angle;
    if (m_rotation >= 360.0)
        m_rotation -= 360.0;
    else if (m_rotation < 0.0)
        m_rotation += 360.0;
}

// ========================
// Utility
// ========================
QRectF Shape::getBoundingRect() const
{
    return QRectF(m_position, m_size);
}
