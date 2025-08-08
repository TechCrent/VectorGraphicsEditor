#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSizeF>
#include <QColor>
#include "shape.h"

// === LAYER CLASS ===
class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(const QString &name = "Layer");
    ~Layer();

    void addShape(Shape *shape);
    void removeShape(Shape *shape);
    void clear();
    QList<Shape*> getShapes() const;

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

// === DOCUMENT CLASS ===
class Document : public QObject
{
    Q_OBJECT
public:
    Document();
    ~Document();

    // Layer management
    void setActiveLayer(Layer *layer);
    void setActiveLayer(const QString &id);         // 🔄 Optional overload for setting by name
    Layer* getActiveLayer() const;                  // ✅ Already implemented
    void addLayer(const QString& name = "Layer");
	void addLayer(Layer* layer);
    void removeLayer(int index);
	void removeLayer(Layer* layer);
    void moveLayerUp(int index);
    void moveLayerDown(int index);
    void setCurrentLayer(int index);
    int currentLayerIndex() const;
    Layer* currentLayer() const;
    QList<Layer*> getLayers() const;
    void renameLayer(const QString& oldId, const QString& newName); // 🔄 Optional

    // Shape management
    void addShape(Shape *shape);
    void removeShape(Shape *shape);
    Shape* getShapeAt(const QPointF &point) const;
    QList<Shape*> getShapes() const;
    QList<Shape*> getAllShapes() const;

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

    void clear(); // 🔄 Moved into class, not outside

signals:
    void documentChanged();
    void layerAdded(Layer *layer);
    void layerRemoved(Layer *layer);
    void shapeAdded(Shape *shape);
    void shapeRemoved(Shape *shape);

private:
    QList<Layer*> m_layers;
    int m_currentLayerIndex;
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
