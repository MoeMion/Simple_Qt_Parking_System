#include "user.h"
#include "ui_user.h"

user::user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user)
{
    ui->setupUi(this);
}

user::~user()
{
    delete ui;
}

void user::on_car_store_clicked()
{
    ui->resulttext->clear();
    char carid[6];
    qstr2cstr(ui->carid_input->text(),carid);
    if(carid_check(carid)){
        int stallid=car_storage(carid);
        if(stallid == 0){
            ui->resulttext->append("当前车库已满，请稍后再试！");
        }else if(stallid == 1){
            ui->resulttext->append("该车辆已经存放在车库中，请使用取车功能！");
        }else if(stallid >= 2){
            ui->resulttext->append("存车成功！您的车位为："+QString::number(stallid));
        }
    }else{
        ui->resulttext->append("车牌不符合要求，请重新输入！");
    }
}

void user::on_car_fetch_clicked()
{
    ui->resulttext->clear();
    char carid[6];
    qstr2cstr(ui->carid_input->text(),carid);
    if(carid_check(carid)){
        int parktime=car_fetch(carid);
        if(parktime == 0){
        ui->resulttext->append("当前车辆未入库！");
        }else{
            int amount = finance_compute(parktime);
            finance_check(amount,carid);
            ui->resulttext->append("取车成功！总费用："+QString::number(amount)+"元！");
        }
    }else{
        ui->resulttext->append("车牌不符合要求，请重新输入！");
    }
}
