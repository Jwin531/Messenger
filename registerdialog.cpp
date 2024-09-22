#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "database.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);
    connect(ui->submitButton, &QPushButton::clicked, this, &RegisterDialog::onSubmitButtonClicked);
}

void RegisterDialog::setToDatabase() {
    Database& db = Database::instance();
    QString username = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля");
        return;
    }

    if (db.setUserData(username, password)) {
        QMessageBox::information(this, "Успешная регистрация", "Вы успешно зарегистрированы");
        emit registrationSuccessful(username);
        accept();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить данные пользователя");
    }
}

void RegisterDialog::onSubmitButtonClicked() {
    setToDatabase();
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}
