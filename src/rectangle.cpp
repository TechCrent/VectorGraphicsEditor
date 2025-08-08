#include "rectangle.h"
#include <QRectF>
#include <QPainter>
#include <cmath>

#ifdef ENABLE_CAIRO
#include <cairo.h>
#endif


Rectangle::Rectangle(const QPointF &pos, const QSizeF &size)
    : Shape()
    , m_cornerRadius(0.0)
{
    setPosition(pos);
    setSize(size);
}

// =========================
// Qt Drawing
// =========================
void Rectangle::draw(QPainter &painter)
{
    if (!isVisible()) return;

    QPointF pos = getPosition();
    QSizeF size = getSize();

    painter.save();
    painter.setPen(getPen());
    painter.setBrush(getBrush());

    QRectF rect(pos, size);

    if (m_cornerRadius > 0.0) {
        painter.drawRoundedRect(rect, m_cornerRadius, m_cornerRadius);
    } else {
        painter.drawRect(rect);
    }

    painter.restore();
}

// =========================
// Cairo Drawing
// =========================
#ifdef ENABLE_CAIRO
void Rectangle::draw(cairo_t *cr)
{
    if (!isVisible() || !cr) return;

    QPointF pos = getPosition();
    QSizeF size = getSize();
    QPen pen = getPen();
    QBrush brush = getBrush();

    // Fill
    if (brush.style() != Qt::NoBrush) {
        cairo_save(cr);

        if (m_cornerRadius > 0.0) {
            double x = pos.x(), y = pos.y();
            double w = size.width(), h = size.height();
            double r = m_cornerRadius;

            cairo_new_sub_path(cr);
            cairo_arc(cr, x + r, y + r, r, M_PI, 3 * M_PI / 2);
            cairo_arc(cr, x + w - r, y + r, r, 3 * M_PI / 2, 0);
            cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI / 2);
            cairo_arc(cr, x + r, y + h - r, r, M_PI / 2, M_PI);
            cairo_close_path(cr);
        } else {
            cairo_rectangle(cr, pos.x(), pos.y(), size.width(), size.height());
        }

        QColor fillColor = brush.color();
        cairo_set_source_rgba(cr, fillColor.redF(), fillColor.greenF(), fillColor.blueF(), fillColor.alphaF());
        cairo_fill_preserve(cr);
        cairo_restore(cr);
    }

    // Stroke
    if (pen.style() != Qt::NoPen) {
        cairo_save(cr);

        if (m_cornerRadius > 0.0) {
            double x = pos.x(), y = pos.y();
            double w = size.width(), h = size.height();
            double r = m_cornerRadius;

            cairo_new_sub_path(cr);
            cairo_arc(cr, x + r, y + r, r, M_PI, 3 * M_PI / 2);
            cairo_arc(cr, x + w - r, y + r, r, 3 * M_PI / 2, 0);
            cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI / 2);
            cairo_arc(cr, x + r, y + h - r, r, M_PI / 2, M_PI);
            cairo_close_path(cr);
        } else {
            cairo_rectangle(cr, pos.x(), pos.y(), size.width(), size.height());
        }

        QColor strokeColor = pen.color();
        cairo_set_source_rgba(cr, strokeColor.redF(), strokeColor.greenF(), strokeColor.blueF(), strokeColor.alphaF());
        cairo_set_line_width(cr, pen.widthF());
        cairo_stroke(cr);
        cairo_restore(cr);
    }
}
#endif

// =========================
// Other Methods
// =========================
bool Rectangle::contains(const QPointF &point) const
{
    QPointF pos = getPosition();
    QSizeF size = getSize();
    QRectF rect(pos, size);

    // NOTE: Rounded corner hit-testing can be added later if needed
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
