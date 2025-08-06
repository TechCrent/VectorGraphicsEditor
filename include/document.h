#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSizeF>
#include <QColor>
#include "shape.h"

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(const QString &name = "Layer");
    ~Layer();

    // Shape management
    void addShape(Shape *shape);
    void removeShape(Shape *shape);
    void clear();
    QList<Shape*> getShapes() const;   // ✅ Required for canvas.cpp

    // Layer properties
    QString getName() const;
    void setName(const QString &name);
    bool isVisible() const;
    void setVisible(bool visible);
    bool isLocked() const;
    void setLocked(bool locked);

private:
    QString m_name;
    QList<Shape*> m_shapes;
    bool m_visible;
    bool m_locked;
};

class Document : public QObject
{
    Q_OBJECT
public:
    Document();
    ~Document();

    // Layer management
    void addLayer(Layer *layer);
    void removeLayer(Layer *layer);
    void clear();
    QList<Layer*> getLayers() const;
    Layer* getActiveLayer() const;
    void setActiveLayer(Layer *layer);

    // Shape management
    void addShape(Shape *shape);
    void removeShape(Shape *shape);
    Shape* getShapeAt(const QPointF &point) const;
    QList<Shape*> getShapes() const;        // ✅ Added: Needed for canvas.cpp
    QList<Shape*> getAllShapes() const;     // ✅ Already present, but retained for global access

    // Document properties
    void setSize(const QSizeF &size);
    QSizeF getSize() const;
    void setBackgroundColor(const QColor &color);
    QColor getBackgroundColor() const;

    // File operations
    bool save(const QString &filename);
    bool load(const QString &filename);

    // Undo/Redo
    void clearHistory();
    bool canUndo() const;
    bool canRedo() const;
    void undo();
    void redo();

signals:
    void documentChanged();
    void layerAdded(Layer *layer);
    void layerRemoved(Layer *layer);
    void shapeAdded(Shape *shape);
    void shapeRemoved(Shape *shape);

private:
    QList<Layer*> m_layers;
    Layer *m_activeLayer;
    QSizeF m_size;
    QColor m_backgroundColor;

    struct Command {
        enum Type { AddShape, RemoveShape, ModifyShape };
        Type type;
        Shape *shape;
        Layer *layer;
    };

    QList<Command> m_undoStack;
    QList<Command> m_redoStack;
};

#endif // DOCUMENT_H
