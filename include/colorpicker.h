#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class ColorWheel;
class ColorStrip;

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = nullptr);
    
    QColor currentColor() const;
    void setCurrentColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

private slots:
    void onColorWheelChanged(const QColor &color);
    void onRedChanged(int value);
    void onGreenChanged(int value);
    void onBlueChanged(int value);
    void onAlphaChanged(int value);
    void onHexChanged();

private:
    void setupUI();
    void updateUI();
    void updateRGBSliders();
    void updateHexInput();
    
    QColor m_currentColor;
    
    // UI Components
    ColorWheel *m_colorWheel;
    ColorStrip *m_brightnessStrip;
    ColorStrip *m_alphaStrip;
    
    QSlider *m_redSlider;
    QSlider *m_greenSlider;
    QSlider *m_blueSlider;
    QSlider *m_alphaSlider;
    
    QSpinBox *m_redSpinBox;
    QSpinBox *m_greenSpinBox;
    QSpinBox *m_blueSpinBox;
    QSpinBox *m_alphaSpinBox;
    
    QLineEdit *m_hexLineEdit;
    QWidget *m_colorPreview;
};

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

#endif // COLORPICKER_H