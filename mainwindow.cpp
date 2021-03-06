﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //界面上设置不了的都代码设置字体
    font.setFamily(QStringLiteral("DejaVu Sans Mono"));
    font.setPointSize(12);
    ui->label->setFont(font);
    ui->label_3->setFont(font);
    ui->label_2->setFont(font);
    ui->pcomboxStation->setFont(font);
    ui->pcomboxSystem->setFont(font);
    ui->pcomboxTaskState->setFont(font);
    ui->pcomboxTaskState->addItem("正在执行");
    ui->pcomboxTaskState->addItem("所有任务");
    ui->pcomboxTaskState->setCurrentText("正在执行");
    //设置左侧导航树控件,以及下拉系统
    for(int i=0;i<gall.size();i++)
    {
        CSGLSystem sys=gall.getSystemByIndex(i);
        hlog(sys);
        ui->pcomboxSystem->addItem(sys.strName.c_str());
        auto prooti=ui->ptreel->addRoot(sys.strName.c_str());
        for(int j=0;j<sys.size();j++)
        {
            CSGLStation stationj=sys.getStationByIndex(j);
            auto jointj=ui->ptreel->newJoint(stationj.strName.c_str());
            prooti->addChild(jointj);
        }
    }
    ui->ptreel->expandAll();
    //设置默认显示空基系统
    ui->pcomboxSystem->setCurrentText("空基系统");
    //设置右侧任务树控件
    ui->ptreer->setHeaderText(
                QStringList()
                <<"任务流水号"
                <<"作业任务编号"
                <<"作业模式"
                <<"数据源"
                <<"优先级"
                <<"卫星名称"
                <<"预计开始时间");
    ui->ptreer->setColWidth(0,310);
    ui->ptreer->setColWidth(1,200);
    ui->ptreer->setColWidth(2,100);
    ui->ptreer->setColWidth(3,80);
    ui->ptreer->setColWidth(4,80);
    ui->ptreer->setColWidth(5,120);

    //右键
    connect(ui->ptreer->newAction("复制任务流水号",1),&QAction::triggered,[this](){
        qlib::setCopyInClipboard(ui->ptreer->getCurrentItem()->text(0));
    });
    connect(ui->ptreer->newAction("复制作业任务编号",1),&QAction::triggered,[this](){
        qlib::setCopyInClipboard(ui->ptreer->getCurrentItem()->text(1));
    });
    connect(ui->ptreer->newAction("复制通道号",3),&QAction::triggered,[this](){
        qlib::setCopyInClipboard(ui->ptreer->getCurrentItem()->text(0));
    });
    connect(ui->ptreer->newAction("展开所有"),&QAction::triggered,[this](){ui->ptreer->expandAll();});
    connect(ui->ptreer->newAction("收缩所有"),&QAction::triggered,[this](){ui->ptreer->collapseAll();});
    connect(ui->ptreel->newAction("SSH远程",1),&QAction::triggered,[this](){
        auto item=ui->ptreel->getCurrentItem();
        //获取站名
        pstring strNameStation=(item->text(0)).toStdString();
        //获取系统名
        pstring strNameSystem=item->parent()->text(0).toStdString();
        //ip地址
        pstring strHost=gall.getHostBySystemAndStationName(strNameSystem,strNameStation);
        hlog(strNameSystem,strNameStation,strHost);
        //线程putty
        plib::sshzkyThread(strHost);
    });

    //tcp连接11.6服务器
    ptc=new ptcp(host,port);
    if(ptc->connectx()<0)
    {
        qlib::messageBox("连接服务器失败,请检查服务器设置");
        exit(0);
    }


    //左键点击系统或站点响应
    connect(ui->ptreel,SIGNAL(sigClick(QTreeWidgetItem*,int)),this,SLOT(slotClickTreeLeftItem(QTreeWidgetItem*,int)));
    qRegisterMetaType<pmap<pstring,plist<TASK_STATUS_STRUCT>>>("pmap<pstring,plist<TASK_STATUS_STRUCT>>");
    qRegisterMetaType<plist<TASK_STATUS_STRUCT>>("plist<TASK_STATUS_STRUCT>");
    connect(this,SIGNAL(sigShowTaskForTaskNumParent(pmap<pstring,plist<TASK_STATUS_STRUCT> >)),
            this,SLOT(slotShowTaskForTaskNumParent(pmap<pstring,plist<TASK_STATUS_STRUCT> >)));
    connect(this,SIGNAL(sigShowTask(plist<TASK_STATUS_STRUCT>)),
            this,SLOT(slotShowTask(plist<TASK_STATUS_STRUCT>)));
    //connect三个参数,第三个参数直接lambda表达式,不用单独整一个槽
    connect(this,&MainWindow::sigMessageBox,[](pstring info){qlib::messageBox(info.c_str());});

    //定时获取所有任务,直接lambuda表达式
    //    std::thread(&MainWindow::threadGetTasksAll,this).detach();
    std::thread([this]{
        while(1)
        {
            //        threadGetTasks(pliststring()<<"172.16.174.2");
            if(ptc->sendx("getMapSystemTask",host)<0)
            {
                sigMessageBox("连接服务器"+(host)+"失败,请重试");
                hlog(pstring()<<"connect host "<<host<<"fail,retrying...");
                plib::sleep(1000);
                continue;
            }
            presult result=ptc->recvz(5);
            if(result.res<0)
            {
                sigMessageBox("连接服务器"+(host)+"失败,请重试");
                hlog(pstring()<<"connect host "<<host<<"fail,retrying...");
                plib::sleep(1000);
                continue;
            }
            pmap<pstring,plist<TASK_STATUS_STRUCT> > maptaskSystem=(ptc->getClassFromPresult<pmap<pstring,plist<TASK_STATUS_STRUCT> > >(result));
            //        hlog(maptaskSystem.size());
            //根据下拉框决定显示哪个系统
            //        hlog(plib::toChinese(ui->pcomboxSystem->currentText().toStdString()));
            plist<TASK_STATUS_STRUCT> lres=maptaskSystem[ui->pcomboxSystem->currentText().toStdString()];
            //带任务单的
            //            //最终所有列表,如果是单个host就是单个的host任务,如果是系统,则系统内所有站任务都放到一起
            //            //要根据任务流水号区分任务单流水号
            //            pset<pstring> setTaskNumParent;
            //            //    hlog(lres.size());
            //            for(int i=0;i<lres.size();i++)
            //            {
            //                pstring strTaskNumParent=pstring(lres[i].arrchTaskSerialNumber).split("_")[0];
            //                //        hlog(strTaskNumParent);
            //                setTaskNumParent.append(strTaskNumParent);
            //            }
            //            //    hlog(setTaskNumParent);
            //            //    for(int i=0;i<setTaskNumParent.size();i++)
            //            //    {
            //            //        hlog(setTaskNumParent[i]);
            //            //    }
            //            //map,任务单流水号和任务的对应
            //            pmap<pstring,plist<TASK_STATUS_STRUCT>> mapTask;
            //            for(int i=0;i<setTaskNumParent.size();i++)
            //            {
            //                plist<TASK_STATUS_STRUCT> ltaski;
            //                pstring strTaskNumParent=setTaskNumParent[i];
            //                for(int j=0;j<lres.size();j++)
            //                {
            //                    pstring strTaskNumParentj=pstring(lres[j].arrchTaskSerialNumber).split("_")[0];
            //                    if(strTaskNumParentj==strTaskNumParent)
            //                        ltaski.append(lres[j]);
            //                }
            //                mapTask.add(strTaskNumParent,ltaski);
            //            }
            //            //要在这里过滤一下,当前运行的任务和还没运行的任务
            //            if(ui->pcomboxTaskState->currentText()=="正在执行")
            //            {
            //                pmap<pstring,plist<TASK_STATUS_STRUCT>> mapTaskCopy=mapTask;
            //                mapTask.clear();
            //                for(int i=0;i<mapTaskCopy.size();i++)
            //                {
            //                    pstring strTaskNumParent=mapTaskCopy.getKey(i);
            //                    plist<TASK_STATUS_STRUCT> ltask=mapTaskCopy[strTaskNumParent];
            //                    //如果有一个任务在执行,那么久最终显示这个,加到显示列表中
            //                    bool brunning=false;
            //                    for(int j=0;j<ltask.size();j++)
            //                    {
            //                        if(ltask[j].shInexecutingState==5)
            //                        {
            //                            brunning=true;
            //                            break;
            //                        }
            //                    }
            //                    if(brunning)
            //                    {
            //                        mapTask.add(strTaskNumParent,ltask);
            //                    }
            //                }
            //            }

            //            sigShowTaskForTaskNumParent(mapTask);
            //不带任务单的
            if(ui->pcomboxTaskState->currentText()=="正在执行")
            {
                plist<TASK_STATUS_STRUCT> lcp=lres;
                lres.clear();
                for(int i=0;i<lcp.size();i++)
                {
                    if(lcp[i].shInexecutingState==5||lcp[i].shInexecutingState==9)
                        lres.append(lcp[i]);
                }
            }
            sigShowTask(lres);
            plib::sleep(1000);
        }
    }).detach();

}

