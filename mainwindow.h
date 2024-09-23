#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void messege(const QString& messege);

private slots:
    void onSendMessageClicked();
    void onSendVoiceMessageClicked();

private:
    Ui::MainWindow *ui;
    Client *client_;
};

#endif // MAINWINDOW_H
