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
        processLine(line);
    }
}

void Client::processLine(const QString& line) {
    // Парсим JSON сообщение
    QJsonDocument jsonDoc = QJsonDocument::fromJson(line.toUtf8()); // Убедимся, что строка в правильном формате

    // Проверяем, что документ корректен
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка парсинга JSON:" << line;
        return;
    }

    // Получаем корневой объект
    QJsonObject jsonObject = jsonDoc.object();

    // Проверяем, содержит ли объект поле "response"
    if (jsonObject.contains("response")) {
        QString response = jsonObject["response"].toString();

        // Парсим вложенный JSON
        QJsonDocument responseDoc = QJsonDocument::fromJson(response.toUtf8());
        if (responseDoc.isNull()) {
            qDebug() << "Ошибка парсинга вложенного JSON:" << response;
            return;
        }

        // Обрабатываем вложенный JSON
        QJsonObject responseObject = responseDoc.object();

        // Обрабатываем различные типы сообщений
        if (responseObject.contains("logins")) {
            QJsonArray loginsArray = responseObject["logins"].toArray();
            qDebug() << "Активные пользователи:";
            for (const QJsonValue &value : loginsArray) {
                qDebug() << value.toString(); // Выводим логины активных пользователей
            }
        } else if (responseObject.contains("login")) {
            QString login = responseObject["login"].toString();
            qDebug() << "Новый пользователь:" << login;
        } else {
            qDebug() << "Неизвестный тип сообщения во вложенном JSON:" << response;
        }
    } else {
        qDebug() << "Неизвестный тип сообщения:" << line;
    }
}
