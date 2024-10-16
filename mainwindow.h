#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include "database.h"
#include "logindialog.h"
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setUsername(const QString& username);
    ~MainWindow();
signals:
    void messege(const QString& messege);

private slots:
    void onSendMessageClicked();
    void takeOnlineUser(const QString& line);
    void handleUserButtonClick(const QString &login);
    void onMessageReceived(const QString &sender, const QString &message,const bool& status);
    void deleteDisconnectUser(const QString& login);
public slots:
    void addUserButton(const QString& login);
    void takeLogin(const QString& login);

private:
    QLabel* returnLabel(const QString &sender);
    Ui::MainWindow *ui;
    QString selectedUser;
    Client *client_;
};

#endif // MAINWINDOW_H
