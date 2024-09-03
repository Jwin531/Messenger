#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include "database.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->loginButton,&QPushButton::clicked,this,&LoginDialog::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::onRegisterClicked()
{
    RegisterDialog registerDialog(this);
    registerDialog.exec();
}

void LoginDialog::onLoginClicked()
{
    Database db;
    QString login = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if(login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка","Заполните все поля");
    }

    if(db.getUserData(login,password))
    {
        accept();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось войти в систему");
    }

}
