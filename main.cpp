#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qlib::loadQss(&a,"flatwhite.css");
    MainWindow w;
    w.show();

    return a.exec();
}
