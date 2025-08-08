#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

ColorWheel::ColorWheel(QWidget *parent)
    : QWidget(parent), m_currentColor(Qt::white), m_isDragging(false)
{
    setMinimumSize(150, 150);
    updatePixmap();
}

QColor ColorWheel::currentColor() const {
    return m_currentColor;
}

void ColorWheel::setCurrentColor(const QColor &color) {
    if (m_currentColor != color) {
        m_currentColor = color;
        m_currentPoint = colorToPoint(color);
        update();
        emit colorChanged(m_currentColor);
    }
}

void ColorWheel::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(rect(), m_wheelPixmap);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(m_currentPoint, 5, 5);
}

void ColorWheel::mousePressEvent(QMouseEvent *event) {
    m_isDragging = true;
    updateColor(event->pos());
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging)
        updateColor(event->pos());
}

void ColorWheel::resizeEvent(QResizeEvent *) {
    updatePixmap();
}

void ColorWheel::updateColor(const QPoint &pos) {
    int cx = width() / 2;
    int cy = height() / 2;
    int dx = pos.x() - cx;
    int dy = pos.y() - cy;
    double angle = qAtan2(dy, dx) * 180.0 / M_PI;
    double radius = qSqrt(dx * dx + dy * dy) / (qMin(width(), height()) / 2.0);
    if (radius <= 1.0) {
        m_currentColor.setHsvF((angle < 0 ? angle + 360 : angle) / 360.0, radius, 1.0);
        m_currentPoint = pos;
        update();
        emit colorChanged(m_currentColor);
    }
}

QPoint ColorWheel::colorToPoint(const QColor &color) const {
    float h, s, v;
    color.getHsvF(&h, &s, &v);

    double angle = h * 2 * M_PI;
    double radius = s * qMin(width(), height()) / 2.0 - 1;
    int x = width() / 2 + static_cast<int>(radius * cos(angle));
    int y = height() / 2 - static_cast<int>(radius * sin(angle));
    return QPoint(x, y);
}


void ColorWheel::updatePixmap() {
    m_wheelPixmap = QPixmap(size());
    m_wheelPixmap.fill(Qt::transparent);
    QPainter p(&m_wheelPixmap);
    int cx = width() / 2;
    int cy = height() / 2;
    int radius = qMin(cx, cy);
    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            int dx = x - cx;
            int dy = y - cy;
            double dist = qSqrt(dx * dx + dy * dy) / radius;
            if (dist > 1.0) continue;
            double angle = qAtan2(dy, dx) * 180.0 / M_PI;
            QColor color;
            color.setHsvF((angle < 0 ? angle + 360 : angle) / 360.0, dist, 1.0);
            p.setPen(color);
            p.drawPoint(x, y);
        }
    }
}
