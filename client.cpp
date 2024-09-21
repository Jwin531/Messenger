#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this)) {
    connect(socket_, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

void Client::connectToServer(const QString &host, quint16 port) {
    socket_->connectToHost(host, port);
}

void Client::sendMessage(const QString &message) {
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->write(message.toUtf8());
    }
}

void Client::onReadyRead() {
    QByteArray data = socket_->readAll();
    // Обработка полученного сообщения
}
