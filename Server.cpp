#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <Server.h>

using boost::asio::ip::tcp;

// class Session : public std::enable_shared_from_this<Session> {
// public:
Session::Session(tcp::socket socket) : socket_(std::move(socket)), timer_(socket_.get_executor()) {}

    void Session::start() {
        do_write("Вы подключены к серверу!\n");
        do_read();
        start_timer();
    }

// private:
    void Session::do_read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, buffer_, '\n',
                                      [this, self](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::istream is(&buffer_);
                                              std::string message;
                                              std::getline(is, message);

                                              std::cout << "Сообщение от клиента: " << message << std::endl;
                                              do_write("Сообщение получено: " + message + "\n");
                                              do_read();  // Читаем снова
                                          } else {
                                              std::cout << "Клиент отключен." << std::endl;
                                              timer_.cancel();
                                          }
                                      });
    }

    void Session::do_write(const std::string& message) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (ec) {
                                         std::cout << "Ошибка при отправке сообщения." << std::endl;
                                     }
                                 });
    }

    void Session::start_timer() {
        timer_.expires_after(boost::asio::chrono::seconds(5));
        wait_timer();
    }

    void Session::wait_timer() {
        auto self(shared_from_this());
        timer_.async_wait([this, self](boost::system::error_code ec) {
            if (!ec && socket_.is_open()) {
                std::cout << "Таймер сработал. Проверка активности клиента." << std::endl;
                do_write("Привет от сервера спустя 5 секунд!\n");
                timer_.expires_after(boost::asio::chrono::seconds(5));  // Устанавливаем таймер снова
                wait_timer();  // Ждем снова
            } else if (ec) {
                std::cout << "Таймер остановлен, клиент отключен." << std::endl;
            }
        });
    }

    // tcp::socket socket_;
    // boost::asio::steady_timer timer_;
    // boost::asio::streambuf buffer_;
// };

// class Server {
// public:
    Server::Server(boost::asio::io_context& io_context, short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

// private:
    void Server::do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "Клиент подключен." << std::endl;
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            });
    }

    // tcp::acceptor acceptor_;
// };
