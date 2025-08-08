#include "layer.h"
#include "shape.h"

Layer::Layer(const QString& name)
    : m_name(name), m_visible(true), m_locked(false)
{}

QString Layer::name() const {
    return m_name;
}

void Layer::setName(const QString& name) {
    m_name = name;
}

bool Layer::isVisible() const {
    return m_visible;
}

void Layer::setVisible(bool visible) {
    m_visible = visible;
}

bool Layer::isLocked() const {
    return m_locked;
}

void Layer::setLocked(bool locked) {
    m_locked = locked;
}

void Layer::addShape(Shape* shape) {
    if (!m_shapes.contains(shape))
        m_shapes.append(shape);
}

void Layer::removeShape(Shape* shape) {
    m_shapes.removeAll(shape);
}

QList<Shape*> Layer::shapes() const {
    return m_shapes;
}
