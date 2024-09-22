#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
//

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);

signals:
    void messageReceived(const QString &message);

private slots:
    void readMessage();

private:
    QTcpSocket *socket_;
};

#endif // CLIENT_H
