#include "xtreewidget.h"
#include "ui_xtreewidget.h"
void xTreeWidget::setHeaderText(QStringList listtext)
{
    //隔行不一样颜色
    ui->treeWidget->setAlternatingRowColors(true);
    ui->treeWidget->setHeaderHidden(false);//隐藏上述表头
    ui->treeWidget->setHeaderLabels(listtext);

    //    ui->tableWidget->setColumnWidth(1,230);
    //    ui->tableWidget->setColumnWidth(2,100);
    //    for(int i=0;i<this->getColCount()-1;i++)
    //    {
    //        ui->tableWidget->setColumnWidth(i,300);
    //    }
    bSortUporDown=false;
    //设置列宽可拖动
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Interactive);
    //最后一列拉满
    ui->treeWidget->header()->setStretchLastSection(true);

    //设置行高可拖动,一般不用
    //     ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //直接用header好像不管用,得每个都设置
    for(int i=0;i<listtext.size();i++)
        ui->treeWidget->headerItem()->setFont(i,font);
    //除最后一列外默认200宽度
    ui->treeWidget->setColumnWidth(0,200);
}

void xTreeWidget::setColWidth(int col, int width)
{
    ui->treeWidget->setColumnWidth(col,width);
}
xTreeWidget::xTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::xTreeWidget)
{
    ui->setupUi(this);
    font.setFamily(QStringLiteral("DejaVu Sans Mono"));
    font.setPointSize(12);
    this->isCheckable=false;
    ui->treeWidget->setDropIndicatorShown(false);
    ui->treeWidget->setDragEnabled(false);
    //启用拖放相关默认关闭,不然会显示拖放图标
    //    ui->treeWidget->setDragEnabled(true);
    //    //设置显示将要被放置的位置
    //    ui->treeWidget->setDropIndicatorShown(true);
    //        //设置拖放模式为移动项目，否则为复制项目,这里是复制
    //    ui->treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
    //    ui->treeWidget->setDefaultDropAction(Qt::MoveAction);



    //设置列数,一般1就行,多个是为xml这种的可以
        ui->treeWidget->setColumnCount(1);
    //设置选中项
    ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    //为了自动滚动设置的//设置平滑一点
    ui->treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //自适应
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->treeWidget->setHeaderHidden(true);//隐藏上述表头



    //    QTreeWidgetItem*proot=this->addRoot(("根节点"));
    //    QTreeWidgetItem* pjoint2=this->newJoint(("吊车架"));
    //    QTreeWidgetItem* pjoint3=this->newJoint(("三级节点"));
    //    QTreeWidgetItem* pjoint4=this->newJoint(("钢柱"));
    //    QTreeWidgetItem* pjoint5=this->newJoint(("xlfdQProcess"));

    //    proot->insertChild(0,pjoint2);
    //    pjoint2->insertChild(0,pjoint3);
    //    proot->addChild(pjoint4);
    //    proot->insertChild(2,pjoint5);

    ////    QTreeWidgetItem* proot2=this->addRoot("根节点2");
    //    //节点始终保持展开

    //    ui->treeWidget->expandAll(); //结点全部展开


    //检测点击事件
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(itemClick(QTreeWidgetItem*,int)));
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemDoubleClick(QTreeWidgetItem*,int)));
    //右键菜单
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(const QPoint&)), this,SLOT(popMenu(const QPoint&)));

    //右键菜单
    ui->treeWidget->setContextMenuPolicy (Qt::CustomContextMenu);
    //弹出菜单
    pop_menu = new QMenu(ui->treeWidget);

    //右键菜单相关,右键菜单在哪个地方用click事件全局获取
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,
            SLOT(on_stuTableWidget_customContextMenuRequested(QPoint)));




    ui->treeWidget->setSortingEnabled(true);
    //SortIndicator为水平标题栏文字旁边的三角指示器
    //        headerGoods->setSortIndicator(0, Qt::AscendingOrder);
    ui->treeWidget->header()->setSortIndicatorShown(true);
    //        headerGoods->setClickable(true);

    connect(ui->treeWidget->header(),SIGNAL(sectionClicked(int )),this,
            SLOT(sortByColumn(int)));

}
void xTreeWidget::itemClick(QTreeWidgetItem *item, int column)
{
    sigClickString(item->text(column),column);
//    hlog(plib::toChinese(item->text(0).toStdString()));
//    QTreeWidgetItem *parent = item->parent();   //获取父节点
//    if(NULL==parent) //注意：最顶端项是没有父节点的
//        return;
//    int row = parent->indexOfChild(item); //item在父项中的节点行号(从0开始)
//    //    qDebug()<<row;
//    pstring info=plib::toGBK((pstring()<<"click "<<row<<" line").c_str());
//    hlog(info);
    sigClick(item,column);

    //获取数据从其他地方
    //    qDebug()<<item->checkState(0);
    //    QString strRtsp=(item->data(0,Qt::UserRole+1).toString());
    //    hlog(strRtsp.toStdString());
    //    DataTree dt=item->data(0,Qt::UserRole+1).value<DataTree>();
    //发出限号给显示组件
    //    sigClick(strRtsp);
}

