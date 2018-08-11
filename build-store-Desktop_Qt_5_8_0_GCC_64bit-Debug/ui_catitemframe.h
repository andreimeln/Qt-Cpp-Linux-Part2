/********************************************************************************
** Form generated from reading UI file 'catitemframe.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATITEMFRAME_H
#define UI_CATITEMFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CatItemFrame
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *lblCode;
    QLineEdit *edtCode;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblTitle;
    QLineEdit *edtTitle;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QLabel *lblPeriod;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *cbxFromEnabled;
    QDateEdit *edtFrom;
    QCheckBox *cbxToEnabled;
    QDateEdit *edtTo;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *cbxLocal;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *lblComment;
    QTextEdit *edtComment;

    void setupUi(QFrame *CatItemFrame)
    {
        if (CatItemFrame->objectName().isEmpty())
            CatItemFrame->setObjectName(QStringLiteral("CatItemFrame"));
        CatItemFrame->resize(569, 389);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CatItemFrame->sizePolicy().hasHeightForWidth());
        CatItemFrame->setSizePolicy(sizePolicy);
        CatItemFrame->setFrameShape(QFrame::StyledPanel);
        CatItemFrame->setFrameShadow(QFrame::Raised);
        widget = new QWidget(CatItemFrame);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 541, 341));
        verticalLayout_5 = new QVBoxLayout(widget);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lblCode = new QLabel(widget);
        lblCode->setObjectName(QStringLiteral("lblCode"));

        verticalLayout->addWidget(lblCode);

        edtCode = new QLineEdit(widget);
        edtCode->setObjectName(QStringLiteral("edtCode"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(edtCode->sizePolicy().hasHeightForWidth());
        edtCode->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(edtCode);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lblTitle = new QLabel(widget);
        lblTitle->setObjectName(QStringLiteral("lblTitle"));
        sizePolicy.setHeightForWidth(lblTitle->sizePolicy().hasHeightForWidth());
        lblTitle->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(lblTitle);

        edtTitle = new QLineEdit(widget);
        edtTitle->setObjectName(QStringLiteral("edtTitle"));

        verticalLayout_2->addWidget(edtTitle);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_5->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        lblPeriod = new QLabel(widget);
        lblPeriod->setObjectName(QStringLiteral("lblPeriod"));

        verticalLayout_3->addWidget(lblPeriod);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        cbxFromEnabled = new QCheckBox(widget);
        cbxFromEnabled->setObjectName(QStringLiteral("cbxFromEnabled"));
        cbxFromEnabled->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_2->addWidget(cbxFromEnabled);

        edtFrom = new QDateEdit(widget);
        edtFrom->setObjectName(QStringLiteral("edtFrom"));
        edtFrom->setEnabled(false);
        edtFrom->setCalendarPopup(true);

        horizontalLayout_2->addWidget(edtFrom);

        cbxToEnabled = new QCheckBox(widget);
        cbxToEnabled->setObjectName(QStringLiteral("cbxToEnabled"));
        cbxToEnabled->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_2->addWidget(cbxToEnabled);

        edtTo = new QDateEdit(widget);
        edtTo->setObjectName(QStringLiteral("edtTo"));
        edtTo->setEnabled(false);
        edtTo->setCalendarPopup(true);

        horizontalLayout_2->addWidget(edtTo);


        verticalLayout_3->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        cbxLocal = new QCheckBox(widget);
        cbxLocal->setObjectName(QStringLiteral("cbxLocal"));

        horizontalLayout_3->addWidget(cbxLocal);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_5->addLayout(horizontalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        lblComment = new QLabel(widget);
        lblComment->setObjectName(QStringLiteral("lblComment"));

        verticalLayout_4->addWidget(lblComment);

        edtComment = new QTextEdit(widget);
        edtComment->setObjectName(QStringLiteral("edtComment"));

        verticalLayout_4->addWidget(edtComment);


        verticalLayout_5->addLayout(verticalLayout_4);


        retranslateUi(CatItemFrame);
        QObject::connect(cbxFromEnabled, SIGNAL(toggled(bool)), edtFrom, SLOT(setEnabled(bool)));
        QObject::connect(cbxToEnabled, SIGNAL(toggled(bool)), edtTo, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(CatItemFrame);
    } // setupUi

    void retranslateUi(QFrame *CatItemFrame)
    {
        CatItemFrame->setWindowTitle(QApplication::translate("CatItemFrame", "Frame", Q_NULLPTR));
        lblCode->setText(QApplication::translate("CatItemFrame", "Code", Q_NULLPTR));
        lblTitle->setText(QApplication::translate("CatItemFrame", "Title", Q_NULLPTR));
        lblPeriod->setText(QApplication::translate("CatItemFrame", "Period", Q_NULLPTR));
        cbxFromEnabled->setText(QApplication::translate("CatItemFrame", "From", Q_NULLPTR));
        cbxToEnabled->setText(QApplication::translate("CatItemFrame", "To", Q_NULLPTR));
        cbxLocal->setText(QApplication::translate("CatItemFrame", "Local", Q_NULLPTR));
        lblComment->setText(QApplication::translate("CatItemFrame", "Comment", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CatItemFrame: public Ui_CatItemFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATITEMFRAME_H
