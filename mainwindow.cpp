#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->sendMessage, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
    connect(ui->SendVoiceMessage, &QPushButton::clicked, this, &MainWindow::onSendVoiceMessageClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::onSendMessageClicked() {
    QString message = ui->messageLine->text();
    if (!message.isEmpty()) {

        ui->textWith->append("You: " + message);  // Отобразим сообщение в чате
        ui->messageLine->clear();
    }
}

void MainWindow::onSendVoiceMessageClicked() {
    // Логика отправки голосовых сообщений
}
