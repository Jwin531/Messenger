#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <sw/redis++/redis++.h>

using namespace sw::redis;
using namespace std;

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void connectToServer(const QString &host, int port);

    void sendLogin(const QString& login);

    void sendMessage(const QString& message);

    void readData();
    QString getToLogin(){return toLogin;}
    QString getLogin() {return login_;}
    void setLogin(const QString& login){login_ = login;}

    void setToLogin(const QString& login){toLogin = login;}
signals:
    void processLine(const QString& line);

    void disconnectUser(const QString& login);

    void messageReceived(const QString &sender, const QString &message, const bool& status);

    void messageToMain(const QString& message);

private:
    QTcpSocket *socket_;
    shared_ptr<Redis> redis_;
    QString login_;
    QString toLogin;
};

#endif // CLIENT_H
