#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <string>
#include <cstring>
#include "controller.h"

namespace Ui {
class admin;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_car_store_clicked();

    void on_car_fetch_clicked();

    void on_car_quire_clicked();

    void on_his_quire_clicked();

    void on_list_stall_clicked();

    void on_list_finance_clicked();

private:
    Ui::admin *ui;
};

#endif // ADMIN_H
