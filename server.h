#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <boost/asio.hpp>
#include <QDebug>

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_context& io_context, short port);
protected:
    void do_accept();

    tcp::acceptor acceptor_;
};

#endif // SERVER_H
