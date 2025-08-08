#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include <QPointF>
#include <QSizeF>
#include <QPainter>

#ifdef ENABLE_CAIRO
#include <cairo.h>
#endif


class Rectangle : public Shape
{
public:
    Rectangle(const QPointF &pos = QPointF(), const QSizeF &size = QSizeF(100, 100));
    ~Rectangle() override = default;

    // Drawing
    void draw(QPainter &painter) override;      // Qt-based drawing (for UI rendering)
	#ifdef ENABLE_CAIRO
    void draw(cairo_t *cr) override;           // Cairo-based drawing (for export)
	#endif

    bool contains(const QPointF &point) const override;
    Type getType() const override { return Shape::Rectangle; }
    Rectangle* clone() const override;

    // Rectangle-specific methods
    void setCornerRadius(double radius);
    double getCornerRadius() const;

private:
    double m_cornerRadius;
};

#endif // RECTANGLE_H
