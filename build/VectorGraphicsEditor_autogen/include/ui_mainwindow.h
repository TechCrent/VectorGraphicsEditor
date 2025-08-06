/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "canvas.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionImport_SVG;
    QAction *actionExport_SVG;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionDelete;
    QAction *actionSelect_All;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionFit_to_View;
    QAction *actionActual_Size;
    QAction *actionShow_Grid;
    QAction *actionSnap_to_Grid;
    QAction *actionShow_Rulers;
    QAction *actionShow_Guides;
    QAction *actionSelect;
    QAction *actionRectangle;
    QAction *actionEllipse;
    QAction *actionLine;
    QAction *actionBezier;
    QAction *actionText;
    QAction *actionPen;
    QAction *actionNew_Layer;
    QAction *actionDuplicate_Layer;
    QAction *actionDelete_Layer;
    QAction *actionMove_Layer_Up;
    QAction *actionMove_Layer_Down;
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QWidget *centralwidget;
    QHBoxLayout *centralLayout;
    QFrame *canvasFrame;
    QVBoxLayout *canvasLayout;
    Canvas *canvasWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuLayer;
    QMenu *menuHelp;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;
    QToolBar *toolsToolBar;
    QDockWidget *layersDock;
    QWidget *layersContent;
    QVBoxLayout *layersLayout;
    QFrame *layerButtonsFrame;
    QHBoxLayout *layerButtonsLayout;
    QPushButton *addLayerBtn;
    QPushButton *removeLayerBtn;
    QSpacerItem *layerButtonsSpacer;
    QListWidget *layersList;
    QDockWidget *propertiesDock;
    QWidget *propertiesContent;
    QVBoxLayout *propertiesLayout;
    QGroupBox *fillGroup;
    QVBoxLayout *fillLayout;
    QPushButton *fillColorBtn;
    QGroupBox *strokeGroup;
    QVBoxLayout *strokeLayout;
    QPushButton *strokeColorBtn;
    QFrame *strokeWidthFrame;
    QHBoxLayout *strokeWidthLayout;
    QLabel *strokeWidthLabel;
    QSpinBox *strokeWidthSpinBox;
    QSpacerItem *propertiesVerticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        MainWindow->setMinimumSize(QSize(1024, 768));
        QIcon icon;
        icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"QMainWindow {\n"
