#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>

ColorStrip::ColorStrip(StripType type, QWidget *parent)
    : QWidget(parent), m_type(type), m_value(1.0), m_baseColor(Qt::white), m_isDragging(false)
{
    setMinimumHeight(20);
    updatePixmap();
}

double ColorStrip::value() const {
    return m_value;
}

void ColorStrip::setValue(double value) {
    m_value = qBound(0.0, value, 1.0);
    update();
    emit valueChanged(m_value);
}

void ColorStrip::setBaseColor(const QColor &color) {
    m_baseColor = color;
    updatePixmap();
    update();
}

void ColorStrip::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(rect(), m_stripPixmap);
    int x = m_value * width();
    painter.setPen(Qt::black);
    painter.drawLine(x, 0, x, height());
}

void ColorStrip::mousePressEvent(QMouseEvent *event) {
    m_isDragging = true;
    updateValue(event->pos());
}

void ColorStrip::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging)
        updateValue(event->pos());
}

void ColorStrip::updateValue(const QPoint &pos) {
    m_value = qBound(0.0, static_cast<double>(pos.x()) / width(), 1.0);
    update();
    emit valueChanged(m_value);
}

void ColorStrip::updatePixmap() {
    m_stripPixmap = QPixmap(size());
    m_stripPixmap.fill(Qt::transparent);
    QPainter p(&m_stripPixmap);
    for (int x = 0; x < width(); ++x) {
        QColor color;
        double ratio = static_cast<double>(x) / width();
        if (m_type == Brightness)
            color.setHsvF(m_baseColor.hueF(), m_baseColor.saturationF(), ratio);
        else // Alpha
            color = QColor(m_baseColor.red(), m_baseColor.green(), m_baseColor.blue(), int(ratio * 255));
        p.setPen(color);
        p.drawLine(x, 0, x, height());
    }
}
