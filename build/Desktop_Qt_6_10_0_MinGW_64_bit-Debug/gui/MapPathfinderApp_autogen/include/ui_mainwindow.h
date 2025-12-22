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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QPushButton *btnGenerate;
    QComboBox *comboHero;
    QPushButton *btnAddEdge;
    QPushButton *btnFindPath;
    QLabel *lblStatus;
    QSpacerItem *verticalSpacer;
    QGraphicsView *mapView;
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
        btnGenerate = new QPushButton(centralwidget);
        btnGenerate->setObjectName("btnGenerate");

        verticalLayout->addWidget(btnGenerate);

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

        btnFindPath = new QPushButton(centralwidget);
        btnFindPath->setObjectName("btnFindPath");

        verticalLayout->addWidget(btnFindPath);

        lblStatus = new QLabel(centralwidget);
        lblStatus->setObjectName("lblStatus");
        lblStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblStatus->setWordWrap(true);

        verticalLayout->addWidget(lblStatus);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        mapView = new QGraphicsView(centralwidget);
        mapView->setObjectName("mapView");
        mapView->setMouseTracking(true);

        horizontalLayout->addWidget(mapView);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 22));
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
        btnGenerate->setText(QCoreApplication::translate("MainWindow", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\272\320\260\321\200\321\202\321\203", nullptr));
        comboHero->setItemText(0, QCoreApplication::translate("MainWindow", "Human", nullptr));
        comboHero->setItemText(1, QCoreApplication::translate("MainWindow", "Wood Elf", nullptr));
        comboHero->setItemText(2, QCoreApplication::translate("MainWindow", "Orc", nullptr));
        comboHero->setItemText(3, QCoreApplication::translate("MainWindow", "Gnome", nullptr));

        btnAddEdge->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\321\214", nullptr));
        btnFindPath->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270 \320\277\321\203\321\202\321\214", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\321\203\321\201: \320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
