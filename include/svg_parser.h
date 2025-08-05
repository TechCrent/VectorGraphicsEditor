#ifndef SVG_PARSER_H
#define SVG_PARSER_H

#include <QString>
#include <QList>
#include <QTextStream>
// #include <libxml/parser.h>
// #include <libxml/tree.h>
#include "shape.h"
#include "rectangle.h"
#include "ellipse.h"
#include "line.h"
#include "bezier.h"

class Document;

class SVGParser
{
public:
    SVGParser();
    ~SVGParser();

    // Import SVG file
    bool importFromFile(const QString &filename, Document *document);
    
    // Export to SVG file
    bool exportToFile(const QString &filename, Document *document);
    
    // Parse SVG string
    bool parseSVGString(const QString &svgString, Document *document);
    
    // Generate SVG string
    QString generateSVGString(Document *document);

private:
    // Import helpers
    void parseBasicShapes(const QString &svgString, Document *document);
    void parseRectElement(const QString &element, Document *document);
    void parseEllipseElement(const QString &element, Document *document);
    void parseLineElement(const QString &element, Document *document);
    void parseShapeStyle(const QString &element, Shape *shape);
    
    // Export helpers
    void writeShape(QTextStream &stream, Shape *shape);
    void writeRectangle(QTextStream &stream, Rectangle *rect);
    void writeEllipse(QTextStream &stream, Ellipse *ellipse);
    void writeLine(QTextStream &stream, Line *line);
    void writeBezier(QTextStream &stream, Bezier *bezier);
    
    // Utility functions
    QColor parseColor(const QString &colorString);
    QString extractAttribute(const QString &element, const QString &attribute);
    QString colorToString(const QColor &color);
};

#endif // SVG_PARSER_H 