void MainWindow::slotShowTaskForTaskNumParent(pmap<pstring, plist<TASK_STATUS_STRUCT> > mapTasks)
{
    for(int i=0;i<mapTasks.size();i++)
    {
        //        hlog(mapTasks.getKey(i),mapTasks.getValue(i).size());
        //任务单流水号
        pstring strTaskNumParent=mapTasks.getKey(i);
        plist<TASK_STATUS_STRUCT> ltaski=mapTasks.getValue(i);
        //遍历树的根节点,先找所有root节点任务单流水号看有没有已经有的,有的更新,没有的删除
        QVector<QTreeWidgetItem*> listroot= ui->ptreer->getRoot();
        auto it=std::find_if(listroot.begin(),listroot.end(),[strTaskNumParent](QTreeWidgetItem* pItem){
                return pstring(pItem->text(0).toStdString())==strTaskNumParent;
    });
        //找不到任务单流水号,则新建
        if(it==listroot.end())
        {
            //添加根节点,任务单流水号
            auto jointRooti=ui->ptreer->addRootList(QStringList()<<strTaskNumParent.c_str());
            //            jointRooti->setTextColor(0,QColor("blue"));
            for(int j=0;j<ltaski.size();j++)
            {
                TASK_STATUS_STRUCT taskj=ltaski[j];
                //添加每个任务的信息
                auto jointTaskj=ui->ptreer->newJointList(
                            QStringList()
                            <<taskj.arrchTaskSerialNumber
                            <<taskj.arrchJobTaskID
                            <<taskj.arrchTaskMode
                            <<taskj.arrchDataSource
                            <<toStringYXJ(taskj.shTaskPriority)
                            <<taskj.arrchSatelliteName
                            <<taskj.arrchForeseeStartTime);
                //                jointTaskj->setTextColor(0,QColor("yellow"));
                //添加通道标识
                auto jointRoadHeader=ui->ptreer->newJointList(
                            QStringList()<<"通道标识"<<"通道状态"<<"传输百分比"
                            <<"分配带宽"<<"实际带宽");

                //通道数大于0,有通道执行才显示表头
                if(taskj.iRoadNumber>0)
                    jointTaskj->addChild(jointRoadHeader);
                //添加每个通道
                for(int k=0;k<taskj.iRoadNumber;k++)
                {
                    ROAD_STATUS_STRUCT road=taskj.arrstruRoadStatus[k];
                    auto jointRoad=ui->ptreer->newJointList(
                                QStringList()
                                <<road.arrchRoadNumber
                                <<toStringState(road.shRoadState)
                                <<qlib::toString(road.shFinishPercent)
                                <<qlib::toString(road.iCompressRatio)
                                <<qlib::toString(road.iTransferRate));
                    jointRoad->setTextColor(1,QColor("red"));
                    //                    jointRoad->setTextColor(2,QColor("red"));
                    //要先加入树再建QProgress,不然不管用
                    jointRoadHeader->addChild(jointRoad);
                    QProgressBar* pbar=new QProgressBar(this);
                    pbar->setMinimum(0);
                    pbar->setMaximum(100);
                    pbar->setValue(road.shFinishPercent);
                    pbar->setFont(font);
                    ui->ptreer->setItemWidget(jointRoad,2,pbar);

                }
                jointRooti->addChild(jointTaskj);
            }
        }
        else
        {
            //找到了,则更新,先获取这个根节点--也就是任务单流水号
            //            QTreeWidgetItem* prootFind=listroot[ifind];
            QTreeWidgetItem* prootFind=*it;
            //遍历每个任务
            for(int j=0;j<ltaski.size();j++)
            {
                TASK_STATUS_STRUCT taskj=ltaski[j];
                pstring strTaskNum=taskj.arrchTaskSerialNumber;
                //看任务在该任务单流水号内是否有
                int ifind2=-1;
                for(int k=0;k<prootFind->childCount();k++)
                {
                    pstring strTaskNumFromTree=prootFind->child(k)->text(0).toStdString();
                    if(strTaskNumFromTree==strTaskNum)
                    {
                        ifind2=k;
                        break;
                    }
                }
                //没有找到这个任务,则新建
                if(ifind2<0)
                {
                    //添加每个任务的信息
                    auto jointTaskj=ui->ptreer->newJointList(
                                QStringList()
                                <<taskj.arrchTaskSerialNumber
                                <<taskj.arrchJobTaskID
                                <<taskj.arrchTaskMode
                                <<taskj.arrchDataSource
                                <<toStringYXJ(taskj.shTaskPriority)
                                <<taskj.arrchSatelliteName
                                <<taskj.arrchForeseeStartTime);
                    //添加通道标识
                    auto jointRoadHeader=ui->ptreer->newJointList(
                                QStringList()<<"通道标识"<<"通道状态"<<"传输百分比");
                    //通道数大于0,有通道执行才显示表头
                    if(taskj.iRoadNumber>0)
                        jointTaskj->addChild(jointRoadHeader);
                    //添加每个通道
                    for(int k=0;k<taskj.iRoadNumber;k++)
                    {
                        ROAD_STATUS_STRUCT road=taskj.arrstruRoadStatus[k];
                        auto jointRoad=ui->ptreer->newJointList(
                                    QStringList()
                                    <<road.arrchRoadNumber
                                    <<toStringState(road.shRoadState)
                                    <<qlib::toString(road.shFinishPercent)
                                    <<qlib::toString(road.iCompressRatio)
                                    <<qlib::toString(road.iTransferRate));
                        jointRoad->setTextColor(1,QColor("red"));
                        //获取进度条
                        QProgressBar* pbar=(QProgressBar*)ui->ptreer->getItemWidget(jointRoad,2);
                        //                        hlog(pbar==NULL);
                        if(pbar!=NULL)
                            pbar->setValue(road.shFinishPercent);
                        jointRoadHeader->addChild(jointRoad);
                    }
                    prootFind->addChild(jointTaskj);
                }
                else
                {
                    //找到了这个任务,则更新任务
                    QTreeWidgetItem* pItem2Find=prootFind->child(ifind2);
                    pItem2Find->setText(0,taskj.arrchTaskSerialNumber);
                    pItem2Find->setText(1,taskj.arrchJobTaskID);
                    pItem2Find->setText(2,taskj.arrchTaskMode);
                    pItem2Find->setText(3,taskj.arrchDataSource);
                    pItem2Find->setText(4,toStringYXJ(taskj.shTaskPriority));
                    pItem2Find->setText(5,taskj.arrchSatelliteName);
                    pItem2Find->setText(6,taskj.arrchForeseeStartTime);
                    //找通道表头
                    QTreeWidgetItem* pheadRoad=NULL;
                    //如果通道数大于0,则通道表头找到,否则就是null
                    if(pItem2Find->childCount()>0)
                        pheadRoad=pItem2Find->child(0);
                    if(pheadRoad!=NULL)
                    {
                        //通道表头不用更新了,只更新通道
                        for(int t=0;t<taskj.iRoadNumber;t++)
                        {
                            ROAD_STATUS_STRUCT road=taskj.arrstruRoadStatus[t];
                            pstring strRoadNum=road.arrchRoadNumber;
                            int ifind3=-1;
                            //找这个通道在不在这个任务里
                            for(int s=0;s<pheadRoad->childCount();s++)
                            {
                                QTreeWidgetItem* pitem3=pheadRoad->child(s);
                                if(pstring(pitem3->text(0).toStdString())==strRoadNum)
                                {
                                    ifind3=s;
                                    break;
                                }
                            }
                            //找不到该通道,则新建通道
                            if(ifind3<0)
                            {
                                auto jointRoad=ui->ptreer->newJointList(
                                            QStringList()
                                            <<road.arrchRoadNumber
                                            <<toStringState(road.shRoadState)
                                            <<qlib::toString(road.shFinishPercent)
                                            <<qlib::toString(road.iCompressRatio)
                                            <<qlib::toString(road.iTransferRate));
                                jointRoad->setTextColor(1,QColor("red"));
                                //获取进度条
                                QProgressBar* pbar=(QProgressBar*)ui->ptreer->getItemWidget(jointRoad,2);
                                //                                hlog(pbar==NULL);
                                if(pbar!=NULL)
                                    pbar->setValue(road.shFinishPercent);
                                pheadRoad->addChild(jointRoad);
                            }
                            else
                            {
                                //找到该通道,直接更新通道
                                QTreeWidgetItem* pItemRoad=pheadRoad->child(ifind3);
                                pItemRoad->setText(0,road.arrchRoadNumber);
                                pItemRoad->setText(1,toStringState(road.shRoadState));
                                pItemRoad->setTextColor(1,QColor("red"));
                                pItemRoad->setText(2,qlib::toString(road.shFinishPercent));
                                pItemRoad->setText(3,qlib::toString(road.iCompressRatio));
                                pItemRoad->setText(4,qlib::toString(road.iTransferRate));
                                //获取进度条
                                QProgressBar* pbar=(QProgressBar*)ui->ptreer->getItemWidget(pItemRoad,2);
                                //                                hlog(pbar==NULL);
                                if(pbar!=NULL)
                                    pbar->setValue(road.shFinishPercent);
                            }
                        }
                    }
                }
            }
        }
    }

    //比对一下当前树上的任务单,如果有task里面没有的,就要删掉,说明完成了
    //先找所有root节点任务单流水号看有没有已经有的,有的更新,没有的删除
    //只有当任务单里的所有任务都完成,任务单都没有的时候,才删除,便于查看任务单里的所有任务情况
    //删除的是整个任务单,而通道,任务都不删,便于观察情况
    QVector<QTreeWidgetItem*> listroot= ui->ptreer->getRoot();
    //先找到树上比task多的个数
    int countForReduce=0;
    for(int i=0;i<listroot.size();i++)
    {
        QTreeWidgetItem* prooti=listroot[i];
        pstring strTaskNumInTree=prooti->text(0).toStdString();
        int ifind=-1;
        for(int j=0;j<mapTasks.size();j++)
        {
            //任务单流水号
            pstring strTaskNumParent=mapTasks.getKey(j);
            if(strTaskNumInTree==strTaskNumParent)
            {
                ifind=j;
                break;
            }
        }
        //没找到,说明已经无效了,要删除
        if(ifind<0)
        {
            countForReduce++;
        }
    }
    //    hlog(countForReduce);
    //然后一起删除
    for(int k=0;k<countForReduce;k++)
    {
        //        hlog(k);
        for(int i=0;i<ui->ptreer->getRoot().size();i++)
        {
            QTreeWidgetItem* prooti=ui->ptreer->getRoot()[i];
            pstring strTaskNumInTree=prooti->text(0).toStdString();
            int ifind=-1;
            for(int j=0;j<mapTasks.size();j++)
            {
                //任务单流水号
                pstring strTaskNumParent=mapTasks.getKey(j);
                if(strTaskNumInTree==strTaskNumParent)
                {
                    ifind=j;
                    break;
                }
            }
            //没找到,说明已经无效了,要删除
            if(ifind<0)
            {
                //                            hlog(strTaskNumInTree);
                ui->ptreer->deleteRootItem(prooti);
                break;
            }
        }
    }


    //    //先清空再重新整
    //    ui->ptreer->clear();
    //    for(int i=0;i<mapTasks.size();i++)
    //    {
    //        hlog(mapTasks.getKey(i),mapTasks.getValue(i).size());
    //        pstring strTaskNumParent=mapTasks.getKey(i);
    //        plist<TASK_STATUS_STRUCT> ltaski=mapTasks.getValue(i);

    //        //添加根节点,任务单流水号
    //        auto jointRooti=ui->ptreer->addRootList(QStringList()<<strTaskNumParent.c_str());
    //        for(int j=0;j<ltaski.size();j++)
    //        {
    //            TASK_STATUS_STRUCT taskj=ltaski[j];
    //            //添加每个任务的信息
    //            auto jointTaskj=ui->ptreer->newJointList(
    //                        QStringList()<<taskj.arrchTaskSerialNumber<<taskj.arrchJobTaskID
    //                        <<taskj.arrchTaskMode);
    //            //添加通道标识
    //            auto jointRoadHeader=ui->ptreer->newJointList(
    //                        QStringList()<<"通道标识"<<"通道状态"<<"传输百分比");
    //            //通道数大于0,有通道执行才显示表头
    //            if(taskj.iRoadNumber>0)
    //                jointTaskj->addChild(jointRoadHeader);
    //            //添加每个通道
    //            for(int k=0;k<taskj.iRoadNumber;k++)
    //            {
    //                ROAD_STATUS_STRUCT road=taskj.arrstruRoadStatus[k];
    //                auto jointRoad=ui->ptreer->newJointList(
    //                            QStringList()<<road.arrchRoadNumber<<qlib::toString(road.shRoadState)
    //                            <<qlib::toString(road.shRoadState));
    //                jointTaskj->addChild(jointRoad);
    //            }
    //            jointRooti->addChild(jointTaskj);
    //        }
    //    }
}

