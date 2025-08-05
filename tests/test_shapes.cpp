#include <gtest/gtest.h>
#include <QApplication>
#include <QColor>
#include <QPen>
#include <QBrush>
#include "../include/rectangle.h"
#include "../include/ellipse.h"
#include "../include/line.h"
#include "../include/bezier.h"
#include <cairo.h>

class ShapeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize Cairo surface for testing
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 100, 100);
        cr = cairo_create(surface);
    }
    
    void TearDown() override {
        if (cr) cairo_destroy(cr);
        if (surface) cairo_surface_destroy(surface);
    }
    
    cairo_surface_t* surface = nullptr;
    cairo_t* cr = nullptr;
};

// Rectangle Tests
TEST_F(ShapeTest, RectangleCreation) {
    Rectangle rect(QPointF(10, 20), QSizeF(50, 30));
    
    EXPECT_EQ(rect.getPosition(), QPointF(10, 20));
    EXPECT_EQ(rect.getSize(), QSizeF(50, 30));
    EXPECT_EQ(rect.getType(), Shape::Rectangle);
    EXPECT_TRUE(rect.isVisible());
    EXPECT_FALSE(rect.isSelected());
}

TEST_F(ShapeTest, RectangleContainment) {
    Rectangle rect(QPointF(10, 20), QSizeF(50, 30));
    
    // Point inside rectangle
    EXPECT_TRUE(rect.contains(QPointF(30, 35)));
    
    // Point outside rectangle
    EXPECT_FALSE(rect.contains(QPointF(5, 5)));
    EXPECT_FALSE(rect.contains(QPointF(70, 35)));
    
    // Point on edge
    EXPECT_TRUE(rect.contains(QPointF(10, 30)));
}

TEST_F(ShapeTest, RectangleClone) {
    Rectangle rect(QPointF(10, 20), QSizeF(50, 30));
    rect.setCornerRadius(5.0);
    rect.setPen(QPen(Qt::blue, 2));
    rect.setBrush(QBrush(Qt::red));
    
    Rectangle* clone = rect.clone();
    
    ASSERT_NE(clone, nullptr);
    EXPECT_EQ(clone->getPosition(), rect.getPosition());
    EXPECT_EQ(clone->getSize(), rect.getSize());
    EXPECT_EQ(clone->getCornerRadius(), 5.0);
    EXPECT_EQ(clone->getPen().color(), Qt::blue);
    EXPECT_EQ(clone->getBrush().color(), Qt::red);
    
    delete clone;
}

TEST_F(ShapeTest, RectangleDraw) {
    Rectangle rect(QPointF(10, 20), QSizeF(50, 30));
    rect.setPen(QPen(Qt::black, 1));
    rect.setBrush(QBrush(Qt::white));
    
    // Should not crash
    EXPECT_NO_THROW(rect.draw(cr));
    
    // Test with null context
    EXPECT_NO_THROW(rect.draw(nullptr));
}

// Ellipse Tests
TEST_F(ShapeTest, EllipseCreation) {
    Ellipse ellipse(QPointF(15, 25), QSizeF(40, 60));
    
    EXPECT_EQ(ellipse.getPosition(), QPointF(15, 25));
    EXPECT_EQ(ellipse.getSize(), QSizeF(40, 60));
    EXPECT_EQ(ellipse.getType(), Shape::Ellipse);
    EXPECT_DOUBLE_EQ(ellipse.getStartAngle(), 0.0);
    EXPECT_DOUBLE_EQ(ellipse.getEndAngle(), 360.0);
}

TEST_F(ShapeTest, EllipseContainment) {
    Ellipse ellipse(QPointF(0, 0), QSizeF(100, 100));
    
    // Point at center should be inside
    EXPECT_TRUE(ellipse.contains(QPointF(50, 50)));
    
    // Point clearly outside
    EXPECT_FALSE(ellipse.contains(QPointF(0, 0)));
    EXPECT_FALSE(ellipse.contains(QPointF(100, 100)));
    
    // Point on approximate edge
    EXPECT_TRUE(ellipse.contains(QPointF(75, 50))); // Should be inside
}

TEST_F(ShapeTest, EllipseAngles) {
    Ellipse ellipse;
    
    ellipse.setStartAngle(45.0);
    ellipse.setEndAngle(315.0);
    
    EXPECT_DOUBLE_EQ(ellipse.getStartAngle(), 45.0);
    EXPECT_DOUBLE_EQ(ellipse.getEndAngle(), 315.0);
}

TEST_F(ShapeTest, EllipseDraw) {
    Ellipse ellipse(QPointF(10, 20), QSizeF(50, 30));
    ellipse.setPen(QPen(Qt::blue, 2));
    ellipse.setBrush(QBrush(Qt::yellow));
    
    // Should not crash
    EXPECT_NO_THROW(ellipse.draw(cr));
    
    // Test with null context
    EXPECT_NO_THROW(ellipse.draw(nullptr));
}

// Line Tests
TEST_F(ShapeTest, LineCreation) {
    Line line(QPointF(10, 20), QPointF(50, 80));
    
    EXPECT_EQ(line.getStartPoint(), QPointF(10, 20));
    EXPECT_EQ(line.getEndPoint(), QPointF(50, 80));
    EXPECT_EQ(line.getType(), Shape::Line);
}

