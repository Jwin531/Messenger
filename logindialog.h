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
    ~LoginDialog() override;

signals:
    void loginSuccessful(const QString &username);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::LoginDialog *ui;
    Client* client_;
};

#endif // LOGINDIALOG_H
