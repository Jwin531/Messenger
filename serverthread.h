#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include "server.h"  // Подключите ваш файл с определением ChatServer и ChatSession

class ServerThread : public QThread {
    Q_OBJECT

public:
    ServerThread();
    ~ServerThread();

protected:
    void run() override;

private:
    boost::asio::io_context io_context_;
    boost::asio::signal_set signals_;
    ChatServer server_;
};

#endif // SERVERTHREAD_H
