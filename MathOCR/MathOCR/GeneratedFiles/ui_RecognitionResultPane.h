/********************************************************************************
** Form generated from reading UI file 'RecognitionResultPane.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECOGNITIONRESULTPANE_H
#define UI_RECOGNITIONRESULTPANE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecognitionResultPane
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *controlsViewLayout;
    QVBoxLayout *objectsViewLayout;
    QListWidget *listWidget;
    QLabel *label;
    QHBoxLayout *FullResultLayout;
    QLabel *FullResultLabel;
    QLineEdit *FullResultLineEdit;
    QHBoxLayout *PossibleCharsLayout;
    QLabel *PossibleCharsLabel;
    QLineEdit *PossibleCharsLineEdit;
    QVBoxLayout *BarChartLayout;

    void setupUi(QWidget *RecognitionResultPane)
    {
        if (RecognitionResultPane->objectName().isEmpty())
            RecognitionResultPane->setObjectName(QStringLiteral("RecognitionResultPane"));
        RecognitionResultPane->resize(577, 549);
        verticalLayout = new QVBoxLayout(RecognitionResultPane);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        controlsViewLayout = new QVBoxLayout();
        controlsViewLayout->setObjectName(QStringLiteral("controlsViewLayout"));
        objectsViewLayout = new QVBoxLayout();
        objectsViewLayout->setObjectName(QStringLiteral("objectsViewLayout"));
        listWidget = new QListWidget(RecognitionResultPane);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMinimumSize(QSize(0, 300));

        objectsViewLayout->addWidget(listWidget);

        label = new QLabel(RecognitionResultPane);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        objectsViewLayout->addWidget(label);


        controlsViewLayout->addLayout(objectsViewLayout);

        FullResultLayout = new QHBoxLayout();
        FullResultLayout->setObjectName(QStringLiteral("FullResultLayout"));
        FullResultLabel = new QLabel(RecognitionResultPane);
        FullResultLabel->setObjectName(QStringLiteral("FullResultLabel"));

        FullResultLayout->addWidget(FullResultLabel);

        FullResultLineEdit = new QLineEdit(RecognitionResultPane);
        FullResultLineEdit->setObjectName(QStringLiteral("FullResultLineEdit"));

        FullResultLayout->addWidget(FullResultLineEdit);


        controlsViewLayout->addLayout(FullResultLayout);

        PossibleCharsLayout = new QHBoxLayout();
        PossibleCharsLayout->setObjectName(QStringLiteral("PossibleCharsLayout"));
        PossibleCharsLabel = new QLabel(RecognitionResultPane);
        PossibleCharsLabel->setObjectName(QStringLiteral("PossibleCharsLabel"));

        PossibleCharsLayout->addWidget(PossibleCharsLabel);

        PossibleCharsLineEdit = new QLineEdit(RecognitionResultPane);
        PossibleCharsLineEdit->setObjectName(QStringLiteral("PossibleCharsLineEdit"));

        PossibleCharsLayout->addWidget(PossibleCharsLineEdit);


        controlsViewLayout->addLayout(PossibleCharsLayout);


        verticalLayout_2->addLayout(controlsViewLayout);


        verticalLayout->addLayout(verticalLayout_2);

        BarChartLayout = new QVBoxLayout();
        BarChartLayout->setObjectName(QStringLiteral("BarChartLayout"));

        verticalLayout->addLayout(BarChartLayout);


        retranslateUi(RecognitionResultPane);

        QMetaObject::connectSlotsByName(RecognitionResultPane);
    } // setupUi

    void retranslateUi(QWidget *RecognitionResultPane)
    {
        RecognitionResultPane->setWindowTitle(QApplication::translate("RecognitionResultPane", "Form", 0));
        label->setText(QApplication::translate("RecognitionResultPane", "Objects", 0));
        FullResultLabel->setText(QApplication::translate("RecognitionResultPane", "Full result", 0));
        PossibleCharsLabel->setText(QApplication::translate("RecognitionResultPane", "Selection possible chars", 0));
    } // retranslateUi

};

namespace Ui {
    class RecognitionResultPane: public Ui_RecognitionResultPane {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECOGNITIONRESULTPANE_H
