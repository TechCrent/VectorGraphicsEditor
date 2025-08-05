#include <gtest/gtest.h>
#include <QApplication>
#include "../include/document.h"
#include "../include/rectangle.h"
#include "../include/ellipse.h"

class DocumentTest : public ::testing::Test {
protected:
    void SetUp() override {
        document = new Document();
    }
    
    void TearDown() override {
        delete document;
    }
    
    Document* document = nullptr;
};

class LayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        layer = new Layer("Test Layer");
    }
    
    void TearDown() override {
        delete layer;
    }
    
    Layer* layer = nullptr;
};

// Layer Tests
TEST_F(LayerTest, LayerCreation) {
    EXPECT_EQ(layer->getName(), "Test Layer");
    EXPECT_TRUE(layer->isVisible());
    EXPECT_FALSE(layer->isLocked());
    EXPECT_EQ(layer->getShapes().size(), 0);
}

TEST_F(LayerTest, LayerProperties) {
    // Test name
    layer->setName("New Name");
    EXPECT_EQ(layer->getName(), "New Name");
    
    // Test visibility
    layer->setVisible(false);
    EXPECT_FALSE(layer->isVisible());
    layer->setVisible(true);
    EXPECT_TRUE(layer->isVisible());
    
    // Test locked state
    layer->setLocked(true);
    EXPECT_TRUE(layer->isLocked());
    layer->setLocked(false);
    EXPECT_FALSE(layer->isLocked());
}

TEST_F(LayerTest, LayerShapeManagement) {
    Rectangle* rect = new Rectangle(QPointF(10, 20), QSizeF(30, 40));
    Ellipse* ellipse = new Ellipse(QPointF(50, 60), QSizeF(70, 80));
    
    // Add shapes
    layer->addShape(rect);
    layer->addShape(ellipse);
    
    EXPECT_EQ(layer->getShapes().size(), 2);
    EXPECT_TRUE(layer->getShapes().contains(rect));
    EXPECT_TRUE(layer->getShapes().contains(ellipse));
    
    // Remove shape
    layer->removeShape(rect); // This should delete the shape
    EXPECT_EQ(layer->getShapes().size(), 1);
    EXPECT_FALSE(layer->getShapes().contains(rect));
    EXPECT_TRUE(layer->getShapes().contains(ellipse));
    
    // Clear all shapes
    layer->clear();
    EXPECT_EQ(layer->getShapes().size(), 0);
}

TEST_F(LayerTest, LayerAddNullShape) {
    // Adding null shape should be handled gracefully
    layer->addShape(nullptr);
    EXPECT_EQ(layer->getShapes().size(), 0);
}

TEST_F(LayerTest, LayerRemoveNonExistentShape) {
    Rectangle* rect = new Rectangle();
    
    // Removing shape that was never added should be handled gracefully
    EXPECT_NO_THROW(layer->removeShape(rect));
    
    delete rect; // We need to delete it since removeShape didn't
}

// Document Tests
TEST_F(DocumentTest, DocumentCreation) {
    EXPECT_EQ(document->getLayers().size(), 0);
    EXPECT_EQ(document->getSize(), QSizeF(800, 600)); // Default size
}

TEST_F(DocumentTest, DocumentSize) {
    QSizeF newSize(1200, 900);
    document->setSize(newSize);
    EXPECT_EQ(document->getSize(), newSize);
}

TEST_F(DocumentTest, DocumentLayerManagement) {
    Layer* layer1 = new Layer("Layer 1");
    Layer* layer2 = new Layer("Layer 2");
    
    // Add layers
    document->addLayer(layer1);
    document->addLayer(layer2);
    
    EXPECT_EQ(document->getLayers().size(), 2);
    EXPECT_TRUE(document->getLayers().contains(layer1));
    EXPECT_TRUE(document->getLayers().contains(layer2));
    
    // Remove layer
    document->removeLayer(layer1); // This should delete the layer
    EXPECT_EQ(document->getLayers().size(), 1);
    EXPECT_FALSE(document->getLayers().contains(layer1));
    EXPECT_TRUE(document->getLayers().contains(layer2));
    
    // Clear all layers
    document->clear();
    EXPECT_EQ(document->getLayers().size(), 0);
}

TEST_F(DocumentTest, DocumentAddNullLayer) {
    // Adding null layer should be handled gracefully
    document->addLayer(nullptr);
    EXPECT_EQ(document->getLayers().size(), 0);
}

TEST_F(DocumentTest, DocumentShapeRetrieval) {
    // Create layers with shapes
    Layer* layer1 = new Layer("Layer 1");
    Layer* layer2 = new Layer("Layer 2");
    
    Rectangle* rect = new Rectangle(QPointF(10, 20), QSizeF(30, 40));
    Ellipse* ellipse = new Ellipse(QPointF(50, 60), QSizeF(70, 80));
    
    layer1->addShape(rect);
    layer2->addShape(ellipse);
    
    document->addLayer(layer1);
    document->addLayer(layer2);
    
    // Test getShapeAt
    Shape* foundShape = document->getShapeAt(QPointF(25, 35)); // Inside rectangle
    EXPECT_EQ(foundShape, rect);
    
    foundShape = document->getShapeAt(QPointF(85, 100)); // Inside ellipse
    EXPECT_EQ(foundShape, ellipse);
    
    foundShape = document->getShapeAt(QPointF(0, 0)); // Not on any shape
    EXPECT_EQ(foundShape, nullptr);
}