void xTreeWidget::itemDoubleClick(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parent = item->parent();   //获取父节点
    if(NULL==parent) //注意：最顶端项是没有父节点的
        return;


    int row = parent->indexOfChild(item); //item在父项中的节点行号(从0开始)
    //        QString info=QString::fromLocal8Bit("双击了Tree控件的第")+qlib::toString(row)+QString::fromLocal8Bit("行");
    qDebug()<<row;
    //    string strinfo=string("双击了Tree控件的第")+plib::toString(row)+"行";
    //    hlog(plib::toChinese(strinfo.c_str()));
    //获取双击的索引地址和播放地址

    QString strRtsp=(item->data(0,Qt::UserRole+1).toString());
    //    hlog(strRtsp.toStdString());
    //    DataTree dt=item->data(0,Qt::UserRole+1).value<DataTree>();
    //发出限号给显示组件
    sigShowMaxIndex(strRtsp);



    //双击选中/取消选择
    if(this->isCheckable)
    {
        qDebug()<<item->checkState(0);
        if(item->checkState(0)==Qt::Checked)
        {
            item->setCheckState(0,Qt::Unchecked);
        }
        else {
            item->setCheckState(0,Qt::Checked);
        }
    }

    hlog(item->text(column));
    sigDoubleClick(item,column);
}


xTreeWidget::~xTreeWidget()
{
    delete ui;
}

void xTreeWidget::deleteRootItem(QTreeWidgetItem *pitem)
{
    int index=ui->treeWidget->indexOfTopLevelItem(pitem);
    hlog(index);
    ui->treeWidget->takeTopLevelItem(index);
}

void xTreeWidget::clear()
{
    ui->treeWidget->clear();
}

QAction* xTreeWidget::newAction(QString name)
{
    QAction* pActionNew=new QAction(name,this);
    pop_menu->addAction(name);
    pop_menu->setFont(font);
    return pActionNew;
}

void xTreeWidget::expandItem(QTreeWidgetItem *pitem)
{

    ui->treeWidget->expandItem(pitem);
}

void xTreeWidget::expandAll()
{
    ui->treeWidget->expandAll();
}



QTreeWidgetItem *xTreeWidget::newJointList(QStringList listname)
{
    //    //一般默认使用colum=1,多个的以后再试
    QTreeWidgetItem *pAdd=new QTreeWidgetItem(listname);
    for(int i=0;i<listname.size();i++)
    {
        pAdd->setFont(i,font);
        //        pAdd->setIcon(i,QIcon(":/icons/摄像头.png"));//设置图标
    }
    return pAdd;
}

QTreeWidgetItem *xTreeWidget::addRootList(QStringList listname)
{
    QTreeWidgetItem *pAdd=new QTreeWidgetItem(ui->treeWidget,listname);
    for(int i=0;i<listname.size();i++)
    {
        pAdd->setFont(i,font);
        //        pAdd->setIcon(i,QIcon(":/icons/层级.png"));//设置图标
    }
    return pAdd;
}

QTreeWidgetItem* xTreeWidget::newJoint(QString name)
{
    //    //一般默认使用colum=1,多个的以后再试
    QTreeWidgetItem *pAdd=new QTreeWidgetItem(QStringList()<<name);
    pAdd->setFont(0,font);
    pAdd->setIcon(0,QIcon(":/icons/摄像头.png"));//设置图标

    //    //就算可复选,也不包括根节点
    //    if(pItemParent!=NULL&&this->isCheckable)
    //    {
    //        pAdd->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    //        pAdd->setCheckState(0, Qt::Checked);
    //    }

    return pAdd;
}

QTreeWidgetItem* xTreeWidget::addRoot(QString name)
{
    QTreeWidgetItem *pAdd=new QTreeWidgetItem(ui->treeWidget,QStringList()<<name);
    pAdd->setFont(0,font);
    pAdd->setIcon(0,QIcon(":/icons/层级.png"));//设置图标
    return pAdd;
}

