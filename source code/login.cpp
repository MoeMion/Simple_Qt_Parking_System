#include "login.h"
#include "ui_login.h"


login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    if(ui->password_input->text()=="123456"){
        this->close();
        new_admin_win = new admin;
        new_admin_win->setWindowTitle("管理界面 - 停车场智能管理系统");
        new_admin_win->show();
    }else{
        ui->label_2->setText("密码错误请重试！");
    }
}
