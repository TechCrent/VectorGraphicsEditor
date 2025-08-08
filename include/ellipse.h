#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"
#include <QPointF>
#include <QSizeF>

#ifdef ENABLE_CAIRO
#include <cairo.h>
#endif


class Ellipse : public Shape
{
public:
    Ellipse(const QPointF &pos = QPointF(), const QSizeF &size = QSizeF(100, 100));
    ~Ellipse() override = default;

    // Drawing
    void draw(QPainter &painter) override;   // Qt drawing
	#ifdef ENABLE_CAIRO
    void draw(cairo_t *cr) override;        // Cairo drawing
	#endif

    // Logic
    bool contains(const QPointF &point) const override;
    Type getType() const override { return Shape::Ellipse; }
    Ellipse* clone() const override;

    // Ellipse-specific methods
    void setStartAngle(double angle);
    void setEndAngle(double angle);
    double getStartAngle() const;
    double getEndAngle() const;

private:
    double m_startAngle;  // Degrees
    double m_endAngle;    // Degrees
};

#endif // ELLIPSE_H
