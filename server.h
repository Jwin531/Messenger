#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <set>
#include <string>

using namespace std;
using boost::asio::ip::tcp;

class ChatSession;
class ChatServer;

class ChatSession : public enable_shared_from_this<ChatSession> {
public:
    ChatSession(tcp::socket socket, ChatServer& server);
    void start();
    void deliver(const string& message);

private:
    void do_read();
    void do_write();

    tcp::socket socket_;
    ChatServer& server_;
    char read_msg_[1024] = {0};
    string write_msg_;
};

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context, short port);
    void join(shared_ptr<ChatSession> session);
    void leave(shared_ptr<ChatSession> session);
    void deliver(const string& message, shared_ptr<ChatSession> sender);

private:
    void do_accept();

    tcp::acceptor acceptor_;
    set<shared_ptr<ChatSession>> sessions_;
};

#endif // SERVER_H
