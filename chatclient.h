#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>#include <boost/asio.hpp>
#include <boost/thread.hpp>

class ChatClient : public QMainWindow {
    Q_OBJECT
public:
    explicit ChatClient(const QUrl &url, QWidget *parent = nullptr);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);

private:
    QWebSocket *m_webSocket;
    QUrl m_url;
    boost::asio::io_context m_ioContext;
    boost::asio::steady_timer m_timer;
};

#endif // CHATCLIENT_H
