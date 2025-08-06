#include "document.h"

Layer::Layer(const QString &name)
    : QObject(), m_name(name), m_visible(true), m_locked(false) {}

Layer::~Layer() {
    clear();
}

void Layer::addShape(Shape *shape) {
    if (shape) {
        m_shapes.append(shape);
    }
}

void Layer::removeShape(Shape *shape) {
    if (shape) {
        m_shapes.removeOne(shape);
    }
}

void Layer::clear() {
    qDeleteAll(m_shapes);
    m_shapes.clear();
}

QList<Shape*> Layer::getShapes() const {
    return m_shapes;
}

QString Layer::getName() const {
    return m_name;
}

void Layer::setName(const QString &name) {
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

// =============================
// Document Implementation
// =============================

Document::Document()
    : QObject(), m_size(800, 600), m_backgroundColor(Qt::white) {
    m_activeLayer = new Layer("Default Layer");
    m_layers.append(m_activeLayer);
}

Document::~Document() {
    clear();
}

void Document::addLayer(Layer *layer) {
    if (layer) {
        m_layers.append(layer);
        emit layerAdded(layer);
    }
}

void Document::removeLayer(Layer *layer) {
    if (layer && m_layers.contains(layer)) {
        m_layers.removeOne(layer);
        if (m_activeLayer == layer) {
            m_activeLayer = m_layers.isEmpty() ? nullptr : m_layers.last();
        }
        delete layer;
        emit layerRemoved(layer);
    }
}

void Document::clear() {
    qDeleteAll(m_layers);
    m_layers.clear();
    m_activeLayer = nullptr;
    m_undoStack.clear();
    m_redoStack.clear();
}

QList<Layer*> Document::getLayers() const {
    return m_layers;
}

Layer* Document::getActiveLayer() const {
    return m_activeLayer;
}

void Document::setActiveLayer(Layer *layer) {
    if (layer && m_layers.contains(layer)) {
        m_activeLayer = layer;
    }
}

void Document::addShape(Shape *shape) {
    if (shape && m_activeLayer) {
        m_activeLayer->addShape(shape);
        m_undoStack.append({Command::AddShape, shape, m_activeLayer});
        m_redoStack.clear();
        emit shapeAdded(shape);
        emit documentChanged();
    }
}

void Document::removeShape(Shape *shape) {
    if (shape) {
        for (auto layer : m_layers) {
            if (layer->getShapes().contains(shape)) {
                layer->removeShape(shape);
                m_undoStack.append({Command::RemoveShape, shape, layer});
                m_redoStack.clear();
                emit shapeRemoved(shape);
                emit documentChanged();
                break;
            }
        }
    }
}

Shape* Document::getShapeAt(const QPointF &point) const {
    for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
        if ((*it)->isVisible()) {
            const QList<Shape*> shapes = (*it)->getShapes();
            for (auto shapeIt = shapes.rbegin(); shapeIt != shapes.rend(); ++shapeIt) {
                if (*shapeIt && (*shapeIt)->isVisible() && (*shapeIt)->contains(point)) {
                    return *shapeIt;
                }
            }
        }
    }
    return nullptr;
}

QList<Shape*> Document::getShapes() const {
    // ✅ Return shapes only from the active layer
    return (m_activeLayer && m_activeLayer->isVisible())
        ? m_activeLayer->getShapes()
        : QList<Shape*>();
}

QList<Shape*> Document::getAllShapes() const {
    QList<Shape*> allShapes;
    for (auto layer : m_layers) {
        if (layer->isVisible()) {
            allShapes.append(layer->getShapes());
        }
    }
    return allShapes;
}

void Document::setSize(const QSizeF &size) {
    m_size = size;
    emit documentChanged();
}

QSizeF Document::getSize() const {
    return m_size;
}

void Document::setBackgroundColor(const QColor &color) {
    m_backgroundColor = color;
    emit documentChanged();
}

QColor Document::getBackgroundColor() const {
    return m_backgroundColor;
}

bool Document::save(const QString &filename) {
    // Placeholder for file saving logic
    return false;
}

bool Document::load(const QString &filename) {
    // Placeholder for file loading logic
    return false;
}

void Document::clearHistory() {
    m_undoStack.clear();
    m_redoStack.clear();
}

bool Document::canUndo() const {
    return !m_undoStack.isEmpty();
}

bool Document::canRedo() const {
    return !m_redoStack.isEmpty();
}

void Document::undo() {
    if (!m_undoStack.isEmpty()) {
        Command cmd = m_undoStack.takeLast();
        if (cmd.type == Command::AddShape) {
            cmd.layer->removeShape(cmd.shape);
            m_redoStack.append(cmd);
            emit shapeRemoved(cmd.shape);
        } else if (cmd.type == Command::RemoveShape) {
            cmd.layer->addShape(cmd.shape);
            m_redoStack.append(cmd);
            emit shapeAdded(cmd.shape);
        }
        emit documentChanged();
    }
}

void Document::redo() {
    if (!m_redoStack.isEmpty()) {
        Command cmd = m_redoStack.takeLast();
        if (cmd.type == Command::AddShape) {
            cmd.layer->addShape(cmd.shape);
            m_undoStack.append(cmd);
            emit shapeAdded(cmd.shape);
        } else if (cmd.type == Command::RemoveShape) {
            cmd.layer->removeShape(cmd.shape);
            m_undoStack.append(cmd);
            emit shapeRemoved(cmd.shape);
        }
        emit documentChanged();
    }
}
