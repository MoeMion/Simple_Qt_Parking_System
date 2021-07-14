#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "login.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    user *new_user_win;
    login *new_login_win;

private slots:
    void on_user_button_clicked();

    void on_admin_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
