/********************************************************************************
** Form generated from reading UI file 'xtreewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XTREEWIDGET_H
#define UI_XTREEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_xTreeWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *xTreeWidget)
    {
        if (xTreeWidget->objectName().isEmpty())
            xTreeWidget->setObjectName(QStringLiteral("xTreeWidget"));
        xTreeWidget->resize(544, 389);
        horizontalLayout = new QHBoxLayout(xTreeWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(xTreeWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));

        horizontalLayout->addWidget(treeWidget);


        retranslateUi(xTreeWidget);

        QMetaObject::connectSlotsByName(xTreeWidget);
    } // setupUi

    void retranslateUi(QWidget *xTreeWidget)
    {
        xTreeWidget->setWindowTitle(QApplication::translate("xTreeWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class xTreeWidget: public Ui_xTreeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XTREEWIDGET_H
