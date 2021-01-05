#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "qlibwin/qlib.h"
#include "interface.h"
#include <QProgressBar>
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

private slots:

    //左键点击系统或站点响应
    void slotClickTreeLeftItem(QTreeWidgetItem* item,int column);
    void slotShowTask(pmap<pstring,plist<TASK_STATUS_STRUCT>> mapTasks);
signals:
    void sigShowTask(pmap<pstring,plist<TASK_STATUS_STRUCT>> mapTasks);
    void sigMessageBox(pstring info);
private:
    QString toStringState(int state);
    QString toStringYXJ(int yxj);
    ptcp* ptc;
    Ui::MainWindow *ui;
    pstring host="bf36yziked.54http.tech";
    int port=32513;
};

#endif // MAINWINDOW_H
