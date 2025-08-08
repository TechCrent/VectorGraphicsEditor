#ifndef SHAPE_H
#define SHAPE_H

#include <QPointF>
#include <QSizeF>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QPainter>

#ifdef ENABLE_CAIRO
#include <cairo.h>
#endif


class Shape
{
public:
    enum Type {
        Rectangle,
        Ellipse,
        Line,
        Bezier,
		Text
    };

    Shape();
    virtual ~Shape() = default;

    // ========================
    // Pure virtual methods
    // ========================
	#ifdef ENABLE_CAIRO
    virtual void draw(cairo_t *cr) = 0;          // Cairo-based drawing
    #endif

	virtual void draw(QPainter &painter) = 0;    // QPainter-based drawing
    virtual bool contains(const QPointF &point) const = 0;
    virtual Type getType() const = 0;
    virtual Shape* clone() const = 0;

    // ========================
    // Common properties
    // ========================
    void setPosition(const QPointF &pos);
    QPointF getPosition() const;

    void setSize(const QSizeF &size);
    QSizeF getSize() const;

    void setPen(const QPen &pen);
    QPen getPen() const;

    void setBrush(const QBrush &brush);
    QBrush getBrush() const;

    void setVisible(bool visible);
    bool isVisible() const;

    void setSelected(bool selected);
    bool isSelected() const;

    // ========================
    // Transformations
    // ========================
	double getRotation() const { return m_rotation; }

    virtual void move(const QPointF &offset);
    virtual void scale(double factor);
    virtual void rotate(double angle);

    // ========================
    // Utilities
    // ========================
    virtual QRectF getBoundingRect() const;

protected:
    QPointF m_position;
    QSizeF m_size;
    QPen m_pen;
    QBrush m_brush;
    bool m_visible;
    bool m_selected;
    double m_rotation;
};

#endif // SHAPE_H
