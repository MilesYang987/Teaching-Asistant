#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "databasehelper.h"
#include "confighelper.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    void Login();
    void TestConnection();
    void LoadConfig();

private:
    Ui::LoginWindow *ui;

    bool first_flag;

    DatabaseHelper& db;

    ConfigHelper& config;
};
#endif // LOGINWINDOW_H
