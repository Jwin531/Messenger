#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <thread>
#include "serverthread.h"  // Подключите файл с объявлением ServerThread

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    //void sendMessageToUser(const QString& message);

private:
    Ui::MainWindow *ui;
    ServerThread* serverThread_;
};

#endif // MAINWINDOW_H
