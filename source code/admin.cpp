#include "admin.h"
#include "ui_admin.h"

admin::admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
}

admin::~admin()
{
    delete ui;
}

void admin::on_car_store_clicked()
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

void admin::on_car_fetch_clicked()
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

void admin::on_car_quire_clicked()
{
    ui->resulttext->clear();
    char carid[6];
    qstr2cstr(ui->carid_input->text(),carid);
    if(carid_check(carid)){
        stall_id = 0 ;
        stall_get_id(carid);
        if(stall_id){
            ui->resulttext->append("车辆"+QString(carid)+"停泊在车位："+QString::number(stall_id));
        }else{
            ui->resulttext->append("车库中不存在此车辆！");
        }
    }else{
        ui->resulttext->append("车牌不符合要求，请重新输入！");
    }
}

void admin::on_his_quire_clicked()
{
    ui->resulttext->clear();
    char carid[6];
    qstr2cstr(ui->carid_input->text(),carid);
    result_output = "";
    if(carid_check(carid)){
        history_quire(carid);
        if(result_output!=""){
            ui->resulttext->append(result_output);
        }else{
            ui->resulttext->append("系统中暂无该车辆出入记录！");
        }
    }else{
        ui->resulttext->append("车牌不符合要求，请重新输入！");
    }
}

void admin::on_list_stall_clicked()
{
    ui->resulttext->clear();
    result_output = "";
    stall_listall();
    if(result_output!=""){
        ui->resulttext->append(result_output);
    }else{
        ui->resulttext->append("系统中暂出入记录！");
    }
}


void admin::on_list_finance_clicked()
{
    ui->resulttext->clear();
    result_output = "";
    finance_listall();
    if(result_output!=""){
        ui->resulttext->append(result_output);
    }else{
        ui->resulttext->append("系统中暂无财务记录！");
    }
}
