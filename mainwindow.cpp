#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), client_(new Client(this)) {
    ui->setupUi(this);
    connect(ui->sendMessage, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
    connect(ui->SendVoiceMessage, &QPushButton::clicked, this, &MainWindow::onSendVoiceMessageClicked);
    connect(client_,&Client::messageToMain,this,&MainWindow::messegeFromAnother);

    client_->connectToServer("127.0.0.1", 1234);
}

MainWindow::~MainWindow() {
    qDebug() << "Приложение закрыто!";
    delete ui;
}

void MainWindow::onSendMessageClicked() {
    QString message = ui->messageLine->text();
    if (!message.isEmpty()) {
        ui->textWith->append("You: " + message);
        client_->sendMessageToServer(message);
        ui->messageLine->clear();
    } else {
        qDebug() << "Сообщение пустое!";
    }
}

void MainWindow::onSendVoiceMessageClicked() {
    // Логика отправки голосовых сообщений
}

void MainWindow::messegeFromAnother(const QString& message)
{
    ui->textWith->append("He: " + message);
}
