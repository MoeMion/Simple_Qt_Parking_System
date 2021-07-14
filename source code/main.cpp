#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <string>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.setWindowTitle("欢迎界面 - 停车场智能管理系统");
    w.show();
    return a.exec();
}

