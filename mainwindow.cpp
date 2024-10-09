#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "QUuid"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),client_(new Client(this)) {
    ui->setupUi(this);

    connect(ui->sendMessage, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
    connect(ui->SendVoiceMessage, &QPushButton::clicked, this, &MainWindow::onSendVoiceMessageClicked);
    connect(client_,&Client::processLine,this,&MainWindow::takeOnlineUser);
    connect(client_,&Client::messageReceived,this,&MainWindow::onMessageReceived);
    connect(client_,&Client::disconnectUser,this,&MainWindow::deleteDisconnectUser);

    ui->textWith->hide();
    ui->sendMessage->hide();
    ui->SendVoiceMessage->hide();
    ui->messageLine->hide();
    client_->connectToServer("127.0.0.1", 1234);
    ui->UsersVerticalLayout->addSpacing(5);
}


MainWindow::~MainWindow() {
    qDebug() << "Приложение закрыто!";
    delete ui;
}

// Сигнал удаления отключившегося пользователя
void MainWindow::deleteDisconnectUser(const QString& login)
{
    for (int i = 0; i < ui->UsersVerticalLayout->count(); ++i) {
        QLayoutItem* item = ui->UsersVerticalLayout->itemAt(i);
        if (item) {
            QWidget* widget = item->widget();
            if (widget) {
                QPushButton* button = qobject_cast<QPushButton*>(widget);
                if (button) {
                    // Проверка текста кнопки
                    if (button->text() == login) {
                        ui->UsersVerticalLayout->removeWidget(button);
                        button->deleteLater();
                    }
                }
            }
        }
    }
}

// Сигнал после нажатия отправки сообщения
void MainWindow::onSendMessageClicked() {
    QString message = ui->messageLine->text();
    if (!message.isEmpty()) {
        ui->textWith->append("You: " + message);
        client_->sendMessage(message);
        ui->messageLine->clear();
    } else {
        qDebug() << "Сообщение пустое!";
    }
}

void MainWindow::onSendVoiceMessageClicked() {
    // Логика отправки голосовых сообщений
}

void MainWindow::takeLogin(const QString& login)
{
    client_->sendLogin(login);
}

void MainWindow::takeOnlineUser(const QString& line) {

    // Парсим JSON сообщение
    QJsonDocument jsonDoc = QJsonDocument::fromJson(line.toUtf8());

    // Проверяем, что документ корректен
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка парсинга JSON:" << line;
        return;
    }

    // Получаем корневой объект
    QJsonObject jsonObject = jsonDoc.object();

    // Проверяем, содержит ли объект поле "response"
    if (jsonObject.contains("response")) {
        QString response = jsonObject["response"].toString();

        // Парсим вложенный JSON
        QJsonDocument responseDoc = QJsonDocument::fromJson(response.toUtf8());
        if (responseDoc.isNull()) {
            qDebug() << "Ошибка парсинга вложенного JSON:" << response;
            return;
        }

        // Обрабатываем вложенный JSON
        QJsonObject responseObject = responseDoc.object();

        if (responseObject.contains("logins")) {
            // Если пришел список пользователей
            QJsonArray loginsArray = responseObject["logins"].toArray();

            for (const QJsonValue &value : loginsArray) {
                QPushButton *loginButton = new QPushButton(value.toString());
                ui->UsersVerticalLayout->addWidget(loginButton);

                connect(loginButton, &QPushButton::clicked, this, [=]() {
                    handleUserButtonClick(loginButton->text());
                });

                qDebug() << value.toString();
            }
        } else if (responseObject.contains("login")) {
            // Если добавляется новый пользователь
            QString login = responseObject["login"].toString();

            QPushButton *loginButton = new QPushButton(login);
            ui->UsersVerticalLayout->addWidget(loginButton);

            connect(loginButton, &QPushButton::clicked, this, [=]() {
                handleUserButtonClick(loginButton->text());
            });
        } else {
            qDebug() << "Неизвестный тип сообщения во вложенном JSON:" << response;
        }
    } else {
        qDebug() << "Неизвестный тип сообщения:" << line;
    }
}



void MainWindow::handleUserButtonClick(const QString& login)
{
    client_->setToLogin(login);
    Database& db = Database::instance();
    QVector<QString> messages = db.takeAllMessagesFromThisChat(client_->getToLogin(),client_->getLogin());

    ui->textWith->show();
    ui->sendMessage->show();
    ui->SendVoiceMessage->show();
    ui->messageLine->show();
    ui->textWith->clear();
    ui->chatWithLabel->setText(login);
    if(messages.isEmpty())
    {
        ui->textWith->append("Пустой диалог!");
    }
    else
    {
        for (const QString& message : messages) {
            ui->textWith->append(message);
        }
    }

    qDebug() << "Кто отправит: " << client_->getLogin();
    qDebug() << "Кому отправится: " << client_->getToLogin();
}

void MainWindow::onMessageReceived(const QString &sender, const QString &message, const QString &type)
{
    ui->textWith->append(sender+ ": " + message);
}