"    background-color: #2b2b2b;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"QMenuBar {\n"
"    background-color: #3c3c3c;\n"
"    color: #ffffff;\n"
"    border-bottom: 1px solid #555555;\n"
"    padding: 2px;\n"
"}\n"
"\n"
"QMenuBar::item {\n"
"    background-color: transparent;\n"
"    padding: 6px 12px;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    background-color: #4a90e2;\n"
"}\n"
"\n"
"QToolBar {\n"
"    background-color: #3c3c3c;\n"
"    border: none;\n"
"    spacing: 4px;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QToolButton {\n"
"    background-color: #4a4a4a;\n"
"    border: 1px solid #666666;\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"    margin: 2px;\n"
"    min-width: 32px;\n"
"    min-height: 32px;\n"
"}\n"
"\n"
"QToolButton:hover {\n"
"    background-color: #5a5a5a;\n"
"    border-color: #4a90e2;\n"
"}\n"
"\n"
"QToolButton:pressed, QToolButton:checked {\n"
"    background-color: #4a90e2;\n"
"    border-color: #357abd;\n"
"}\n"
"\n"
"QDockWid"
                        "get {\n"
"    background-color: #2b2b2b;\n"
"    color: #ffffff;\n"
"    border: 1px solid #555555;\n"
"    titlebar-close-icon: url(close.png);\n"
"    titlebar-normal-icon: url(float.png);\n"
"}\n"
"\n"
"QDockWidget::title {\n"
"    background-color: #3c3c3c;\n"
"    padding: 6px;\n"
"    border-bottom: 1px solid #555555;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QStatusBar {\n"
"    background-color: #3c3c3c;\n"
"    color: #ffffff;\n"
"    border-top: 1px solid #555555;\n"
"}\n"
"\n"
"#canvasWidget {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #555555;\n"
"    border-radius: 4px;\n"
"}\n"
"   "));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName("actionSave_As");
        actionImport_SVG = new QAction(MainWindow);
        actionImport_SVG->setObjectName("actionImport_SVG");
        actionExport_SVG = new QAction(MainWindow);
        actionExport_SVG->setObjectName("actionExport_SVG");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName("actionUndo");
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName("actionRedo");
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName("actionCut");
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName("actionCopy");
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName("actionPaste");
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName("actionDelete");
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName("actionSelect_All");
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName("actionZoom_In");
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName("actionZoom_Out");
        actionFit_to_View = new QAction(MainWindow);
        actionFit_to_View->setObjectName("actionFit_to_View");
        actionActual_Size = new QAction(MainWindow);
        actionActual_Size->setObjectName("actionActual_Size");
        actionShow_Grid = new QAction(MainWindow);
        actionShow_Grid->setObjectName("actionShow_Grid");
        actionShow_Grid->setCheckable(true);
        actionShow_Grid->setChecked(true);
        actionSnap_to_Grid = new QAction(MainWindow);
        actionSnap_to_Grid->setObjectName("actionSnap_to_Grid");
        actionSnap_to_Grid->setCheckable(true);
        actionShow_Rulers = new QAction(MainWindow);
        actionShow_Rulers->setObjectName("actionShow_Rulers");
        actionShow_Rulers->setCheckable(true);
        actionShow_Guides = new QAction(MainWindow);
        actionShow_Guides->setObjectName("actionShow_Guides");
        actionShow_Guides->setCheckable(true);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName("actionSelect");
        actionSelect->setCheckable(true);
        actionSelect->setChecked(true);
        actionRectangle = new QAction(MainWindow);
        actionRectangle->setObjectName("actionRectangle");
        actionRectangle->setCheckable(true);
        actionEllipse = new QAction(MainWindow);
        actionEllipse->setObjectName("actionEllipse");
        actionEllipse->setCheckable(true);
        actionLine = new QAction(MainWindow);
        actionLine->setObjectName("actionLine");
        actionLine->setCheckable(true);
        actionBezier = new QAction(MainWindow);
        actionBezier->setObjectName("actionBezier");
        actionBezier->setCheckable(true);
        actionText = new QAction(MainWindow);
        actionText->setObjectName("actionText");
        actionText->setCheckable(true);
        actionPen = new QAction(MainWindow);
        actionPen->setObjectName("actionPen");
        actionPen->setCheckable(true);
        actionNew_Layer = new QAction(MainWindow);
        actionNew_Layer->setObjectName("actionNew_Layer");
        actionDuplicate_Layer = new QAction(MainWindow);
        actionDuplicate_Layer->setObjectName("actionDuplicate_Layer");
        actionDelete_Layer = new QAction(MainWindow);
        actionDelete_Layer->setObjectName("actionDelete_Layer");
        actionMove_Layer_Up = new QAction(MainWindow);
        actionMove_Layer_Up->setObjectName("actionMove_Layer_Up");
        actionMove_Layer_Down = new QAction(MainWindow);
        actionMove_Layer_Down->setObjectName("actionMove_Layer_Down");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName("actionAbout_Qt");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QHBoxLayout(centralwidget);
        centralLayout->setSpacing(0);
        centralLayout->setObjectName("centralLayout");
        centralLayout->setContentsMargins(4, 4, 4, 4);
        canvasFrame = new QFrame(centralwidget);
        canvasFrame->setObjectName("canvasFrame");
        canvasFrame->setFrameShape(QFrame::StyledPanel);
        canvasFrame->setFrameShadow(QFrame::Raised);
        canvasFrame->setStyleSheet(QString::fromUtf8("\n"
"QFrame#canvasFrame {\n"
"    background-color: #f0f0f0;\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 8px;\n"
"}\n"
"       "));
        canvasLayout = new QVBoxLayout(canvasFrame);
        canvasLayout->setSpacing(0);
        canvasLayout->setObjectName("canvasLayout");
        canvasLayout->setContentsMargins(4, 4, 4, 4);
        canvasWidget = new Canvas(canvasFrame);
        canvasWidget->setObjectName("canvasWidget");
        canvasWidget->setMinimumSize(QSize(600, 400));
        canvasWidget->setStyleSheet(QString::fromUtf8("\n"
"Canvas {\n"
"    background-color: white;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 4px;\n"
"}\n"
"          "));

        canvasLayout->addWidget(canvasWidget);


        centralLayout->addWidget(canvasFrame);

        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1200, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menuBar);
        menuView->setObjectName("menuView");
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName("menuTools");
        menuLayer = new QMenu(menuBar);
        menuLayer->setObjectName("menuLayer");
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        statusBar->setStyleSheet(QString::fromUtf8("\n"
"QStatusBar {\n"
"    border-top: 1px solid #555555;\n"
"    padding: 2px;\n"
"}\n"
"    "));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        toolsToolBar = new QToolBar(MainWindow);
        toolsToolBar->setObjectName("toolsToolBar");
        toolsToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolsToolBar);
        layersDock = new QDockWidget(MainWindow);
        layersDock->setObjectName("layersDock");
        layersDock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        layersContent = new QWidget();
        layersContent->setObjectName("layersContent");
        layersLayout = new QVBoxLayout(layersContent);
        layersLayout->setSpacing(4);
        layersLayout->setObjectName("layersLayout");
        layersLayout->setContentsMargins(6, 6, 6, 6);
        layerButtonsFrame = new QFrame(layersContent);
        layerButtonsFrame->setObjectName("layerButtonsFrame");
        layerButtonsFrame->setFrameShape(QFrame::NoFrame);
        layerButtonsLayout = new QHBoxLayout(layerButtonsFrame);
        layerButtonsLayout->setSpacing(2);
        layerButtonsLayout->setObjectName("layerButtonsLayout");
        layerButtonsLayout->setContentsMargins(0, 0, 0, 0);
        addLayerBtn = new QPushButton(layerButtonsFrame);
        addLayerBtn->setObjectName("addLayerBtn");
        addLayerBtn->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: #4a90e2;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 6px 12px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #357abd;\n"
