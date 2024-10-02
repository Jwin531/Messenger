#include "client.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QThread>

Client::Client(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this)) {
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
        emit processLine(line);
    }
}

