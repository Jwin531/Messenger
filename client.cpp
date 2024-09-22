#include "client.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
//
Client::Client(QObject *parent): QObject(parent), socket_(new QTcpSocket(this)) {
    connect(socket_, &QTcpSocket::readyRead, this, &Client::readMessage);
}

void Client::connectToServer(const QString &host, quint16 port) {
    socket_->connectToHost(host, port);
}

void Client::readMessage() {
    while (socket_->canReadLine()) {
        QByteArray data = socket_->readLine();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            QString status = jsonObj.value("status").toString();
            qDebug() << "Статус:" << status;
            emit messageReceived(status);
        } else {
            qDebug() << "Неверный формат JSON";
        }
    }
}
