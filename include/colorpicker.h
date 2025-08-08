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

#include "colorwheel.h"
#include "colorstrip.h"

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

#endif // COLORPICKER_H
