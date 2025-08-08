#include "colorpicker.h"
#include "colorwheel.h"
#include "colorstrip.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QColorDialog>

//
// ─── COLOR PICKER ──────────────────────────────────────────────────────────────
//

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent),
      m_colorWheel(new ColorWheel(this)),
      m_brightnessStrip(new ColorStrip(ColorStrip::Brightness, this)),
      m_alphaStrip(new ColorStrip(ColorStrip::Alpha, this)),
      m_redSlider(new QSlider(Qt::Horizontal, this)),
      m_greenSlider(new QSlider(Qt::Horizontal, this)),
      m_blueSlider(new QSlider(Qt::Horizontal, this)),
      m_alphaSlider(new QSlider(Qt::Horizontal, this)),
      m_redSpinBox(new QSpinBox(this)),
      m_greenSpinBox(new QSpinBox(this)),
      m_blueSpinBox(new QSpinBox(this)),
      m_alphaSpinBox(new QSpinBox(this)),
      m_hexLineEdit(new QLineEdit(this)),
      m_colorPreview(new QWidget(this))
{
    m_currentColor = Qt::black;
    setupUI();
    updateUI();
}

QColor ColorPicker::currentColor() const {
    return m_currentColor;
}

void ColorPicker::setCurrentColor(const QColor &color) {
    m_currentColor = color;
    updateUI();
    emit colorChanged(color);
}

void ColorPicker::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();
    QVBoxLayout *sliderLayout = new QVBoxLayout();

    // Setup sliders
    for (QSlider* slider : {m_redSlider, m_greenSlider, m_blueSlider, m_alphaSlider}) {
        slider->setRange(0, 255);
    }

    for (QSpinBox* spin : {m_redSpinBox, m_greenSpinBox, m_blueSpinBox, m_alphaSpinBox}) {
        spin->setRange(0, 255);
    }

    // Connect RGB
    connect(m_redSlider, &QSlider::valueChanged, this, &ColorPicker::onRedChanged);
    connect(m_greenSlider, &QSlider::valueChanged, this, &ColorPicker::onGreenChanged);
    connect(m_blueSlider, &QSlider::valueChanged, this, &ColorPicker::onBlueChanged);
    connect(m_alphaSlider, &QSlider::valueChanged, this, &ColorPicker::onAlphaChanged);

    connect(m_redSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::onRedChanged);
    connect(m_greenSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::onGreenChanged);
    connect(m_blueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::onBlueChanged);
    connect(m_alphaSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::onAlphaChanged);

    connect(m_colorWheel, &ColorWheel::colorChanged, this, &ColorPicker::onColorWheelChanged);
    connect(m_hexLineEdit, &QLineEdit::editingFinished, this, &ColorPicker::onHexChanged);

    auto addSliderRow = [&](const QString &name, QSlider *slider, QSpinBox *spin) {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(new QLabel(name));
        layout->addWidget(slider);
        layout->addWidget(spin);
        sliderLayout->addLayout(layout);
    };

    addSliderRow("R", m_redSlider, m_redSpinBox);
    addSliderRow("G", m_greenSlider, m_greenSpinBox);
    addSliderRow("B", m_blueSlider, m_blueSpinBox);
    addSliderRow("A", m_alphaSlider, m_alphaSpinBox);

    // Hex
    QHBoxLayout *hexLayout = new QHBoxLayout();
    hexLayout->addWidget(new QLabel("Hex:"));
    hexLayout->addWidget(m_hexLineEdit);
    sliderLayout->addLayout(hexLayout);

    // Preview
    m_colorPreview->setFixedSize(40, 40);
    m_colorPreview->setStyleSheet("border: 1px solid black;");
    sliderLayout->addWidget(m_colorPreview);

    // Final layout
    topLayout->addWidget(m_colorWheel);
    topLayout->addLayout(sliderLayout);

    mainLayout->addLayout(topLayout);
}

void ColorPicker::onColorWheelChanged(const QColor &color) {
    m_currentColor.setHsv(color.hue(), color.saturation(), m_currentColor.value(), m_currentColor.alpha());
    updateUI();
    emit colorChanged(m_currentColor);
}

void ColorPicker::onRedChanged(int value) {
    m_currentColor.setRed(value);
    updateUI();
    emit colorChanged(m_currentColor);
}
void ColorPicker::onGreenChanged(int value) {
    m_currentColor.setGreen(value);
    updateUI();
    emit colorChanged(m_currentColor);
}
void ColorPicker::onBlueChanged(int value) {
    m_currentColor.setBlue(value);
    updateUI();
    emit colorChanged(m_currentColor);
}
void ColorPicker::onAlphaChanged(int value) {
    m_currentColor.setAlpha(value);
    updateUI();
    emit colorChanged(m_currentColor);
}

void ColorPicker::onHexChanged() {
    QColor color(m_hexLineEdit->text());
    if (color.isValid()) {
        m_currentColor = color;
        updateUI();
        emit colorChanged(m_currentColor);
    }
}

void ColorPicker::updateUI() {
    updateRGBSliders();
    updateHexInput();

    m_colorPreview->setStyleSheet(QString("background-color: %1; border: 1px solid black;").arg(m_currentColor.name(QColor::HexArgb)));
}

void ColorPicker::updateRGBSliders() {
    m_redSlider->blockSignals(true); m_redSpinBox->blockSignals(true);
    m_greenSlider->blockSignals(true); m_greenSpinBox->blockSignals(true);
    m_blueSlider->blockSignals(true); m_blueSpinBox->blockSignals(true);
    m_alphaSlider->blockSignals(true); m_alphaSpinBox->blockSignals(true);

    m_redSlider->setValue(m_currentColor.red());
    m_greenSlider->setValue(m_currentColor.green());
    m_blueSlider->setValue(m_currentColor.blue());
    m_alphaSlider->setValue(m_currentColor.alpha());

    m_redSpinBox->setValue(m_currentColor.red());
    m_greenSpinBox->setValue(m_currentColor.green());
    m_blueSpinBox->setValue(m_currentColor.blue());
    m_alphaSpinBox->setValue(m_currentColor.alpha());

    m_redSlider->blockSignals(false); m_redSpinBox->blockSignals(false);
    m_greenSlider->blockSignals(false); m_greenSpinBox->blockSignals(false);
    m_blueSlider->blockSignals(false); m_blueSpinBox->blockSignals(false);
    m_alphaSlider->blockSignals(false); m_alphaSpinBox->blockSignals(false);
}

void ColorPicker::updateHexInput() {
    m_hexLineEdit->setText(m_currentColor.name(QColor::HexArgb));
}
