#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse : public Shape
{
public:
    Ellipse(const QPointF &pos = QPointF(), const QSizeF &size = QSizeF(100, 100));
    ~Ellipse() override = default;

    void draw(cairo_t *cr) override;
    bool contains(const QPointF &point) const override;
    Type getType() const override { return Shape::Ellipse; }
    Ellipse* clone() const override;

    // Ellipse-specific methods
    void setStartAngle(double angle);
    void setEndAngle(double angle);
    double getStartAngle() const;
    double getEndAngle() const;

private:
    double m_startAngle;
    double m_endAngle;
};

#endif // ELLIPSE_H 