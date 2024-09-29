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

private:
    Ui::MainWindow *ui;
    QString selectedUser;
    Client *client_;
};

#endif // MAINWINDOW_H
