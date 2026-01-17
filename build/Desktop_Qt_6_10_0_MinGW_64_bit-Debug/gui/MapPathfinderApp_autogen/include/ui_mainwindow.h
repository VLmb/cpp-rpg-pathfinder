/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *btnGenerateNatural;
    QPushButton *btnGenerateCast;
    QComboBox *comboHero;
    QPushButton *btnAddEdge;
    QPushButton *btnRemoveEdge;
    QPushButton *btnClearMap;
    QPushButton *btnFindPathNatural;
    QLabel *lblStatus;
    QSpacerItem *verticalSpacer;
    QGraphicsView *mapView;
    QGroupBox *legendGroup;
    QVBoxLayout *verticalLayout_Legend;
    QScrollArea *scrollAreaLegend;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *legendLayoutContainer;
    QSpacerItem *verticalSpacerLegend;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        btnGenerateNatural = new QPushButton(centralwidget);
        btnGenerateNatural->setObjectName("btnGenerateNatural");

        verticalLayout->addWidget(btnGenerateNatural);

        btnGenerateCast = new QPushButton(centralwidget);
        btnGenerateCast->setObjectName("btnGenerateCast");

        verticalLayout->addWidget(btnGenerateCast);

        comboHero = new QComboBox(centralwidget);
        comboHero->addItem(QString());
        comboHero->addItem(QString());
        comboHero->addItem(QString());
        comboHero->addItem(QString());
        comboHero->setObjectName("comboHero");

        verticalLayout->addWidget(comboHero);

        btnAddEdge = new QPushButton(centralwidget);
        btnAddEdge->setObjectName("btnAddEdge");

        verticalLayout->addWidget(btnAddEdge);

        btnRemoveEdge = new QPushButton(centralwidget);
        btnRemoveEdge->setObjectName("btnRemoveEdge");

        verticalLayout->addWidget(btnRemoveEdge);

        btnClearMap = new QPushButton(centralwidget);
        btnClearMap->setObjectName("btnClearMap");

        verticalLayout->addWidget(btnClearMap);

        btnFindPathNatural = new QPushButton(centralwidget);
        btnFindPathNatural->setObjectName("btnFindPathNatural");

        verticalLayout->addWidget(btnFindPathNatural);

        lblStatus = new QLabel(centralwidget);
        lblStatus->setObjectName("lblStatus");
        lblStatus->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lblStatus->setWordWrap(true);

        verticalLayout->addWidget(lblStatus);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        mapView = new QGraphicsView(centralwidget);
        mapView->setObjectName("mapView");
        mapView->setMouseTracking(true);

        horizontalLayout->addWidget(mapView);

        legendGroup = new QGroupBox(centralwidget);
        legendGroup->setObjectName("legendGroup");
        legendGroup->setMinimumSize(QSize(220, 0));
        legendGroup->setMaximumSize(QSize(250, 16777215));
        verticalLayout_Legend = new QVBoxLayout(legendGroup);
        verticalLayout_Legend->setObjectName("verticalLayout_Legend");
        verticalLayout_Legend->setContentsMargins(5, -1, 5, -1);
        scrollAreaLegend = new QScrollArea(legendGroup);
        scrollAreaLegend->setObjectName("scrollAreaLegend");
        scrollAreaLegend->setFrameShape(QFrame::Shape::NoFrame);
        scrollAreaLegend->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 234, 596));
        legendLayoutContainer = new QVBoxLayout(scrollAreaWidgetContents);
        legendLayoutContainer->setSpacing(5);
        legendLayoutContainer->setObjectName("legendLayoutContainer");
        legendLayoutContainer->setContentsMargins(0, 0, 0, 0);
        verticalSpacerLegend = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        legendLayoutContainer->addItem(verticalSpacerLegend);

        scrollAreaLegend->setWidget(scrollAreaWidgetContents);

        verticalLayout_Legend->addWidget(scrollAreaLegend);


        horizontalLayout->addWidget(legendGroup);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "RPG Pathfinder", nullptr));
        btnGenerateNatural->setText(QCoreApplication::translate("MainWindow", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\276\320\261\321\213\321\207\320\275\321\203\321\216 \320\272\320\260\321\200\321\202\321\203", nullptr));
        btnGenerateCast->setText(QCoreApplication::translate("MainWindow", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \321\203\320\277\321\200\320\276\321\211\320\265\320\275\320\275\321\203\321\216 \320\272\320\260\321\200\321\202\321\203", nullptr));
        comboHero->setItemText(0, QCoreApplication::translate("MainWindow", "Human", nullptr));
        comboHero->setItemText(1, QCoreApplication::translate("MainWindow", "Wood Elf", nullptr));
        comboHero->setItemText(2, QCoreApplication::translate("MainWindow", "Orc", nullptr));
        comboHero->setItemText(3, QCoreApplication::translate("MainWindow", "Gnome", nullptr));

        btnAddEdge->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\321\214", nullptr));
        btnRemoveEdge->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\321\214", nullptr));
        btnClearMap->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\272\320\260\321\200\321\202\321\203", nullptr));
        btnFindPathNatural->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270 \320\277\321\203\321\202\321\214", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\321\203\321\201: \320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265", nullptr));
        legendGroup->setTitle(QCoreApplication::translate("MainWindow", "\320\233\320\265\320\263\320\265\320\275\320\264\320\260 \320\261\320\270\320\276\320\274\320\276\320\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
