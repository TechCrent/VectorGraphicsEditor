#include "text.h"
#include <QPainter>

Text::Text()
    : m_text("Sample Text")
{
    setPosition(QPointF(0, 0));
    setSize(QSizeF(100, 50));
}

void Text::draw(QPainter &painter)
{
    if (!isVisible()) return;

    painter.save();
    painter.setPen(getPen());
    painter.setFont(QFont("Arial", 14));
    painter.drawText(QRectF(getPosition(), getSize()), Qt::AlignLeft, m_text);
    painter.restore();
}

bool Text::contains(const QPointF &point) const
{
    return QRectF(getPosition(), getSize()).contains(point);
}

Text* Text::clone() const
{
    Text* t = new Text();
    t->setText(m_text);
    t->setPosition(getPosition());
    t->setSize(getSize());
    t->setPen(getPen());
    t->setBrush(getBrush());
    t->setVisible(isVisible());
    return t;
}

void Text::setText(const QString &text) { m_text = text; }
QString Text::getText() const { return m_text; }
