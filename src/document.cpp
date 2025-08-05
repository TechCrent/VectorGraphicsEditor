#include "document.h"
#include <QDebug>

Layer::Layer(const QString &name)
    : m_name(name)
    , m_visible(true)
    , m_locked(false)
{
}

Layer::~Layer()
{
    qDeleteAll(m_shapes);
}

void Layer::addShape(Shape *shape)
{
    if (shape) {
        m_shapes.append(shape);
    }
}

void Layer::removeShape(Shape *shape)
{
    m_shapes.removeOne(shape);
    delete shape;
}

void Layer::clear()
{
    qDeleteAll(m_shapes);
    m_shapes.clear();
}

QList<Shape*> Layer::getShapes() const
{
    return m_shapes;
}

QString Layer::getName() const
{
    return m_name;
}

void Layer::setName(const QString &name)
{
    m_name = name;
}

bool Layer::isVisible() const
{
    return m_visible;
}

void Layer::setVisible(bool visible)
{
    m_visible = visible;
}

bool Layer::isLocked() const
{
    return m_locked;
}

void Layer::setLocked(bool locked)
{
    m_locked = locked;
}

// Document implementation
Document::Document()
    : m_activeLayer(nullptr)
    , m_size(800, 600)
    , m_backgroundColor(Qt::white)
{
    // Create default layer
    Layer *defaultLayer = new Layer("Layer 1");
    addLayer(defaultLayer);
    setActiveLayer(defaultLayer);
}

Document::~Document()
{
    qDeleteAll(m_layers);
}

void Document::addLayer(Layer *layer)
{
    if (layer) {
        m_layers.append(layer);
        if (!m_activeLayer) {
            setActiveLayer(layer);
        }
        emit layerAdded(layer);
        emit documentChanged();
    }
}

void Document::removeLayer(Layer *layer)
{
    if (layer && m_layers.contains(layer)) {
        m_layers.removeOne(layer);
        
        if (m_activeLayer == layer) {
            setActiveLayer(m_layers.isEmpty() ? nullptr : m_layers.first());
        }
        
        emit layerRemoved(layer);
        emit documentChanged();
        delete layer;
    }
}

void Document::clear()
{
    qDeleteAll(m_layers);
    m_layers.clear();
    m_activeLayer = nullptr;
    m_undoStack.clear();
    m_redoStack.clear();
    emit documentChanged();
}

QList<Layer*> Document::getLayers() const
{
    return m_layers;
}

Layer* Document::getActiveLayer() const
{
    return m_activeLayer;
}

void Document::setActiveLayer(Layer *layer)
{
    if (m_layers.contains(layer)) {
        m_activeLayer = layer;
        emit documentChanged();
    }
}

void Document::addShape(Shape *shape)
{
    if (shape && m_activeLayer) {
        m_activeLayer->addShape(shape);
        
        // Add to undo stack
        Command cmd;
        cmd.type = Command::AddShape;
        cmd.shape = shape;
        cmd.layer = m_activeLayer;
        m_undoStack.append(cmd);
        m_redoStack.clear();
        
        emit shapeAdded(shape);
        emit documentChanged();
    }
}

void Document::removeShape(Shape *shape)
{
    if (shape) {
        // Find which layer contains this shape
        for (Layer *layer : m_layers) {
            if (layer->getShapes().contains(shape)) {
                layer->removeShape(shape);
                
                // Add to undo stack
                Command cmd;
                cmd.type = Command::RemoveShape;
                cmd.shape = shape;
                cmd.layer = layer;
                m_undoStack.append(cmd);
                m_redoStack.clear();
                
                emit shapeRemoved(shape);
                emit documentChanged();
                break;
            }
        }
    }
}

Shape* Document::getShapeAt(const QPointF &point) const
{
    // Search from top to bottom (reverse order of layers)
    for (int i = m_layers.size() - 1; i >= 0; --i) {
        Layer *layer = m_layers[i];
        if (!layer->isVisible() || layer->isLocked()) continue;
        
        for (Shape *shape : layer->getShapes()) {
            if (shape->isVisible() && shape->contains(point)) {
                return shape;
            }
        }
    }
    return nullptr;
}

void Document::setSize(const QSizeF &size)
{
    m_size = size;
    emit documentChanged();
}

QSizeF Document::getSize() const
{
    return m_size;
}

void Document::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    emit documentChanged();
}

QColor Document::getBackgroundColor() const
{
    return m_backgroundColor;
}

bool Document::save(const QString &filename)
{
    // TODO: Implement save functionality
    Q_UNUSED(filename)
    return false;
}

bool Document::load(const QString &filename)
{
    // TODO: Implement load functionality
    Q_UNUSED(filename)
    return false;
}

void Document::clearHistory()
{
    m_undoStack.clear();
    m_redoStack.clear();
}

bool Document::canUndo() const
{
    return !m_undoStack.isEmpty();
}

bool Document::canRedo() const
{
    return !m_redoStack.isEmpty();
}

void Document::undo()
{
    if (!m_undoStack.isEmpty()) {
        Command cmd = m_undoStack.takeLast();
        
        switch (cmd.type) {
            case Command::AddShape:
                // Remove the shape
                if (cmd.layer->getShapes().contains(cmd.shape)) {
                    cmd.layer->removeShape(cmd.shape);
                    emit shapeRemoved(cmd.shape);
                }
                break;
                
            case Command::RemoveShape:
                // Add the shape back
                cmd.layer->addShape(cmd.shape);
                emit shapeAdded(cmd.shape);
                break;
                
            case Command::ModifyShape:
                // TODO: Implement shape modification undo
                break;
        }
        
        m_redoStack.append(cmd);
        emit documentChanged();
    }
}

void Document::redo()
{
    if (!m_redoStack.isEmpty()) {
        Command cmd = m_redoStack.takeLast();
        
        switch (cmd.type) {
            case Command::AddShape:
                // Add the shape back
                cmd.layer->addShape(cmd.shape);
                emit shapeAdded(cmd.shape);
                break;
                
            case Command::RemoveShape:
                // Remove the shape again
                if (cmd.layer->getShapes().contains(cmd.shape)) {
                    cmd.layer->removeShape(cmd.shape);
                    emit shapeRemoved(cmd.shape);
                }
                break;
                
            case Command::ModifyShape:
                // TODO: Implement shape modification redo
                break;
        }
        
        m_undoStack.append(cmd);
        emit documentChanged();
    }
} 