QVector<QTreeWidgetItem *> xTreeWidget::getAllItems()
{
    QVector<QTreeWidgetItem *>res;
    //遍历treeWidget
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        //do something like
        QString str=(*it)->text(0);
        //        qDebug()<<str;
        //通过parent是否是NULL判断是否是根节点
//        hlog(plib::toChinese(str.toStdString()),(*it)->parent()==NULL);
        res.push_back(*it);
        ++it;
    }
    return res;
}

int xTreeWidget::getIndexOfCurrentLevel()
{
    QTreeWidgetItem* pItemCurrent=ui->treeWidget->currentItem();
    int index=pItemCurrent->parent()->indexOfChild(pItemCurrent);
    return index;
}

QVector<QTreeWidgetItem *> xTreeWidget::getChilds(QTreeWidgetItem *pItemCurrent)
{
    QVector<QTreeWidgetItem *>res;
    for(int i=0;i<pItemCurrent->childCount();i++)
    {
        //        qDebug()<<"ri: "<<pItemCurrent->child(i)->text(0)<<" end";
        res.append(pItemCurrent->child(i));

    }
    return res;
}

QVector<QTreeWidgetItem *> xTreeWidget::getChildsNow()
{
    return this->getChilds(ui->treeWidget->currentItem());
}

QVector<QTreeWidgetItem *> xTreeWidget::getSiblings(QTreeWidgetItem *pItemCurrent)
{
    return this->getChilds(pItemCurrent->parent());
}

QVector<QTreeWidgetItem *> xTreeWidget::getSiblingsNow()
{
    return this->getSiblings(ui->treeWidget->currentItem());
}

QVector<QTreeWidgetItem*> xTreeWidget::getRoot()
{
    QVector<QTreeWidgetItem*> listroot;
    QVector<QTreeWidgetItem*> listall=this->getAllItems();
    for(int i=0;i<listall.size();i++)
    {
        QTreeWidgetItem* pitem=listall[i];
        //父节点是空,则说明是root节点
        if(pitem->parent()==NULL)
        {
            listroot.append(pitem);
//            hlog(pitem->text(0));
        }
    }
    return listroot;
}



void xTreeWidget::popMenu(const QPoint &)
{
    QTreeWidgetItem* curItem=ui->treeWidget->currentItem();  //**获取当前被点击的节点
    if(curItem == NULL || curItem->parent() == NULL)
        return;           //右键的位置在空白位置右击或者点击的是顶层item

    qDebug()<<curItem->text(0);


    //创建一个action
    QAction actionDeleteItem(QString::fromLocal8Bit("&删除节点"),this);
    connect(&actionDeleteItem, SIGNAL(triggered()), this, SLOT(slotDeleteItem()));

    QAction actionAddItem(QString::fromLocal8Bit("&在当前节点前添加节点"),this);
    connect(&actionAddItem, SIGNAL(triggered()), this, SLOT(slotAddItem()));

    QAction actionAddItemAfter(QString::fromLocal8Bit("&在当前节点后添加节点"),this);
    connect(&actionAddItemAfter, SIGNAL(triggered()), this, SLOT(slotAddItemAfter()));

    QAction actionItemUpdate(QString::fromLocal8Bit("&修改节点"),this);
    connect(&actionItemUpdate, SIGNAL(triggered()), this, SLOT(slotItemUpdate()));

    //    //创建一个菜单栏
    //    QMenu menu(ui->treeWidget);

    //    menu.addAction(&actionAddItem);
    //    menu.addAction(&actionAddItemAfter);
    //    menu.addAction(&actionItemUpdate);
    //    menu.addAction(&actionDeleteItem);

    //    menu.exec(QCursor::pos());  //在当前鼠标位置显示
}




void xTreeWidget::slotDeleteItem()
{
    QTreeWidgetItem* curItem=ui->treeWidget->currentItem();
    curItem->parent()->removeChild(ui->treeWidget->currentItem());
}