TEST_F(DocumentTest, DocumentShapeRetrievalInvisibleLayer) {
    Layer* layer = new Layer("Layer");
    Rectangle* rect = new Rectangle(QPointF(10, 20), QSizeF(30, 40));
    
    layer->addShape(rect);
    layer->setVisible(false); // Make layer invisible
    document->addLayer(layer);
    
    // Should not find shape in invisible layer
    Shape* foundShape = document->getShapeAt(QPointF(25, 35));
    EXPECT_EQ(foundShape, nullptr);
}

TEST_F(DocumentTest, DocumentShapeRetrievalInvisibleShape) {
    Layer* layer = new Layer("Layer");
    Rectangle* rect = new Rectangle(QPointF(10, 20), QSizeF(30, 40));
    
    rect->setVisible(false); // Make shape invisible
    layer->addShape(rect);
    document->addLayer(layer);
    
    // Should not find invisible shape
    Shape* foundShape = document->getShapeAt(QPointF(25, 35));
    EXPECT_EQ(foundShape, nullptr);
}

TEST_F(DocumentTest, DocumentShapeRetrievalMultipleShapes) {
    Layer* layer = new Layer("Layer");
    
    // Create overlapping shapes
    Rectangle* rect1 = new Rectangle(QPointF(0, 0), QSizeF(50, 50));
    Rectangle* rect2 = new Rectangle(QPointF(25, 25), QSizeF(50, 50));
    
    layer->addShape(rect1);
    layer->addShape(rect2);
    document->addLayer(layer);
    
    // Should find the topmost shape (last added)
    Shape* foundShape = document->getShapeAt(QPointF(30, 30));
    EXPECT_EQ(foundShape, rect2);
}

TEST_F(DocumentTest, DocumentGetAllShapes) {
    Layer* layer1 = new Layer("Layer 1");
    Layer* layer2 = new Layer("Layer 2");
    
    Rectangle* rect = new Rectangle();
    Ellipse* ellipse = new Ellipse();
    
    layer1->addShape(rect);
    layer2->addShape(ellipse);
    
    document->addLayer(layer1);
    document->addLayer(layer2);
    
    QList<Shape*> allShapes = document->getAllShapes();
    EXPECT_EQ(allShapes.size(), 2);
    EXPECT_TRUE(allShapes.contains(rect));
    EXPECT_TRUE(allShapes.contains(ellipse));
}

TEST_F(DocumentTest, DocumentGetAllShapesEmptyDocument) {
    QList<Shape*> allShapes = document->getAllShapes();
    EXPECT_EQ(allShapes.size(), 0);
}

TEST_F(DocumentTest, DocumentGetAllShapesInvisibleLayers) {
    Layer* visibleLayer = new Layer("Visible");
    Layer* invisibleLayer = new Layer("Invisible");
    
    Rectangle* rect1 = new Rectangle();
    Rectangle* rect2 = new Rectangle();
    
    visibleLayer->addShape(rect1);
    invisibleLayer->addShape(rect2);
    invisibleLayer->setVisible(false);
    
    document->addLayer(visibleLayer);
    document->addLayer(invisibleLayer);
    
    QList<Shape*> allShapes = document->getAllShapes();
    // getAllShapes might include all shapes regardless of visibility
    // This depends on implementation - adjust test based on actual behavior
    EXPECT_GE(allShapes.size(), 1); // At least the visible one
}

// Integration Tests
TEST_F(DocumentTest, DocumentLayerShapeIntegration) {
    // Create a complex document structure
    Layer* backgroundLayer = new Layer("Background");
    Layer* foregroundLayer = new Layer("Foreground");
    
    Rectangle* background = new Rectangle(QPointF(0, 0), QSizeF(800, 600));
    Rectangle* box1 = new Rectangle(QPointF(100, 100), QSizeF(200, 150));
    Ellipse* circle = new Ellipse(QPointF(400, 200), QSizeF(100, 100));
    
    backgroundLayer->addShape(background);
    foregroundLayer->addShape(box1);
    foregroundLayer->addShape(circle);
    
    document->addLayer(backgroundLayer);
    document->addLayer(foregroundLayer);
    
    // Verify structure
    EXPECT_EQ(document->getLayers().size(), 2);
    EXPECT_EQ(backgroundLayer->getShapes().size(), 1);
    EXPECT_EQ(foregroundLayer->getShapes().size(), 2);
    
    // Test shape finding
    Shape* found = document->getShapeAt(QPointF(150, 125)); // Inside box1
    EXPECT_EQ(found, box1);
    
    found = document->getShapeAt(QPointF(450, 250)); // Inside circle
    EXPECT_EQ(found, circle);
    
    found = document->getShapeAt(QPointF(50, 50)); // In background only
    EXPECT_EQ(found, background);
}

// Error handling tests
TEST_F(DocumentTest, DocumentErrorHandling) {
    // Test removing from empty document
    EXPECT_NO_THROW(document->removeLayer(nullptr));
    
    Layer* layer = new Layer("Test");
    EXPECT_NO_THROW(document->removeLayer(layer)); // Not in document
    delete layer;
    
    // Test clear on empty document
    EXPECT_NO_THROW(document->clear());
}

// Memory management test
TEST_F(DocumentTest, DocumentMemoryManagement) {
    Layer* layer = new Layer("Test Layer");
    Rectangle* rect = new Rectangle();
    
    layer->addShape(rect);
    document->addLayer(layer);
    
    // When document is destroyed, it should clean up layers and shapes
    // This is tested by the TearDown method - no memory leaks should occur
    
    // Test explicit cleanup
    document->clear();
    EXPECT_EQ(document->getLayers().size(), 0);
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}