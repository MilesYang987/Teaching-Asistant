#include "loginwindow.h"
#include "./ui_loginwindow.h"

#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow), db(DatabaseHelper::GetInstance()), config(ConfigHelper::GetInstance())
{
    ui->setupUi(this);

    LoadConfig();

    connect(ui->loginButton, &QPushButton::clicked,this, &LoginWindow::Login);

    connect(ui->connectButton, &QPushButton::clicked,this, &LoginWindow::TestConnection);

    connect(ui->log2regButton, &QPushButton::clicked,this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->log2setButton, &QPushButton::clicked,this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->set2logButton, &QPushButton::clicked,this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->reg2logButton, &QPushButton::clicked,this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::Login()
{
    if (first_flag){
        QMessageBox::information(this, "提示","请先设置数据库连接！",QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void LoginWindow::TestConnection()
{
    if (ui->sqlUserLine->text().isEmpty() || ui->sqlPassLine->text().isEmpty()){
        QMessageBox::warning(this,"警告","用户名和密码不能为空！",QMessageBox::Ok);
    }
    db.Setup(QString::fromStdString("teaching"),ui->sqlUserLine->text(),ui->sqlPassLine->text(),ui->sqlIPLine->text(),ui->sqlPortLine->text());
    if (!db.TryConnection()){
        QMessageBox::critical(this,"错误",QString::fromStdString(db.ErrorMsg()),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,"提示","连接成功！",QMessageBox::Ok);
        first_flag = false;

        config.WriteConfig("mysql","ip",ui->sqlIPLine->text().toStdString());
        config.WriteConfig("mysql","port",ui->sqlPortLine->text().toStdString());
        config.WriteConfig("mysql","user",ui->sqlUserLine->text().toStdString());
        config.WriteConfig("mysql","pass",ui->sqlPassLine->text().toStdString());
    }

}

void LoginWindow::LoadConfig()
{
    ui->sqlIPLine->setText(QString::fromStdString(config.ReadConfig("mysql","ip")));
    ui->sqlPortLine->setText(QString::fromStdString(config.ReadConfig("mysql","port")));
    ui->sqlUserLine->setText(QString::fromStdString(config.ReadConfig("mysql","user")));
    ui->sqlPassLine->setText(QString::fromStdString(config.ReadConfig("mysql","pass")));

    first_flag = config.ReadConfig("sys","first") == "1";
}
