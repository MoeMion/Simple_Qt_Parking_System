#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_user_button_clicked()
{
    if(database_check()){
    new_user_win = new user;
    new_user_win->setWindowTitle("用户界面 - 停车场智能管理系统");
    new_user_win->show();
    }else{
        ui->label->setText("数据库连接失败！");
    }
}

void MainWindow::on_admin_button_clicked()
{
    if(database_check()){
    new_login_win = new login;
    new_login_win->setWindowTitle("管理登录 - 停车场智能管理系统");
    new_login_win->show();
    }else{
        ui->label->setText("数据库连接失败！");
    }
}
