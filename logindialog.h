#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "client.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

signals:
    void transLogin(const QString& login);
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
