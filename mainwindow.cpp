#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serverThread_(new ServerThread())  // Инициализация ServerThread
{
    ui->setupUi(this);
    serverThread_->start();  // Запуск потока сервера
}

MainWindow::~MainWindow() {
    serverThread_->quit();  // Остановка потока сервера
    serverThread_->wait();  // Ожидание завершения потока сервера
    delete serverThread_;
    delete ui;
}
