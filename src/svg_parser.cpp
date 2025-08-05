#include "svg_parser.h"
#include "document.h"
#include "rectangle.h"
#include "ellipse.h"
#include "line.h"
#include "bezier.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

SVGParser::SVGParser()
{
}

SVGParser::~SVGParser()
{
}

bool SVGParser::importFromFile(const QString &filename, Document *document)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open SVG file:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    QString svgContent = in.readAll();
    file.close();
    
    return parseSVGString(svgContent, document);
}

bool SVGParser::exportToFile(const QString &filename, Document *document)
{
    QString svgContent = generateSVGString(document);
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to create SVG file:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    out << svgContent;
    file.close();
    
    return true;
}

bool SVGParser::parseSVGString(const QString &svgString, Document *document)
{
    // Simple SVG parsing - this is a basic implementation
    // In a real application, you would use Libxml2 for proper XML parsing
    
    if (!document) return false;
    
    // Clear existing content
    document->clear();
    
    // Parse basic SVG structure
    if (svgString.contains("<svg")) {
        // Extract width and height
        int widthIndex = svgString.indexOf("width=\"");
        int heightIndex = svgString.indexOf("height=\"");
        
        if (widthIndex != -1 && heightIndex != -1) {
            int widthEnd = svgString.indexOf("\"", widthIndex + 8);
            int heightEnd = svgString.indexOf("\"", heightIndex + 9);
            
            if (widthEnd != -1 && heightEnd != -1) {
                QString widthStr = svgString.mid(widthIndex + 8, widthEnd - widthIndex - 8);
                QString heightStr = svgString.mid(heightIndex + 9, heightEnd - heightIndex - 9);
                
                bool ok1, ok2;
                double width = widthStr.toDouble(&ok1);
                double height = heightStr.toDouble(&ok2);
                
                if (ok1 && ok2) {
                    document->setSize(QSizeF(width, height));
                }
            }
        }
        
        // Parse basic shapes (simplified)
        parseBasicShapes(svgString, document);
    }
    
    return true;
}

QString SVGParser::generateSVGString(Document *document)
{
    if (!document) return "";
    
    QString svg;
    QTextStream stream(&svg);
    
    QSizeF size = document->getSize();
    
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
    stream << "width=\"" << size.width() << "\" height=\"" << size.height() << "\">\n";
    
    // Write shapes
    for (Layer *layer : document->getLayers()) {
        if (!layer->isVisible()) continue;
        
        for (Shape *shape : layer->getShapes()) {
            if (shape->isVisible()) {
                writeShape(stream, shape);
            }
        }
    }
    
    stream << "</svg>\n";
    return svg;
}

void SVGParser::parseBasicShapes(const QString &svgString, Document *document)
{
    // Parse rectangles
    int rectIndex = 0;
    while ((rectIndex = svgString.indexOf("<rect", rectIndex)) != -1) {
        int endIndex = svgString.indexOf("/>", rectIndex);
        if (endIndex == -1) break;
        
        QString rectElement = svgString.mid(rectIndex, endIndex - rectIndex + 2);
        parseRectElement(rectElement, document);
        rectIndex = endIndex + 2;
    }
    
    // Parse circles/ellipses
    int circleIndex = 0;
    while ((circleIndex = svgString.indexOf("<circle", circleIndex)) != -1) {
        int endIndex = svgString.indexOf("/>", circleIndex);
        if (endIndex == -1) break;
        
        QString circleElement = svgString.mid(circleIndex, endIndex - circleIndex + 2);
        parseEllipseElement(circleElement, document);
        circleIndex = endIndex + 2;
    }
    
    int ellipseIndex = 0;
    while ((ellipseIndex = svgString.indexOf("<ellipse", ellipseIndex)) != -1) {
        int endIndex = svgString.indexOf("/>", ellipseIndex);
        if (endIndex == -1) break;
        
        QString ellipseElement = svgString.mid(ellipseIndex, endIndex - ellipseIndex + 2);
        parseEllipseElement(ellipseElement, document);
        ellipseIndex = endIndex + 2;
    }
    
    // Parse lines
    int lineIndex = 0;
    while ((lineIndex = svgString.indexOf("<line", lineIndex)) != -1) {
        int endIndex = svgString.indexOf("/>", lineIndex);
        if (endIndex == -1) break;
        
        QString lineElement = svgString.mid(lineIndex, endIndex - lineIndex + 2);
        parseLineElement(lineElement, document);
        lineIndex = endIndex + 2;
    }
}

