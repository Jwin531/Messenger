#include "client.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QThread>
#include <sw/redis++/redis++.h>

using namespace sw::redis;

Client::Client(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this)), redis_(make_shared<Redis>("tcp://127.0.0.1:6379")),login_(""), toLogin("") {
    // Подключение сигналов
    connect(socket_, &QTcpSocket::disconnected, this, []() {
        qDebug() << "Сокет отключен.";
    });
    connect(socket_, &QTcpSocket::readyRead, this, &Client::readData); // Обработчик для получения данных
}

void Client::connectToServer(const QString &host, int port) {
    socket_->connectToHost(host, port);
    if (!socket_->waitForConnected(3000)) {
        qDebug() << "Не удалось подключиться к серверу:" << socket_->errorString();
    } else {
        qDebug() << "Подключено к серверу:" << host << ":" << port;
    }
}

void Client::sendLogin(const QString& login) {
    setLogin(login);
    QJsonObject json;
    json["login"] = login;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n"; // Добавляем перенос строки
    qDebug() << "Отправляем логин:" << data;

    socket_->write(data);
    if (!socket_->waitForBytesWritten(3000)) {
        qDebug() << "Не удалось отправить данные:" << socket_->errorString();
    }
}

void Client::readData() {
    // Читаем все доступные строки
    while (socket_->canReadLine()) {
        QString line = socket_->readLine().trimmed(); // Удаляем пробелы и переносы

        // Разбор строки JSON
        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
        if (doc.isNull()) {
            qDebug() << "Ошибка при разборе JSON:" << line;
            return; // Если JSON не удалось разобрать, выходим
        }

        // Извлекаем данные
        QJsonObject jsonObj = doc.object();
        QString response = jsonObj["response"].toString();

        // Разбор вложенного JSON в строке response
        QJsonDocument responseDoc = QJsonDocument::fromJson(response.toUtf8());
        if (responseDoc.isNull()) {
            qDebug() << "Ошибка при разборе вложенного JSON:" << response;
            return; // Если вложенный JSON не удалось разобрать, выходим
        }
        QJsonObject responseObj = responseDoc.object();
        if(responseObj["type"] == "user_disconnected")
        {
            QString login = responseObj["login"].toString();
            qDebug() << "Пришел логин дисконекта: " + login;
            emit disconnectUser(login);
        }
        else
        {
            emit processLine(line); // Можно оставить, если вам нужно просто обработать строки

            QString message = responseObj["message"].toString();
            QString sender = responseObj["sender"].toString();
            // QString type = responseObj["type"].toString();
            bool status = responseObj["status"].toBool();

            // Обработка сообщения (например, генерируем сигнал)
            qDebug() << "Получено сообщение от" << sender << ":" << message;
            emit messageReceived(sender, message,status); // Эмитируем сигнал с данными
        }
    }
}


void Client::sendMessage(const QString &message)
{
    QJsonObject json;
    json["type"] = "message"; // Тип сообщения
    json["sender"] = getLogin(); // Отправитель
    json["receiver"] = getToLogin(); // Получатель
    json["message"] = message; // Текст сообщения
    json["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate); // Время отправки
    json["message_id"] = QUuid::createUuid().toString(); // Уникальный идентификатор сообщения
    json["messageType"] = "text"; // Тип сообщения (например, текст, изображение и т.д.)

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";

    socket_->write(data);
    if (!socket_->waitForBytesWritten(3000)) {
        qDebug() << "Не удалось отправить данные:" << socket_->errorString();
    }
    qDebug() << "Отправил на сервер: " << data;
}


