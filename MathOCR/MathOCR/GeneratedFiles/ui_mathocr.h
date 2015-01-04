/********************************************************************************
** Form generated from reading UI file 'mathocr.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATHOCR_H
#define UI_MATHOCR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MathOCRClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MathOCRClass)
    {
        if (MathOCRClass->objectName().isEmpty())
            MathOCRClass->setObjectName(QStringLiteral("MathOCRClass"));
        MathOCRClass->resize(600, 400);
        centralWidget = new QWidget(MathOCRClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MathOCRClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MathOCRClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        MathOCRClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MathOCRClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MathOCRClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MathOCRClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MathOCRClass->setStatusBar(statusBar);

        retranslateUi(MathOCRClass);

        QMetaObject::connectSlotsByName(MathOCRClass);
    } // setupUi

    void retranslateUi(QMainWindow *MathOCRClass)
    {
        MathOCRClass->setWindowTitle(QApplication::translate("MathOCRClass", "MathOCR", 0));
    } // retranslateUi

};

namespace Ui {
    class MathOCRClass: public Ui_MathOCRClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATHOCR_H
