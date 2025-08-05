#ifndef BEZIER_H
#define BEZIER_H

#include "shape.h"
#include <QVector>

class Bezier : public Shape
{
public:
    Bezier();
    ~Bezier() override = default;

    void draw(void *cr) override;
    bool contains(const QPointF &point) const override;
    Type getType() const override { return Shape::Bezier; }
    Bezier* clone() const override;

    // Bezier-specific methods
    void addPoint(const QPointF &point);
    void setPoint(int index, const QPointF &point);
    QPointF getPoint(int index) const;
    int getPointCount() const;
    void clearPoints();

    void setClosed(bool closed);
    bool isClosed() const;

private:
    QVector<QPointF> m_points;
    bool m_closed;
};

#endif // BEZIER_H 