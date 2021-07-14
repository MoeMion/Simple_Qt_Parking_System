#ifndef USER_H
#define USER_H

#include <QDialog>
#include <string>
#include <cstring>
#include "controller.h"

namespace Ui {
class user;
}

class user : public QDialog
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr);
    ~user();

private slots:
    void on_car_store_clicked();

    void on_car_fetch_clicked();

private:
    Ui::user *ui;
};

#endif // USER_H