"}\n"
"           "));

        layerButtonsLayout->addWidget(addLayerBtn);

        removeLayerBtn = new QPushButton(layerButtonsFrame);
        removeLayerBtn->setObjectName("removeLayerBtn");
        removeLayerBtn->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: #e74c3c;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 6px 12px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #c0392b;\n"
"}\n"
"           "));

        layerButtonsLayout->addWidget(removeLayerBtn);

        layerButtonsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layerButtonsLayout->addItem(layerButtonsSpacer);


        layersLayout->addWidget(layerButtonsFrame);

        layersList = new QListWidget(layersContent);
        layersList->setObjectName("layersList");
        layersList->setStyleSheet(QString::fromUtf8("\n"
"QListWidget {\n"
"    background-color: #4a4a4a;\n"
"    border: 1px solid #666666;\n"
"    border-radius: 4px;\n"
"    color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"QListWidget::item {\n"
"    background-color: #4a4a4a;\n"
"    border-bottom: 1px solid #666666;\n"
"    padding: 8px 12px;\n"
"}\n"
"\n"
"QListWidget::item:selected {\n"
"    background-color: #4a90e2;\n"
"}\n"
"\n"
"QListWidget::item:hover {\n"
"    background-color: #5a5a5a;\n"
"}\n"
"        "));

        layersLayout->addWidget(layersList);

        layersDock->setWidget(layersContent);
        MainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, layersDock);
        propertiesDock = new QDockWidget(MainWindow);
        propertiesDock->setObjectName("propertiesDock");
        propertiesDock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        propertiesContent = new QWidget();
        propertiesContent->setObjectName("propertiesContent");
        propertiesLayout = new QVBoxLayout(propertiesContent);
        propertiesLayout->setSpacing(8);
        propertiesLayout->setObjectName("propertiesLayout");
        propertiesLayout->setContentsMargins(6, 6, 6, 6);
        fillGroup = new QGroupBox(propertiesContent);
        fillGroup->setObjectName("fillGroup");
        fillGroup->setStyleSheet(QString::fromUtf8("\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    border: 2px solid #666666;\n"
"    border-radius: 4px;\n"
"    margin-top: 8px;\n"
"    padding-top: 4px;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 5px 0 5px;\n"
"}\n"
"        "));
        fillLayout = new QVBoxLayout(fillGroup);
        fillLayout->setObjectName("fillLayout");
        fillColorBtn = new QPushButton(fillGroup);
        fillColorBtn->setObjectName("fillColorBtn");
        fillColorBtn->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: #ffffff;\n"