void SVGParser::parseRectElement(const QString &element, Document *document)
{
    // Extract x, y, width, height attributes
    double x = extractAttribute(element, "x").toDouble();
    double y = extractAttribute(element, "y").toDouble();
    double width = extractAttribute(element, "width").toDouble();
    double height = extractAttribute(element, "height").toDouble();
    
    Rectangle *rect = new Rectangle(QPointF(x, y), QSizeF(width, height));
    
    // Parse style attributes
    QString fill = extractAttribute(element, "fill");
    QString stroke = extractAttribute(element, "stroke");
    QString strokeWidth = extractAttribute(element, "stroke-width");
    
    if (!fill.isEmpty() && fill != "none") {
        rect->setBrush(QBrush(parseColor(fill)));
    }
    if (!stroke.isEmpty() && stroke != "none") {
        QPen pen = rect->getPen();
        pen.setColor(parseColor(stroke));
        if (!strokeWidth.isEmpty()) {
            pen.setWidthF(strokeWidth.toDouble());
        }
        rect->setPen(pen);
    }
    
    document->addShape(rect);
}

void SVGParser::parseEllipseElement(const QString &element, Document *document)
{
    if (element.contains("<circle")) {
        // Parse circle
        double cx = extractAttribute(element, "cx").toDouble();
        double cy = extractAttribute(element, "cy").toDouble();
        double r = extractAttribute(element, "r").toDouble();
        
        Ellipse *ellipse = new Ellipse(QPointF(cx - r, cy - r), QSizeF(r * 2, r * 2));
        parseShapeStyle(element, ellipse);
        document->addShape(ellipse);
    } else {
        // Parse ellipse
        double cx = extractAttribute(element, "cx").toDouble();
        double cy = extractAttribute(element, "cy").toDouble();
        double rx = extractAttribute(element, "rx").toDouble();
        double ry = extractAttribute(element, "ry").toDouble();
        
        Ellipse *ellipse = new Ellipse(QPointF(cx - rx, cy - ry), QSizeF(rx * 2, ry * 2));
        parseShapeStyle(element, ellipse);
        document->addShape(ellipse);
    }
}

void SVGParser::parseLineElement(const QString &element, Document *document)
{
    double x1 = extractAttribute(element, "x1").toDouble();
    double y1 = extractAttribute(element, "y1").toDouble();
    double x2 = extractAttribute(element, "x2").toDouble();
    double y2 = extractAttribute(element, "y2").toDouble();
    
    Line *line = new Line(QPointF(x1, y1), QPointF(x2, y2));
    parseShapeStyle(element, line);
    document->addShape(line);
}

void SVGParser::parseShapeStyle(const QString &element, Shape *shape)
{
    QString fill = extractAttribute(element, "fill");
    QString stroke = extractAttribute(element, "stroke");
    QString strokeWidth = extractAttribute(element, "stroke-width");
    
    if (!fill.isEmpty() && fill != "none") {
        shape->setBrush(QBrush(parseColor(fill)));
    }
    if (!stroke.isEmpty() && stroke != "none") {
        QPen pen = shape->getPen();
        pen.setColor(parseColor(stroke));
        if (!strokeWidth.isEmpty()) {
            pen.setWidthF(strokeWidth.toDouble());
        }
        shape->setPen(pen);
    }
}

QString SVGParser::extractAttribute(const QString &element, const QString &attribute)
{
    QString pattern = attribute + "=\"";
    int startIndex = element.indexOf(pattern);
    if (startIndex == -1) return "";
    
    startIndex += pattern.length();
    int endIndex = element.indexOf("\"", startIndex);
    if (endIndex == -1) return "";
    
    return element.mid(startIndex, endIndex - startIndex);
}

QColor SVGParser::parseColor(const QString &colorString)
{
    if (colorString.startsWith("#")) {
        return QColor(colorString);
    } else if (colorString.startsWith("rgb(")) {
        // Parse rgb(r, g, b) format
        QString rgb = colorString.mid(4, colorString.length() - 5);
        QStringList components = rgb.split(",");
        if (components.size() == 3) {
            return QColor(components[0].trimmed().toInt(),
                         components[1].trimmed().toInt(),
                         components[2].trimmed().toInt());
        }
    } else {
        // Try named colors
        QColor color(colorString);
        if (color.isValid()) return color;
    }
    
    return Qt::black; // Default color
}