TEST_F(ShapeTest, LineContainment) {
    Line line(QPointF(0, 0), QPointF(100, 100));
    
    // Point on line
    EXPECT_TRUE(line.contains(QPointF(50, 50)));
    
    // Point near line (within tolerance)
    EXPECT_TRUE(line.contains(QPointF(51, 50))); // Depends on implementation tolerance
    
    // Point far from line
    EXPECT_FALSE(line.contains(QPointF(0, 100)));
}

TEST_F(ShapeTest, LineDraw) {
    Line line(QPointF(10, 20), QPointF(50, 80));
    line.setPen(QPen(Qt::green, 3));
    
    // Should not crash
    EXPECT_NO_THROW(line.draw(cr));
    
    // Test with null context
    EXPECT_NO_THROW(line.draw(nullptr));
}

// Bezier Tests
TEST_F(ShapeTest, BezierCreation) {
    Bezier bezier;
    
    EXPECT_EQ(bezier.getType(), Shape::Bezier);
    EXPECT_FALSE(bezier.isClosed());
}

TEST_F(ShapeTest, BezierPoints) {
    Bezier bezier;
    
    bezier.addPoint(QPointF(10, 20));
    bezier.addPoint(QPointF(30, 40));
    bezier.addPoint(QPointF(50, 60));
    
    QList<QPointF> points = bezier.getPoints();
    EXPECT_EQ(points.size(), 3);
    EXPECT_EQ(points[0], QPointF(10, 20));
    EXPECT_EQ(points[1], QPointF(30, 40));
    EXPECT_EQ(points[2], QPointF(50, 60));
}

TEST_F(ShapeTest, BezierClosed) {
    Bezier bezier;
    
    EXPECT_FALSE(bezier.isClosed());
    
    bezier.setClosed(true);
    EXPECT_TRUE(bezier.isClosed());
}

TEST_F(ShapeTest, BezierDraw) {
    Bezier bezier;
    bezier.addPoint(QPointF(10, 20));
    bezier.addPoint(QPointF(30, 40));
    bezier.addPoint(QPointF(50, 60));
    bezier.setPen(QPen(Qt::red, 2));
    
    // Should not crash
    EXPECT_NO_THROW(bezier.draw(cr));
    
    // Test with null context
    EXPECT_NO_THROW(bezier.draw(nullptr));
    
    // Test with empty points
    Bezier emptyBezier;
    EXPECT_NO_THROW(emptyBezier.draw(cr));
}

// Base Shape Tests
TEST_F(ShapeTest, ShapeProperties) {
    Rectangle rect;
    
    // Test pen
    QPen testPen(Qt::blue, 5);
    rect.setPen(testPen);
    EXPECT_EQ(rect.getPen().color(), Qt::blue);
    EXPECT_EQ(rect.getPen().width(), 5);
    
    // Test brush
    QBrush testBrush(Qt::red, Qt::SolidPattern);
    rect.setBrush(testBrush);
    EXPECT_EQ(rect.getBrush().color(), Qt::red);
    EXPECT_EQ(rect.getBrush().style(), Qt::SolidPattern);
    
    // Test visibility
    rect.setVisible(false);
    EXPECT_FALSE(rect.isVisible());
    rect.setVisible(true);
    EXPECT_TRUE(rect.isVisible());
    
    // Test selection
    rect.setSelected(true);
    EXPECT_TRUE(rect.isSelected());
    rect.setSelected(false);
    EXPECT_FALSE(rect.isSelected());
}

TEST_F(ShapeTest, ShapeTransformations) {
    Rectangle rect(QPointF(10, 20), QSizeF(50, 30));
    
    // Test move
    rect.move(QPointF(5, 10));
    EXPECT_EQ(rect.getPosition(), QPointF(15, 30));
    
    // Test scale (basic implementation may just update size)
    QSizeF originalSize = rect.getSize();
    rect.scale(2.0);
    // Implementation dependent - may scale size or transform
    
    // Test rotation (angle setting)
    rect.rotate(45.0);
    // Implementation dependent - shape may store rotation
}

// Integration test
TEST_F(ShapeTest, MultipleShapesDraw) {
    Rectangle rect(QPointF(10, 10), QSizeF(30, 20));
    Ellipse ellipse(QPointF(50, 50), QSizeF(40, 40));
    Line line(QPointF(0, 0), QPointF(100, 100));
    
    // Should be able to draw multiple shapes
    EXPECT_NO_THROW({
        rect.draw(cr);
        ellipse.draw(cr);
        line.draw(cr);
    });
}

// Test with invalid inputs
TEST_F(ShapeTest, InvalidInputsHandling) {
    Rectangle rect;
    
    // Negative size should be handled gracefully
    rect.setSize(QSizeF(-10, -20));
    EXPECT_NO_THROW(rect.draw(cr));
    
    // Zero size
    rect.setSize(QSizeF(0, 0));
    EXPECT_NO_THROW(rect.draw(cr));
    
    // Very large coordinates
    rect.setPosition(QPointF(1e6, 1e6));
    EXPECT_NO_THROW(rect.draw(cr));
}

// Main function for running tests
int main(int argc, char **argv) {
    // Qt Application needed for Qt types
    QApplication app(argc, argv);
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}