"    color: #000000;\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-weight: normal;\n"
"}\n"
"QPushButton:hover {\n"
"    border-color: #4a90e2;\n"
"}\n"
"           "));

        fillLayout->addWidget(fillColorBtn);


        propertiesLayout->addWidget(fillGroup);

        strokeGroup = new QGroupBox(propertiesContent);
        strokeGroup->setObjectName("strokeGroup");
        strokeGroup->setStyleSheet(QString::fromUtf8("\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    border: 2px solid #666666;\n"
"    border-radius: 4px;\n"
"    margin-top: 8px;\n"
"    padding-top: 4px;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 5px 0 5px;\n"
"}\n"
"        "));
        strokeLayout = new QVBoxLayout(strokeGroup);
        strokeLayout->setObjectName("strokeLayout");
        strokeColorBtn = new QPushButton(strokeGroup);
        strokeColorBtn->setObjectName("strokeColorBtn");
        strokeColorBtn->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: #000000;\n"
"    color: #ffffff;\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-weight: normal;\n"
"}\n"
"QPushButton:hover {\n"
"    border-color: #4a90e2;\n"
"}\n"
"           "));

        strokeLayout->addWidget(strokeColorBtn);

        strokeWidthFrame = new QFrame(strokeGroup);
        strokeWidthFrame->setObjectName("strokeWidthFrame");
        strokeWidthFrame->setFrameShape(QFrame::NoFrame);
        strokeWidthLayout = new QHBoxLayout(strokeWidthFrame);
        strokeWidthLayout->setObjectName("strokeWidthLayout");
        strokeWidthLayout->setContentsMargins(0, 0, 0, 0);
        strokeWidthLabel = new QLabel(strokeWidthFrame);
        strokeWidthLabel->setObjectName("strokeWidthLabel");
        strokeWidthLabel->setStyleSheet(QString::fromUtf8("color: #ffffff;"));

        strokeWidthLayout->addWidget(strokeWidthLabel);

        strokeWidthSpinBox = new QSpinBox(strokeWidthFrame);
        strokeWidthSpinBox->setObjectName("strokeWidthSpinBox");
        strokeWidthSpinBox->setMinimum(1);
        strokeWidthSpinBox->setMaximum(100);
        strokeWidthSpinBox->setValue(2);
        strokeWidthSpinBox->setStyleSheet(QString::fromUtf8("\n"
"QSpinBox {\n"
"    background-color: #4a4a4a;\n"
"    border: 1px solid #666666;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    color: #ffffff;\n"
"}\n"
"QSpinBox:hover {\n"
"    border-color: #4a90e2;\n"
"}\n"
"              "));

        strokeWidthLayout->addWidget(strokeWidthSpinBox);


        strokeLayout->addWidget(strokeWidthFrame);


        propertiesLayout->addWidget(strokeGroup);

        propertiesVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        propertiesLayout->addItem(propertiesVerticalSpacer);

        propertiesDock->setWidget(propertiesContent);
        MainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, propertiesDock);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuLayer->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionImport_SVG);
        menuFile->addAction(actionExport_SVG);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuView->addAction(actionZoom_In);
        menuView->addAction(actionZoom_Out);
        menuView->addAction(actionFit_to_View);
        menuView->addAction(actionActual_Size);
        menuView->addSeparator();
        menuView->addAction(actionShow_Grid);
        menuView->addAction(actionSnap_to_Grid);
        menuView->addSeparator();
        menuView->addAction(actionShow_Rulers);
        menuView->addAction(actionShow_Guides);
        menuTools->addAction(actionSelect);
        menuTools->addSeparator();
        menuTools->addAction(actionRectangle);
        menuTools->addAction(actionEllipse);
        menuTools->addAction(actionLine);
        menuTools->addAction(actionBezier);
        menuTools->addSeparator();
        menuTools->addAction(actionText);
        menuTools->addAction(actionPen);
        menuLayer->addAction(actionNew_Layer);
        menuLayer->addAction(actionDuplicate_Layer);
        menuLayer->addAction(actionDelete_Layer);
        menuLayer->addSeparator();
        menuLayer->addAction(actionMove_Layer_Up);
        menuLayer->addAction(actionMove_Layer_Down);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionUndo);
        mainToolBar->addAction(actionRedo);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCut);
        mainToolBar->addAction(actionCopy);
        mainToolBar->addAction(actionPaste);
        toolsToolBar->addAction(actionSelect);
        toolsToolBar->addSeparator();
        toolsToolBar->addAction(actionRectangle);
        toolsToolBar->addAction(actionEllipse);
        toolsToolBar->addAction(actionLine);
        toolsToolBar->addAction(actionBezier);
        toolsToolBar->addSeparator();
        toolsToolBar->addAction(actionText);
        toolsToolBar->addAction(actionPen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Vector Graphics Editor - Professional", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "&New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "&Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "&Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_As->setText(QCoreApplication::translate("MainWindow", "Save &As...", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_As->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionImport_SVG->setText(QCoreApplication::translate("MainWindow", "&Import SVG...", nullptr));
        actionExport_SVG->setText(QCoreApplication::translate("MainWindow", "&Export SVG...", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "E&xit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionUndo->setText(QCoreApplication::translate("MainWindow", "&Undo", nullptr));
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRedo->setText(QCoreApplication::translate("MainWindow", "&Redo", nullptr));
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cu&t", nullptr));
#if QT_CONFIG(shortcut)
        actionCut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopy->setText(QCoreApplication::translate("MainWindow", "&Copy", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("MainWindow", "&Paste", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDelete->setText(QCoreApplication::translate("MainWindow", "&Delete", nullptr));
#if QT_CONFIG(shortcut)
        actionDelete->setShortcut(QCoreApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelect_All->setText(QCoreApplication::translate("MainWindow", "Select &All", nullptr));
#if QT_CONFIG(shortcut)
        actionSelect_All->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoom_In->setText(QCoreApplication::translate("MainWindow", "Zoom &In", nullptr));
#if QT_CONFIG(shortcut)
        actionZoom_In->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+=", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoom_Out->setText(QCoreApplication::translate("MainWindow", "Zoom &Out", nullptr));
#if QT_CONFIG(shortcut)
        actionZoom_Out->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFit_to_View->setText(QCoreApplication::translate("MainWindow", "&Fit to View", nullptr));
#if QT_CONFIG(shortcut)
        actionFit_to_View->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+0", nullptr));
#endif // QT_CONFIG(shortcut)
        actionActual_Size->setText(QCoreApplication::translate("MainWindow", "&Actual Size", nullptr));
#if QT_CONFIG(shortcut)
        actionActual_Size->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionShow_Grid->setText(QCoreApplication::translate("MainWindow", "Show &Grid", nullptr));
        actionSnap_to_Grid->setText(QCoreApplication::translate("MainWindow", "&Snap to Grid", nullptr));
        actionShow_Rulers->setText(QCoreApplication::translate("MainWindow", "Show &Rulers", nullptr));
        actionShow_Guides->setText(QCoreApplication::translate("MainWindow", "Show G&uides", nullptr));
        actionSelect->setText(QCoreApplication::translate("MainWindow", "&Select", nullptr));
#if QT_CONFIG(shortcut)
        actionSelect->setShortcut(QCoreApplication::translate("MainWindow", "V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRectangle->setText(QCoreApplication::translate("MainWindow", "&Rectangle", nullptr));
#if QT_CONFIG(shortcut)
        actionRectangle->setShortcut(QCoreApplication::translate("MainWindow", "R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionEllipse->setText(QCoreApplication::translate("MainWindow", "&Ellipse", nullptr));
#if QT_CONFIG(shortcut)
        actionEllipse->setShortcut(QCoreApplication::translate("MainWindow", "E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLine->setText(QCoreApplication::translate("MainWindow", "&Line", nullptr));
#if QT_CONFIG(shortcut)
        actionLine->setShortcut(QCoreApplication::translate("MainWindow", "L", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBezier->setText(QCoreApplication::translate("MainWindow", "&Bezier", nullptr));
#if QT_CONFIG(shortcut)
        actionBezier->setShortcut(QCoreApplication::translate("MainWindow", "B", nullptr));
#endif // QT_CONFIG(shortcut)
        actionText->setText(QCoreApplication::translate("MainWindow", "&Text", nullptr));
#if QT_CONFIG(shortcut)
        actionText->setShortcut(QCoreApplication::translate("MainWindow", "T", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPen->setText(QCoreApplication::translate("MainWindow", "&Pen", nullptr));
#if QT_CONFIG(shortcut)
        actionPen->setShortcut(QCoreApplication::translate("MainWindow", "P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNew_Layer->setText(QCoreApplication::translate("MainWindow", "&New Layer", nullptr));
#if QT_CONFIG(shortcut)
        actionNew_Layer->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDuplicate_Layer->setText(QCoreApplication::translate("MainWindow", "&Duplicate Layer", nullptr));
        actionDelete_Layer->setText(QCoreApplication::translate("MainWindow", "&Delete Layer", nullptr));
        actionMove_Layer_Up->setText(QCoreApplication::translate("MainWindow", "Move Layer &Up", nullptr));
        actionMove_Layer_Down->setText(QCoreApplication::translate("MainWindow", "Move Layer &Down", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "&About", nullptr));
        actionAbout_Qt->setText(QCoreApplication::translate("MainWindow", "About &Qt", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "&View", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "&Tools", nullptr));
        menuLayer->setTitle(QCoreApplication::translate("MainWindow", "&Layer", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Main Toolbar", nullptr));
        toolsToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        layersDock->setWindowTitle(QCoreApplication::translate("MainWindow", "Layers", nullptr));
        addLayerBtn->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
#if QT_CONFIG(tooltip)
        addLayerBtn->setToolTip(QCoreApplication::translate("MainWindow", "Add Layer", nullptr));
#endif // QT_CONFIG(tooltip)
        removeLayerBtn->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
#if QT_CONFIG(tooltip)
        removeLayerBtn->setToolTip(QCoreApplication::translate("MainWindow", "Remove Layer", nullptr));
#endif // QT_CONFIG(tooltip)
        propertiesDock->setWindowTitle(QCoreApplication::translate("MainWindow", "Properties", nullptr));
        fillGroup->setTitle(QCoreApplication::translate("MainWindow", "Fill", nullptr));
        fillColorBtn->setText(QCoreApplication::translate("MainWindow", "Fill Color", nullptr));
        strokeGroup->setTitle(QCoreApplication::translate("MainWindow", "Stroke", nullptr));
        strokeColorBtn->setText(QCoreApplication::translate("MainWindow", "Stroke Color", nullptr));
        strokeWidthLabel->setText(QCoreApplication::translate("MainWindow", "Width:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
