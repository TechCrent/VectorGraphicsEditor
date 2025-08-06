#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QWidget window;
    window.setWindowTitle("Qt Integration Test - Vector Graphics Editor");
    window.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&window);
    
    QLabel *titleLabel = new QLabel("Vector Graphics Editor - Qt Integration Test");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    layout->addWidget(titleLabel);
    
    QLabel *statusLabel = new QLabel("✓ Qt is working correctly!");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: green; font-size: 14px; margin: 10px;");
    layout->addWidget(statusLabel);
    
    QLabel *infoLabel = new QLabel("This test verifies that:\n"
                                  "• Qt5 is properly linked\n"
                                  "• UI components work\n"
                                  "• Event handling functions\n"
                                  "• Layout system is operational");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("margin: 10px;");
    layout->addWidget(infoLabel);
    
    QPushButton *testButton = new QPushButton("Test Button - Click Me!");
    testButton->setStyleSheet("QPushButton { padding: 10px; font-size: 12px; }");
    layout->addWidget(testButton);
    
    QObject::connect(testButton, &QPushButton::clicked, [&]() {
        QMessageBox::information(&window, "Test Success", 
            "Qt integration is working perfectly!\n\n"
            "The Vector Graphics Editor should build and run correctly.");
    });
    
    QLabel *footerLabel = new QLabel("Ready to build the main application!");
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setStyleSheet("color: blue; font-size: 12px; margin: 10px;");
    layout->addWidget(footerLabel);
    
    window.show();
    
    return app.exec();
} 