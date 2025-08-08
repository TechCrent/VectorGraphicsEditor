#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QPixmap>
#include <QPoint>

class ColorWheel : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWheel(QWidget *parent = nullptr);

    QColor currentColor() const;
    void setCurrentColor(const QColor &color);

    signals:
        void colorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateColor(const QPoint &pos);
    QPoint colorToPoint(const QColor &color) const;
    QColor pointToColor(const QPoint &point) const;
    void updatePixmap();

    QColor m_currentColor;
    QPixmap m_wheelPixmap;
    QPoint m_currentPoint;
    bool m_isDragging;
};

#endif // COLORWHEEL_H
