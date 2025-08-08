#ifndef COLORSTRIP_H
#define COLORSTRIP_H

#include <QWidget>
#include <QColor>
#include <QPixmap>

class ColorStrip : public QWidget
{
    Q_OBJECT

public:
    enum StripType {
        Brightness,
        Alpha
    };

    explicit ColorStrip(StripType type, QWidget *parent = nullptr);

    double value() const;
    void setValue(double value);
    void setBaseColor(const QColor &color);

    signals:
        void valueChanged(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updatePixmap();
    void updateValue(const QPoint &pos);

    StripType m_type;
    double m_value;
    QColor m_baseColor;
    QPixmap m_stripPixmap;
    bool m_isDragging;
};

#endif // COLORSTRIP_H
