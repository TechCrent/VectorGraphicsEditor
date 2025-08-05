#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Set application properties
    app.setApplicationName("Vector Graphics Editor");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Simple Vector Editor");
    
    MainWindow window;
    window.show();
    
    return app.exec();
} 