void xTreeWidget::slotAddItem()
{
    QString strInput;
    if(inputbox(strInput))
    {
        //获取所有兄弟节点
        QVector<QTreeWidgetItem*> listNowLevel=this->getSiblingsNow();
        //        qlog(listNowLevel.size());
        //text不能重复
        bool isRepeat=false;
        for(int i=0;i<listNowLevel.size();i++)
        {
            QTreeWidgetItem* pitem=listNowLevel[i];
            if(pitem->text(0)==strInput)
            {
                isRepeat=true;
                break;
            }
        }

        if(isRepeat)
        {
            messageBox(QString::fromLocal8Bit(
                           "当前层中已有该名称的节点,无法添加 [")+strInput+"]");
            return;
        }

        //获取当前级中的index
        int index=this->getIndexOfCurrentLevel();
        //        qlog(index);
        QTreeWidgetItem* pItemCurrent=ui->treeWidget->currentItem();
        QTreeWidgetItem* pNewJoint=this->newJoint(strInput);
        pItemCurrent->parent()->insertChild(index,pNewJoint);
    }
}

void xTreeWidget::slotAddItemAfter()
{
    QString strInput;
    if(inputbox(strInput))
    {
        //获取所有兄弟节点
        QVector<QTreeWidgetItem*> listNowLevel=this->getSiblingsNow();
        //        qlog(listNowLevel.size());
        //text不能重复
        bool isRepeat=false;
        for(int i=0;i<listNowLevel.size();i++)
        {
            QTreeWidgetItem* pitem=listNowLevel[i];
            if(pitem->text(0)==strInput)
            {
                isRepeat=true;
                break;
            }
        }

        if(isRepeat)
        {
            messageBox("["+strInput+QString::fromLocal8Bit("]在该层中已存在,添加失败"));
            return;
        }

        //获取当前级中的index
        int index=this->getIndexOfCurrentLevel();
        //        qlog(index);
        QTreeWidgetItem* pItemCurrent=ui->treeWidget->currentItem();
        QTreeWidgetItem* pNewJoint=this->newJoint(strInput);
        pItemCurrent->parent()->insertChild(index+1,pNewJoint);
    }
}

void xTreeWidget::slotItemUpdate()
{
    QString strInput;
    if(inputbox(strInput))
    {
        //获取所有兄弟节点
        QVector<QTreeWidgetItem*> listNowLevel=this->getSiblingsNow();
        //        qlog(listNowLevel.size());
        //text不能重复
        bool isRepeat=false;
        for(int i=0;i<listNowLevel.size();i++)
        {
            QTreeWidgetItem* pitem=listNowLevel[i];
            if(pitem->text(0)==strInput)
            {
                isRepeat=true;
                break;
            }
        }

        if(isRepeat)
        {
            messageBox("["+strInput+QString::fromLocal8Bit("]在该层中已存在,修改失败"));
            return;
        }
        ui->treeWidget->currentItem()->setText(0,strInput);
    }
}
void xTreeWidget::messageBox(QString strDefault)
{
    QMessageBox::information(NULL,QString::fromLocal8Bit("提示信息"),strDefault);
}
bool xTreeWidget::inputbox(QString &strInput, QString strdefault)
{
    QInputDialog inputDialog;
    inputDialog.setWindowTitle(("inputbox"));
    inputDialog.setLabelText(strdefault);
    inputDialog.setOkButtonText(("ok"));
    inputDialog.setCancelButtonText(("cancel"));
    inputDialog.setTextValue(("please input"));
    if (inputDialog.exec())
    {
        strInput=inputDialog.textValue();
        return true;
    }
    return false;

}

void xTreeWidget::on_stuTableWidget_customContextMenuRequested(QPoint pos)
{
    //获取点击的tablewidget中行和列
    QModelIndex index = ui->treeWidget->indexAt (pos);
    hlog(index.row(),index.column());
    if(index.row()<0||index.column()<0)
        return;
    QTreeWidgetItem *curItem=ui->treeWidget->itemAt(pos);
    hlog(curItem->text(0));
//        this->nowRow = index.row ();//获得QTableWidget列表点击的行数
//        this->nowCol=index.column();
    //    qDebug()<<this->nowRow;
    //    qDebug()<<this->nowCol;
    //最后再显示,不然出问题
    pop_menu->exec(QCursor::pos());
}

void xTreeWidget::sortByColumn(int col)
{
    qDebug()<<"you click header col: "<<col;
    //      this->iColSort=col;
    if(bSortUporDown)
    {
        //不用在这里针对root层排序,因为只要再下一层就能避免通道头排序错乱的问题
        //每个通道都放到通道头下一层就行了
        ui->treeWidget->sortItems(col, Qt::AscendingOrder);

        bSortUporDown=false;
    }
    else
    {
        ui->treeWidget->sortItems(col, Qt::DescendingOrder);
        bSortUporDown=true;
    }
}
