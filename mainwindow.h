#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include "database.h"
#include "logindialog.h"

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
    void onSendVoiceMessageClicked();
    void messegeFromAnother(const QString& message);
    void takeOnlineUser(const QString& line);
    void handleUserButtonClick(const QString &login);
    void onMessageReceived(const QString &sender, const QString &message, const QString &type);
    void deleteDisconnectUser(const QString& login);
public slots:
    void takeLogin(const QString& login);

private:
    Ui::MainWindow *ui;
    QString selectedUser;
    Client *client_;
};

#endif // MAINWINDOW_H
