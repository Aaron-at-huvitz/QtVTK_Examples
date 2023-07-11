/********************************************************************************
** Form generated from reading UI file 'QVTKWidgetWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QVTKWIDGETWINDOW_H
#define UI_QVTKWIDGETWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_QVTKWidgetWindowClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVTKWidget *vtkWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QVTKWidgetWindowClass)
    {
        if (QVTKWidgetWindowClass->objectName().isEmpty())
            QVTKWidgetWindowClass->setObjectName("QVTKWidgetWindowClass");
        QVTKWidgetWindowClass->resize(600, 400);
        centralWidget = new QWidget(QVTKWidgetWindowClass);
        centralWidget->setObjectName("centralWidget");
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        vtkWidget = new QVTKWidget(centralWidget);
        vtkWidget->setObjectName("vtkWidget");

        horizontalLayout->addWidget(vtkWidget);

        QVTKWidgetWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QVTKWidgetWindowClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        QVTKWidgetWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QVTKWidgetWindowClass);
        mainToolBar->setObjectName("mainToolBar");
        QVTKWidgetWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QVTKWidgetWindowClass);
        statusBar->setObjectName("statusBar");
        QVTKWidgetWindowClass->setStatusBar(statusBar);

        retranslateUi(QVTKWidgetWindowClass);

        QMetaObject::connectSlotsByName(QVTKWidgetWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *QVTKWidgetWindowClass)
    {
        QVTKWidgetWindowClass->setWindowTitle(QCoreApplication::translate("QVTKWidgetWindowClass", "QVTKWidgetWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QVTKWidgetWindowClass: public Ui_QVTKWidgetWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVTKWIDGETWINDOW_H
