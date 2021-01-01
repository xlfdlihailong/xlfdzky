#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "qlibwin/qlib.h"
#include "interface.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QFont font;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CSGLAll gall;
    plist<TASK_STATUS_STRUCT> lTaskAll;//所有任务

    void threadGetTasks(pliststring listhost);

private slots:
    void slotExpandAll();
    void slotCollpseAll();
    //左键点击系统或站点响应
    void slotClickTreeLeftItem(QTreeWidgetItem* item,int column);
    void slotShowTask(pmap<pstring,plist<TASK_STATUS_STRUCT>> mapTasks);
    void slotMessageBox(pstring info);
    void threadGetTasksAll();
signals:
    void sigShowTask(pmap<pstring,plist<TASK_STATUS_STRUCT>> mapTasks);
    void sigMessageBox(pstring info);
private:
    ptcp* ptc;
    Ui::MainWindow *ui;
    pstring host="bf36yziked.54http.tech";
    int port=32513;
};

#endif // MAINWINDOW_H
