/********************************************************************************
** Form generated from reading UI file 'buttonsframe.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONSFRAME_H
#define UI_BUTTONSFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ButtonsFrame
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *lblErrorMsg;
    QPushButton *btnOK;
    QPushButton *btnCancel;

    void setupUi(QFrame *ButtonsFrame)
    {
        if (ButtonsFrame->objectName().isEmpty())
            ButtonsFrame->setObjectName(QStringLiteral("ButtonsFrame"));
        ButtonsFrame->resize(470, 133);
        ButtonsFrame->setFrameShape(QFrame::StyledPanel);
        ButtonsFrame->setFrameShadow(QFrame::Raised);
        widget = new QWidget(ButtonsFrame);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(260, 50, 184, 29));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lblErrorMsg = new QLabel(widget);
        lblErrorMsg->setObjectName(QStringLiteral("lblErrorMsg"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblErrorMsg->sizePolicy().hasHeightForWidth());
        lblErrorMsg->setSizePolicy(sizePolicy);
        lblErrorMsg->setStyleSheet(QStringLiteral("{color: red}"));

        horizontalLayout->addWidget(lblErrorMsg);

        btnOK = new QPushButton(widget);
        btnOK->setObjectName(QStringLiteral("btnOK"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnOK->sizePolicy().hasHeightForWidth());
        btnOK->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(btnOK);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        sizePolicy1.setHeightForWidth(btnCancel->sizePolicy().hasHeightForWidth());
        btnCancel->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(btnCancel);


        retranslateUi(ButtonsFrame);

        QMetaObject::connectSlotsByName(ButtonsFrame);
    } // setupUi

    void retranslateUi(QFrame *ButtonsFrame)
    {
        ButtonsFrame->setWindowTitle(QApplication::translate("ButtonsFrame", "Frame", Q_NULLPTR));
        lblErrorMsg->setText(QString());
        btnOK->setText(QApplication::translate("ButtonsFrame", "OK", Q_NULLPTR));
        btnCancel->setText(QApplication::translate("ButtonsFrame", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ButtonsFrame: public Ui_ButtonsFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONSFRAME_H