void SVGParser::writeShape(QTextStream &stream, Shape *shape)
{
    switch (shape->getType()) {
        case Shape::Rectangle:
            writeRectangle(stream, dynamic_cast<Rectangle*>(shape));
            break;
        case Shape::Ellipse:
            writeEllipse(stream, dynamic_cast<Ellipse*>(shape));
            break;
        case Shape::Line:
            writeLine(stream, dynamic_cast<Line*>(shape));
            break;
        case Shape::Bezier:
            writeBezier(stream, dynamic_cast<Bezier*>(shape));
            break;
    }
}

void SVGParser::writeRectangle(QTextStream &stream, Rectangle *rect)
{
    QPointF pos = rect->getPosition();
    QSizeF size = rect->getSize();
    QPen pen = rect->getPen();
    QBrush brush = rect->getBrush();
    
    stream << "  <rect x=\"" << pos.x() << "\" y=\"" << pos.y() << "\" ";
    stream << "width=\"" << size.width() << "\" height=\"" << size.height() << "\" ";
    
    if (brush.color() != Qt::white) {
        stream << "fill=\"" << colorToString(brush.color()) << "\" ";
    }
    if (pen.color() != Qt::black) {
        stream << "stroke=\"" << colorToString(pen.color()) << "\" ";
        stream << "stroke-width=\"" << pen.widthF() << "\" ";
    }
    
    stream << "/>\n";
}

void SVGParser::writeEllipse(QTextStream &stream, Ellipse *ellipse)
{
    QPointF pos = ellipse->getPosition();
    QSizeF size = ellipse->getSize();
    QPen pen = ellipse->getPen();
    QBrush brush = ellipse->getBrush();
    
    double cx = pos.x() + size.width() / 2;
    double cy = pos.y() + size.height() / 2;
    double rx = size.width() / 2;
    double ry = size.height() / 2;
    
    stream << "  <ellipse cx=\"" << cx << "\" cy=\"" << cy << "\" ";
    stream << "rx=\"" << rx << "\" ry=\"" << ry << "\" ";
    
    if (brush.color() != Qt::white) {
        stream << "fill=\"" << colorToString(brush.color()) << "\" ";
    }
    if (pen.color() != Qt::black) {
        stream << "stroke=\"" << colorToString(pen.color()) << "\" ";
        stream << "stroke-width=\"" << pen.widthF() << "\" ";
    }
    
    stream << "/>\n";
}

void SVGParser::writeLine(QTextStream &stream, Line *line)
{
    QPointF start = line->getStartPoint();
    QPointF end = line->getEndPoint();
    QPen pen = line->getPen();
    
    stream << "  <line x1=\"" << start.x() << "\" y1=\"" << start.y() << "\" ";
    stream << "x2=\"" << end.x() << "\" y2=\"" << end.y() << "\" ";
    
    if (pen.color() != Qt::black) {
        stream << "stroke=\"" << colorToString(pen.color()) << "\" ";
        stream << "stroke-width=\"" << pen.widthF() << "\" ";
    }
    
    stream << "/>\n";
}

void SVGParser::writeBezier(QTextStream &stream, Bezier *bezier)
{
    if (bezier->getPointCount() < 2) return;
    
    stream << "  <path d=\"";
    stream << "M " << bezier->getPoint(0).x() << " " << bezier->getPoint(0).y();
    
    for (int i = 1; i < bezier->getPointCount(); i += 3) {
        if (i + 2 < bezier->getPointCount()) {
            stream << " C " << bezier->getPoint(i).x() << " " << bezier->getPoint(i).y();
            stream << " " << bezier->getPoint(i + 1).x() << " " << bezier->getPoint(i + 1).y();
            stream << " " << bezier->getPoint(i + 2).x() << " " << bezier->getPoint(i + 2).y();
        } else if (i + 1 < bezier->getPointCount()) {
            stream << " Q " << bezier->getPoint(i).x() << " " << bezier->getPoint(i).y();
            stream << " " << bezier->getPoint(i + 1).x() << " " << bezier->getPoint(i + 1).y();
        } else {
            stream << " L " << bezier->getPoint(i).x() << " " << bezier->getPoint(i).y();
        }
    }
    
    if (bezier->isClosed()) {
        stream << " Z";
    }
    
    stream << "\" ";
    
    QPen pen = bezier->getPen();
    QBrush brush = bezier->getBrush();
    
    if (brush.color() != Qt::white) {
        stream << "fill=\"" << colorToString(brush.color()) << "\" ";
    }
    if (pen.color() != Qt::black) {
        stream << "stroke=\"" << colorToString(pen.color()) << "\" ";
        stream << "stroke-width=\"" << pen.widthF() << "\" ";
    }
    
    stream << "/>\n";
}

QString SVGParser::colorToString(const QColor &color)
{
    return color.name();
} 