void MainWindow::slotShowTask(plist<TASK_STATUS_STRUCT> mapTasks)
{
    for(int i=0;i<mapTasks.size();i++)
    {
        //任务单流水号
        TASK_STATUS_STRUCT taskj=mapTasks[i];
        //遍历树的根节点,先找所有root节点任务单流水号看有没有已经有的,有的更新,没有的删除
        QVector<QTreeWidgetItem*> listroot= ui->ptreer->getRoot();
        pstring strTaskNum=taskj.arrchTaskSerialNumber;
        auto it=std::find_if(listroot.begin(),listroot.end(),[strTaskNum](QTreeWidgetItem* pItem){
                return pstring(pItem->text(0).toStdString())==pstring(strTaskNum);
        });
        //找不到任务单流水号,则新建
        if(it==listroot.end())
        {
            //添加根节点,任务单流水号
            //添加每个任务的信息
            auto jointTaskj=ui->ptreer->addRootList(
                        QStringList()
                        <<taskj.arrchTaskSerialNumber
                        <<taskj.arrchJobTaskID
                        <<taskj.arrchTaskMode
                        <<taskj.arrchDataSource
                        <<toStringYXJ(taskj.shTaskPriority)
                        <<taskj.arrchSatelliteName
                        <<taskj.arrchForeseeStartTime);
            //                jointTaskj->setTextColor(0,QColor("yellow"));
            //添加通道标识
            auto jointRoadHeader=ui->ptreer->newJointList(
                        QStringList()<<"通道标识"<<"通道状态"<<"传输百分比"
                        <<"分配带宽"<<"实际带宽");

            //通道数大于0,有通道执行才显示表头
            if(taskj.iRoadNumber>0)
                jointTaskj->addChild(jointRoadHeader);
            //添加每个通道
            for(int k=0;k<taskj.iRoadNumber;k++)
            {
                ROAD_STATUS_STRUCT road=taskj.arrstruRoadStatus[k];
                auto jointRoad=ui->ptreer->newJointList(
                            QStringList()
                            <<road.arrchRoadNumber
                            <<toStringState(road.shRoadState)
                            <<qlib::toString(road.shFinishPercent)
                            <<qlib::toString(road.iCompressRatio)
                            <<qlib::toString(road.iTransferRate));
                jointRoad->setTextColor(1,QColor("red"));
                //                    jointRoad->setTextColor(2,QColor("red"));
                //要先加入树再建QProgress,不然不管用
                jointRoadHeader->addChild(jointRoad);
                QProgressBar* pbar=new QProgressBar(this);
                pbar->setMinimum(0);
                pbar->setMaximum(100);
                pbar->setValue(road.shFinishPercent);
                pbar->setFont(font);
                ui->ptreer->setItemWidget(jointRoad,2,pbar);
            }
        }
        else
        {
            //找到了,则更新,先获取这个根节点--也就是任务单流水号
            //            QTreeWidgetItem* prootFind=listroot[ifind];
            QTreeWidgetItem* prootFind=*it;
            //找到了这个任务,则更新任务
            QTreeWidgetItem* pItem2Find=prootFind;
            pItem2Find->setText(0,taskj.arrchTaskSerialNumber);
            pItem2Find->setText(1,taskj.arrchJobTaskID);
            pItem2Find->setText(2,taskj.arrchTaskMode);
            pItem2Find->setText(3,taskj.arrchDataSource);
            pItem2Find->setText(4,toStringYXJ(taskj.shTaskPriority));
            pItem2Find->setText(5,taskj.arrchSatelliteName);
            pItem2Find->setText(6,taskj.arrchForeseeStartTime);
            //找通道表头
            QTreeWidgetItem* pheadRoad=NULL;
            //如果通道数大于0,则通道表头找到,否则就是null
            if(pItem2Find->childCount()>0)
                pheadRoad=pItem2Find->child(0);
            if(pheadRoad!=NULL)
            {
                //通道表头不用更新了,只更新通道
                for(int t=0;t<taskj.iRoadNumber;t++)
                {
                    ROAD_STATUS_STRUCT road=taskj.arrstruRoadStatus[t];
                    pstring strRoadNum=road.arrchRoadNumber;
                    int ifind3=-1;
                    //找这个通道在不在这个任务里
                    for(int s=0;s<pheadRoad->childCount();s++)
                    {
                        QTreeWidgetItem* pitem3=pheadRoad->child(s);
                        if(pstring(pitem3->text(0).toStdString())==strRoadNum)
                        {
                            ifind3=s;
                            break;
                        }
                    }
                    //找不到该通道,则新建通道
                    if(ifind3<0)
                    {
                        auto jointRoad=ui->ptreer->newJointList(
                                    QStringList()
                                    <<road.arrchRoadNumber
                                    <<toStringState(road.shRoadState)
                                    <<qlib::toString(road.shFinishPercent)
                                    <<qlib::toString(road.iCompressRatio)
                                    <<qlib::toString(road.iTransferRate));
                        jointRoad->setTextColor(1,QColor("red"));
                        //获取进度条
                        QProgressBar* pbar=(QProgressBar*)ui->ptreer->getItemWidget(jointRoad,2);
                        //                                hlog(pbar==NULL);
                        if(pbar!=NULL)
                            pbar->setValue(road.shFinishPercent);
                        pheadRoad->addChild(jointRoad);
                    }
                    else
                    {
                        //找到该通道,直接更新通道
                        QTreeWidgetItem* pItemRoad=pheadRoad->child(ifind3);
                        pItemRoad->setText(0,road.arrchRoadNumber);
                        pItemRoad->setText(1,toStringState(road.shRoadState));
                        pItemRoad->setTextColor(1,QColor("red"));
                        pItemRoad->setText(2,qlib::toString(road.shFinishPercent));
                        pItemRoad->setText(3,qlib::toString(road.iCompressRatio));
                        pItemRoad->setText(4,qlib::toString(road.iTransferRate));
                        //获取进度条
                        QProgressBar* pbar=(QProgressBar*)ui->ptreer->getItemWidget(pItemRoad,2);
                        //                                hlog(pbar==NULL);
                        if(pbar!=NULL)
                            pbar->setValue(road.shFinishPercent);
                    }
                }
            }
        }

    }
    //比对一下当前树上的任务单,如果有task里面没有的,就要删掉,说明完成了
    //先找所有root节点任务单流水号看有没有已经有的,有的更新,没有的删除
    //只有当任务单里的所有任务都完成,任务单都没有的时候,才删除,便于查看任务单里的所有任务情况
    //删除的是整个任务单,而通道,任务都不删,便于观察情况
    QVector<QTreeWidgetItem*> listroot= ui->ptreer->getRoot();
    //先找到树上比task多的个数
    int countForReduce=0;
    for(int i=0;i<listroot.size();i++)
    {
        QTreeWidgetItem* prooti=listroot[i];
        pstring strTaskNumInTree=prooti->text(0).toStdString();
        int ifind=-1;
        for(int j=0;j<mapTasks.size();j++)
        {
            //任务单流水号
            pstring strTaskNumParent=mapTasks[j].arrchTaskSerialNumber;
            if(strTaskNumInTree==strTaskNumParent)
            {
                ifind=j;
                break;
            }
        }
        //没找到,说明已经无效了,要删除
        if(ifind<0)
        {
            countForReduce++;
        }
    }
    //    hlog(countForReduce);
    //然后一起删除
    for(int k=0;k<countForReduce;k++)
    {
        //        hlog(k);
        for(int i=0;i<ui->ptreer->getRoot().size();i++)
        {
            QTreeWidgetItem* prooti=ui->ptreer->getRoot()[i];
            pstring strTaskNumInTree=prooti->text(0).toStdString();
            int ifind=-1;
            for(int j=0;j<mapTasks.size();j++)
            {
                //任务单流水号
                pstring strTaskNumParent=mapTasks[j].arrchTaskSerialNumber;
                if(strTaskNumInTree==strTaskNumParent)
                {
                    ifind=j;
                    break;
                }
            }
            //没找到,说明已经无效了,要删除
            if(ifind<0)
            {
                //                            hlog(strTaskNumInTree);
                ui->ptreer->deleteRootItem(prooti);
                break;
            }
        }
    }
}

QString MainWindow::toStringState(int state)
{
    if(state==5)
        return "正在执行";
    else if(state==9)
        return "完成";
    else if(state==10)
        return "失败";
    else
        return "未知";
}

QString MainWindow::toStringYXJ(int yxj)
{
    if(yxj==0)
        return "低";
    else if(yxj==1)
        return "中";
    else if(yxj==2)
        return "高";
}

MainWindow::~MainWindow()
{
    delete ui;
}

//左键点击系统或站点响应
void MainWindow::slotClickTreeLeftItem(QTreeWidgetItem *item, int column)
{
    //用这个判断是系统还是站点
    //    hlog(item->parent()==NULL);
    //系统
    if(item->parent()==NULL)
    {
        QString system=(item->text(column));
        hlog(plib::toChinese(system.toStdString()));
        pliststring listhost=(gall.getHostsBySystemName(system.toStdString()));
        hlog(listhost.size());

    }
    else//站点,第二级
    {
        hlog(item->text(column));
        QString system=item->parent()->text(column);
        QString station=item->text(column);
        hlog(plib::toChinese(system.toStdString()),station);
        pstring host=(gall.getHostBySystemAndStationName(system.toStdString(),station.toStdString()));
        hlog(host);
    }
}


