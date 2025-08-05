#include "ellipse.h"
#include <QRectF>
#include <cmath>
#include <cairo.h>

Ellipse::Ellipse(const QPointF &pos, const QSizeF &size)
    : Shape()
    , m_startAngle(0.0)
    , m_endAngle(360.0)
{
    setPosition(pos);
    setSize(size);
}

void Ellipse::draw(cairo_t *cr)
{
    if (!isVisible() || !cr) return;
    
    QPointF pos = getPosition();
    QSizeF size = getSize();
    QPen pen = getPen();
    QBrush brush = getBrush();
    
    // Calculate center and radii
    double centerX = pos.x() + size.width() / 2;
    double centerY = pos.y() + size.height() / 2;
    double radiusX = size.width() / 2;
    double radiusY = size.height() / 2;
    
    cairo_save(cr);
    
    // Scale for ellipse
    cairo_translate(cr, centerX, centerY);
    cairo_scale(cr, radiusX, radiusY);
    
    // Create circular arc (will be scaled to ellipse)
    double startAngleRad = m_startAngle * M_PI / 180.0;
    double endAngleRad = m_endAngle * M_PI / 180.0;
    
    if (fabs(m_endAngle - m_startAngle) >= 360.0) {
        // Full ellipse
        cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
    } else {
        // Arc
        cairo_arc(cr, 0, 0, 1, startAngleRad, endAngleRad);
    }
    
    cairo_restore(cr);
    
    // Draw filled ellipse if brush is not transparent
    if (brush.style() != Qt::NoBrush) {
        cairo_save(cr);
        cairo_translate(cr, centerX, centerY);
        cairo_scale(cr, radiusX, radiusY);
        
        if (fabs(m_endAngle - m_startAngle) >= 360.0) {
            cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
        } else {
            cairo_arc(cr, 0, 0, 1, startAngleRad, endAngleRad);
        }
        
        QColor fillColor = brush.color();
        cairo_set_source_rgba(cr, fillColor.redF(), fillColor.greenF(), fillColor.blueF(), fillColor.alphaF());
        cairo_fill_preserve(cr);
        cairo_restore(cr);
    }
    
    // Draw stroke if pen is not transparent
    if (pen.style() != Qt::NoPen) {
        cairo_save(cr);
        cairo_translate(cr, centerX, centerY);
        cairo_scale(cr, radiusX, radiusY);
        
        if (fabs(m_endAngle - m_startAngle) >= 360.0) {
            cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
        } else {
            cairo_arc(cr, 0, 0, 1, startAngleRad, endAngleRad);
        }
        
        QColor strokeColor = pen.color();
        cairo_set_source_rgba(cr, strokeColor.redF(), strokeColor.greenF(), strokeColor.blueF(), strokeColor.alphaF());
        cairo_set_line_width(cr, pen.widthF() / ((radiusX + radiusY) / 2)); // Adjust line width for scaling
        cairo_stroke(cr);
        cairo_restore(cr);
    }
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