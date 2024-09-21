#include "serversession.h"

ServerSession::ServerSession(tcp::socket socket): socket_(std::move(socket)), timer_(socket_.get_executor()) {}

void ServerSession::start()
{
    do_write();
    do_read();
    start_timer();
}

void ServerSession::do_read()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, buffer_, '\n',[this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
      if (!ec) {
          std::istream is(&buffer_);
          QString string message;
          std::getline(is, message);

          qDebug() << "Сообщение от клиента: " << message;
          do_write();
          do_read();  // Читаем снова
      } else {
          qDebug() << "Клиент отключен.";
          timer_.cancel();
      }
  });
}

void ServerSession::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(message),[this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (ec) {
            qDebug() << "Ошибка при отправке сообщения.";
        }
    });
}

void ServerSession::start_timer()
{
    timer_.expires_after(boost::asio::chrono::seconds(5));
    wait_timer();
}

void ServerSession::wait_timer()
{
    auto self(shared_from_this());
    timer_.async_wait([this, self](boost::system::error_code ec) {
        if (!ec && socket_.is_open()) {
            qDebug() << "Таймер сработал. Проверка активности клиента.";
            timer_.expires_after(boost::asio::chrono::seconds(5));  // Устанавливаем таймер снова
            wait_timer();  // Ждем снова
        } else if (ec) {
            qDebug() << "Таймер остановлен, клиент отключен.";
        }
    });
}
