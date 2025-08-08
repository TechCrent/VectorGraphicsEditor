#include "ellipse.h"
#include <QRectF>
#include <QPainter>
#include <cmath>

#ifdef ENABLE_CAIRO
#include <cairo.h>
#endif


Ellipse::Ellipse(const QPointF &pos, const QSizeF &size)
    : Shape()
    , m_startAngle(0.0)
    , m_endAngle(360.0)
{
    setPosition(pos);
    setSize(size);
}

// =========================
// Qt Drawing
// =========================
void Ellipse::draw(QPainter &painter)
{
    if (!isVisible()) return;

    QPointF pos = getPosition();
    QSizeF size = getSize();

    QRectF rect(pos, size);

    painter.save();
    painter.setPen(getPen());
    painter.setBrush(getBrush());

    double spanAngle = m_endAngle - m_startAngle;

    if (std::fabs(spanAngle) >= 360.0) {
        // Full ellipse
        painter.drawEllipse(rect);
    }
    else {
        // Partial arc (convert to Qt's 16th degree format)
        painter.drawArc(rect, static_cast<int>(-m_startAngle * 16), static_cast<int>(-spanAngle * 16));
    }

    painter.restore();
}

// =========================
// Cairo Drawing
// =========================
#ifdef ENABLE_CAIRO
void Ellipse::draw(cairo_t *cr)
{
    if (!isVisible() || !cr) return;

    QPointF pos = getPosition();
    QSizeF size = getSize();
    QPen pen = getPen();
    QBrush brush = getBrush();

    double centerX = pos.x() + size.width() / 2.0;
    double centerY = pos.y() + size.height() / 2.0;
    double radiusX = size.width() / 2.0;
    double radiusY = size.height() / 2.0;

    double startAngleRad = m_startAngle * M_PI / 180.0;
    double endAngleRad   = m_endAngle   * M_PI / 180.0;

    // Fill
    if (brush.style() != Qt::NoBrush) {
        cairo_save(cr);
        cairo_translate(cr, centerX, centerY);
        cairo_scale(cr, radiusX, radiusY);

        if (std::fabs(m_endAngle - m_startAngle) >= 360.0) {
            cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
        }
        else {
            cairo_arc(cr, 0, 0, 1, startAngleRad, endAngleRad);
            cairo_line_to(cr, 0, 0);
            cairo_close_path(cr);
        }

        QColor fillColor = brush.color();
        cairo_set_source_rgba(cr, fillColor.redF(), fillColor.greenF(), fillColor.blueF(), fillColor.alphaF());
        cairo_fill_preserve(cr);
        cairo_restore(cr);
    }

    // Stroke
    if (pen.style() != Qt::NoPen) {
        cairo_save(cr);
        cairo_translate(cr, centerX, centerY);
        cairo_scale(cr, radiusX, radiusY);

        if (std::fabs(m_endAngle - m_startAngle) >= 360.0) {
            cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
        }
        else {
            cairo_arc(cr, 0, 0, 1, startAngleRad, endAngleRad);
        }

        QColor strokeColor = pen.color();
        cairo_set_source_rgba(cr, strokeColor.redF(), strokeColor.greenF(), strokeColor.blueF(), strokeColor.alphaF());
        cairo_set_line_width(cr, pen.widthF() / ((radiusX + radiusY) / 2.0));
        cairo_stroke(cr);
        cairo_restore(cr);
    }
}
#endif

// =========================
// Hit-Testing
// =========================
bool Ellipse::contains(const QPointF &point) const
{
    QPointF pos = getPosition();
    QSizeF size = getSize();

    double centerX = pos.x() + size.width() / 2.0;
    double centerY = pos.y() + size.height() / 2.0;
    double radiusX = size.width() / 2.0;
    double radiusY = size.height() / 2.0;

    double dx = point.x() - centerX;
    double dy = point.y() - centerY;
    double normalizedDistance = (dx * dx) / (radiusX * radiusX) + (dy * dy) / (radiusY * radiusY);

    return normalizedDistance <= 1.0;
}

// =========================
// Clone
// =========================
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

// =========================
// Ellipse-specific Methods
// =========================
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
