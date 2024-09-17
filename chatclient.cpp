#include "chatclient.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

ChatClient::ChatClient(const QUrl &url, QWidget *parent)
    : QMainWindow(parent), m_url(url), m_timer(m_ioContext, boost::asio::chrono::seconds(5)) {
    m_webSocket = new QWebSocket();
    connect(m_webSocket, &QWebSocket::connected, this, &ChatClient::onConnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &ChatClient::onTextMessageReceived);
    m_webSocket->open(url);

    // Интерфейс пользователя
    auto *mainLayout = new QVBoxLayout;
    auto *inputField = new QLineEdit;
    auto *sendButton = new QPushButton("Отправить");

    connect(sendButton, &QPushButton::clicked, this, [=]() {
        sendMessage(inputField->text());
        inputField->clear();
    });

    auto *centralWidget = new QWidget(this);
    mainLayout->addWidget(inputField);
    mainLayout->addWidget(sendButton);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Использование Boost Asio
    m_timer.async_wait([&](const boost::system::error_code&) {
        qDebug() << "Timer expired!";
    });

    boost::thread(boost::bind(&boost::asio::io_context::run, &m_ioContext));
}

void ChatClient::onConnected() {
    qDebug() << "Connected to server!";
}

void ChatClient::sendMessage(const QString &message) {
    m_webSocket->sendTextMessage(message);
}

void ChatClient::onTextMessageReceived(const QString &message) {
    qDebug() << "Message received:" << message;
}
