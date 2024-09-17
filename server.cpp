#include "server.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace std;

ChatSession::ChatSession(tcp::socket socket, ChatServer& server)
    : socket_(move(socket)), server_(server) {}

void ChatSession::start() {
    server_.join(shared_from_this());
    do_read();
}

void ChatSession::deliver(const string& message) {
    write_msg_ = message;
    do_write();
}

void ChatSession::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(read_msg_, 1024),
                            [this, self](const boost::system::error_code& error, size_t size) {
                                if (!error) {
                                    string message(read_msg_, size);
                                    server_.deliver(message, shared_from_this());
                                    do_read();
                                } else {
                                    server_.leave(shared_from_this());
                                }
                            });
}

void ChatSession::do_write() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(write_msg_),
                             [this, self](const boost::system::error_code& error, size_t size) {
                                 if (error) {
                                     server_.leave(shared_from_this());
                                 }
                             });
}

ChatServer::ChatServer(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    do_accept();
}

void ChatServer::join(shared_ptr<ChatSession> session) {
    sessions_.insert(session);
    cout << "New client connected!" << endl;
}

void ChatServer::leave(shared_ptr<ChatSession> session) {
    sessions_.erase(session);
}

void ChatServer::deliver(const string& message, shared_ptr<ChatSession> sender) {
    for (auto& session : sessions_) {
        if (session != sender) {
            session->deliver(message);
        }
    }
}

void ChatServer::do_accept() {
    acceptor_.async_accept([this](const boost::system::error_code& error, tcp::socket socket) {
        if (!error) {
            auto session = make_shared<ChatSession>(move(socket), *this);
            session->start();
        }
        do_accept();
    });
}
