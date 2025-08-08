#ifndef TEXT_H
#define TEXT_H

#include "shape.h"
#include <QString>

class Text : public Shape
{
public:
    Text();
    ~Text() override = default;

    void draw(QPainter &painter) override;
#ifdef ENABLE_CAIRO
    void draw(cairo_t *cr) override {}
#endif

    bool contains(const QPointF &point) const override;
    Type getType() const override { return Shape::Text; }
    Text* clone() const override;

    void setText(const QString &text);
    QString getText() const;

private:
    QString m_text;
};

#endif // TEXT_H
