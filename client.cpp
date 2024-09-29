#include "client.h"
#include <QJsonDocument>
#include <QJsonArray>
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


void Client::sendMessageToServer(const QString &message,const short&& messageType) {
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        QJsonObject jsonMessage;
        switch(messageType)
        {

            case 1:
                jsonMessage["message"] = message;
                break;

            case 2:
                jsonMessage["sessionId"] = message;
                break;
        }
            QJsonDocument jsonDoc(jsonMessage);
            QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact) + "\n";


            qDebug() << "Отправка сообщения на сервер:" << jsonData << "Состояние: " << socket_->state();
            socket_->write(jsonData);
            socket_->flush();

    }
    else {
        qDebug() << "Клиент не подключен к серверу! Статус сокета:" << socket_->state();
    }
}

void Client::readMessageFromServer() {
    static QByteArray buffer;
    buffer.append(socket_->readAll());
    if (buffer.contains('\n')) {
        QByteArray jsonData = buffer.left(buffer.indexOf('\n'));
        buffer.remove(0, buffer.indexOf('\n') + 1);

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (!jsonDoc.isNull())
        {
            if (jsonDoc.isObject())
            {
                // Обработка JSON-объекта
                QJsonObject jsonObj = jsonDoc.object();
                for (const QString& key : jsonObj.keys())
                {
                    handleJsonKey(key, jsonObj[key].toString());
                }
            } else if (jsonDoc.isArray())
            {
                // Обработка JSON-массива
                QJsonArray jsonArray = jsonDoc.array();
                for (const QJsonValue& value : jsonArray)
                {
                    if (value.isObject()) {
                        QJsonObject jsonObj = value.toObject();
                        for (const QString& key : jsonObj.keys())
                        {
                            handleJsonKey(key, jsonObj[key].toString());
                        }
                    }
                }
            }
        }

    }
}

void Client::handleJsonKey(const QString& key, const QString& value)
{
    // Общая обработка для всех ключей
    //qDebug() << "Ключ:" << key << ", Значение:" << value;

    if(key == "login")
    {
        qDebug() << "Полученный логин" << value;
    }
    else if (key == "response")
    {
        emit messageToMain(value);
    }

}
