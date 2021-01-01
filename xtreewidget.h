#ifndef XTREEWIDGET_H
#define XTREEWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include "qlibwin/qlib.h"
namespace Ui {
class xTreeWidget;
}
/*
 * 不调用setheaderText的话,就是普通的一列树,带图标
//    QTreeWidgetItem*proot=ui->widget->addRoot(("根节点"));
//    QTreeWidgetItem* pjoint2=ui->widget->newJoint(("吊车架"));
//    QTreeWidgetItem* pjoint3=ui->widget->newJoint(("三级节点"));
//    QTreeWidgetItem* pjoint4=ui->widget->newJoint(("钢柱"));
//    QTreeWidgetItem* pjoint5=ui->widget->newJoint(("xlfdQProcess"));
//    proot->insertChild(0,pjoint2);
//    pjoint2->insertChild(0,pjoint3);
//    proot->addChild(pjoint4);
//    proot->insertChild(2,pjoint5);
 *调用的话,就是传输客户端那种多列的,可以展开
    ui->widget->setHeaderText(QStringList()<<"任务流水号"<<"作业任务编号"<<"作业任务模式");
    ui->widget->setColWidth(0,300);
    ui->widget->setColWidth(1,300);
    ui->widget->setColWidth(2,220);

    QTreeWidgetItem* pdir1=ui->widget->addRootList(QStringList()<<"DT202009121111");

    QTreeWidgetItem* proot=ui->widget->newJointList(QStringList()<<"DT202009121111_MYC"<<"JOB202008130000001"<<"T53");
    QTreeWidgetItem* pRoadName=ui->widget->newJointList(QStringList()<<"通道标识"<<"任务流水号"<<"通道状态");
    QTreeWidgetItem* pRoad1=ui->widget->newJointList(QStringList()<<"JOB2020081301_2_1"<<"DT20200912111111"<<"running");
    QTreeWidgetItem* pRoad2=ui->widget->newJointList(QStringList()<<"JOB2020081301_2_2"<<"DT20200912111111"<<"running");

    pdir1->addChild(proot);
    proot->addChild(pRoadName);
    pRoadName->addChild(pRoad1);
    pRoadName->addChild(pRoad2);


    QTreeWidgetItem* proot2=ui->widget->newJointList(QStringList()<<"DT202009121111_DTC"<<"JOB202008130000002"<<"T53");
    QTreeWidgetItem* pRoadName2=ui->widget->newJointList(QStringList()<<"通道标识"<<"任务流水号"<<"通道状态");
    QTreeWidgetItem* pRoad12=ui->widget->newJointList(QStringList()<<"JOB2020081302_2_1"<<"DT20200912111222"<<"running");
    QTreeWidgetItem* pRoad22=ui->widget->newJointList(QStringList()<<"JOB2020081302_2_2"<<"DT20200912111222"<<"running");

    pdir1->addChild(proot2);
    proot2->addChild(pRoadName2);
    pRoadName2->addChild(pRoad12);
    pRoadName2->addChild(pRoad22);

    //    ui->widget->getAllItems();
    ui->widget->getRoot();


    QAction* pbutCopyTaskNum=ui->widget->newAction("copyTaskNum");
    QAction* pbutCopyJOBID=ui->widget->newAction("copyJobID");
*/
class xTreeWidget : public QWidget
{
    Q_OBJECT

public:
    QFont font;//全局字体
    explicit xTreeWidget(QWidget *parent = nullptr);
    ~xTreeWidget();

    void deleteRootItem(QTreeWidgetItem* pitem);

    void clear();
    QAction *newAction(QString name);
    void collapseItem(QTreeWidgetItem* pitem);
    void collapseAll();
    void expandItem(QTreeWidgetItem* pitem);
    void expandAll();

    void setHeaderText(QStringList listtext);
    void setColWidth(int col,int width);
    QTreeWidgetItem* newJointList(QStringList listname);
    QTreeWidgetItem* addRootList(QStringList listname);
    QTreeWidgetItem* newJoint(QString name);
    QTreeWidgetItem* addRoot(QString name);
    QVector<QTreeWidgetItem*> getAllItems();
    //获取当前节点在本层树中的索引
    int getIndexOfCurrentLevel();
    //获取节点的所有子节点
    QVector<QTreeWidgetItem*> getChilds(QTreeWidgetItem* pItemCurrent);
    //获取当前节点的所有子节点
    QVector<QTreeWidgetItem*> getChildsNow();
    //获取节点的兄弟节点(包括自己)也就是当前level的所有节点
    QVector<QTreeWidgetItem*> getSiblings(QTreeWidgetItem* pItemCurrent);
    //获取当前节点的兄弟节点(包括自己)也就是当前level的所有节点
    QVector<QTreeWidgetItem*> getSiblingsNow();

    //获取所有root节点
    QVector<QTreeWidgetItem *> getRoot();

    //弹出信息提示框
    void messageBox(QString strDefault);
    //弹出输入框,使用方法如下
    //QString strinput;
    //if(qlib::inputbox(strinput))
    //{
    //    qlog(strinput);
    //}
    //支持中文确定和取消了
    bool inputbox(QString &strInput, QString strdefault="");
signals:
    void sigShowMaxIndex(QString url);
    void sigClickString(QString text,int column);
    void sigClick(QTreeWidgetItem *item, int column);
    void sigDoubleClick(QTreeWidgetItem *item, int column);
private slots:
    //右键菜单相关,右键菜单在哪个地方用click事件全局获取
    void on_stuTableWidget_customContextMenuRequested(QPoint pos);
    void sortByColumn(int col);
    void popMenu(const QPoint &);
    void itemClick(QTreeWidgetItem *item, int column);
    void itemDoubleClick(QTreeWidgetItem *item, int column);
    void slotDeleteItem();
    void slotAddItem();
    void slotAddItemAfter();
    void slotItemUpdate();
private:
    bool bSortUporDown;
    //弹出菜单
    QMenu *pop_menu;
    QPoint startPos;
    Ui::xTreeWidget *ui;
    bool isCheckable;//是否可选
};

#endif // XTREEWIDGET_H
