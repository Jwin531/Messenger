// // serverthread.h
// #include <boost/asio.hpp>
// #include "server.h" // Ваш код сервера

// class ServerThread : public QThread {
//     Q_OBJECT

// public:
//     explicit ServerThread(QObject *parent = nullptr);
//     void run() override;
//     void stopServer();

// private:
//     boost::asio::io_context io_context;
//     std::unique_ptr<Server> server;
// };
