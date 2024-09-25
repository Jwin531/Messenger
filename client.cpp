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

    connect(socket_,&QTcpSocket::readyRead,this,&Client::readMessageFromServer);

}

void Client::connectToServer(const QString &host, int port) {
    socket_->connectToHost(host, port);
}


void Client::sendMessageToServer(const QString &message) {
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        QJsonObject jsonMessage;
        jsonMessage["message"] = message;
        QJsonDocument jsonDoc(jsonMessage);
        QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact) + "\n";


        qDebug() << "Отправка сообщения на сервер:" << jsonData << "Состояние: " << socket_->state();
        socket_->write(jsonData);
        socket_->flush();
    } else {
        qDebug() << "Клиент не подключен к серверу! Статус сокета:" << socket_->state();
    }
}

void Client::readMessageFromServer() {
    static QByteArray buffer;
    buffer.append(socket_->readAll());

    // Проверяем, есть ли в буфере полный JSON (оканчивающийся на \n)
    if (buffer.contains('\n')) {
        QByteArray jsonData = buffer.left(buffer.indexOf('\n')); // Извлекаем часть до \n
        buffer.remove(0, buffer.indexOf('\n') + 1); // Удаляем обработанную часть из буфера

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("response"))
            {
                QString serverMessage = jsonObj["response"].toString();
                qDebug() << "Сообщение от сервера:" << serverMessage;
            }
        }
    }
}


