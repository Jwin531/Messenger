// server.h
#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void start();

private:
    void do_read();
    void do_write(const std::string& message);
    void start_timer();
    void wait_timer();

    tcp::socket socket_;
    boost::asio::steady_timer timer_;
    boost::asio::streambuf buffer_;
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void do_accept();
    tcp::acceptor acceptor_;
};

#endif // SERVER_H
