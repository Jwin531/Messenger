#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void connectToServer(const QString &host, int port);

    void sendLogin(const QString& login);

    void readData();
    void processLine(const QString& line);

signals:
    void messageToMain(const QString& message);

private:
    QTcpSocket *socket_;
};

#endif // CLIENT_H
