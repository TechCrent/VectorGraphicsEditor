#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape
{
public:
    Line(const QPointF &start = QPointF(), const QPointF &end = QPointF(100, 100));
    ~Line() override = default;

    void draw(void *cr) override;
    bool contains(const QPointF &point) const override;
    Type getType() const override { return Shape::Line; }
    Line* clone() const override;

    // Line-specific methods
    void setStartPoint(const QPointF &point);
    void setEndPoint(const QPointF &point);
    QPointF getStartPoint() const;
    QPointF getEndPoint() const;

    void setLineWidth(double width);
    double getLineWidth() const;

private:
    QPointF m_startPoint;
    QPointF m_endPoint;
    double m_lineWidth;
};

#endif // LINE_H 