#include "client.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QTimer>

Client::Client(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this)) {
    connect(socket_, &QTcpSocket::disconnected, this, []() {
        qDebug() << "Сокет отключен.";
    });

    connect(socket_, &QTcpSocket::connected, this, &Client::onConnected);

}

void Client::connectToServer(const QString &host, int port) {
    qDebug() << "Попытка подключения к серверу..." << host << ":" << port;

    socket_->connectToHost(host, port);
}

void Client::onConnected() {
    QString message = "1234567890";
    QByteArray messageData = message.toUtf8();
    qDebug() << "Отправка сообщения на сервер:" << message << "Состояние: " << socket_->state();

    socket_->write(messageData);
    socket_->flush();
}

void Client::sendMessageToServer(const QString &message) {
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        QJsonObject jsonMessage;
        jsonMessage["message"] = message;
        QJsonDocument jsonDoc(jsonMessage);
        QByteArray jsonData = jsonDoc.toJson() + "\n";

        qDebug() << "Отправка сообщения на сервер:" << jsonData << "Состояние: " << socket_->state();
        socket_->write(jsonData);
        socket_->flush();
    } else {
        qDebug() << "Клиент не подключен к серверу! Статус сокета:" << socket_->state();
    }
}



