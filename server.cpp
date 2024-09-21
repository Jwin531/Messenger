#include "server.h"
#include "serversession.h"

Server::Server(boost::asio::io_context& io_context, short port):acceptor_(io_context,tcp::endpoint(tcp::v4(),port)) {do_accept();}

void Server::do_accept()
{
    acceptor_.accept([this](const boost::system::error_code& ec,tcp::socket socket){
        if(!ec)
        {
            qDebug() << "Подключен клиент!";
            std::make_shared<ServerSession>(std::move(socket))->start();
        }
        do_accept();
    });
}
