#ifndef SERVERSESSION_H
#define SERVERSESSION_H
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <iostream>
#include <memory>
#include <QWidget>
#include <QDebug>

using boost::asio::ip::tcp;

class ServerSession: public __enable_shared_from_this<ServerSession>
{
public:
    ServerSession(tcp::socket socket);

    void start();
private:

    void do_read();
    void do_write();
    void start_timer();
    void wait_timer();

    tcp::socket socket_;
    boost::asio::steady_timer timer_;
    boost::asio::streambuf buffer_;
};

#endif // SERVERSESSION_H
