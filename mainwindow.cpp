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
    connect(client_,&Client::processLine,this,&MainWindow::takeOnlineUser);
    connect(client_,&Client::messageReceived,this,&MainWindow::onMessageReceived);
    connect(client_,&Client::disconnectUser,this,&MainWindow::deleteDisconnectUser);

    ui->chatWithLabel->setText("Выберите с кем начать диалог");
    ui->textWith->hide();
    ui->sendMessage->hide();
    ui->messageLine->hide();
    client_->connectToServer("127.0.0.1", 1234);
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
        if (!item) {
            continue;  // Защита от возможных null-элементов
        }

        // Проверяем, является ли элемент макетом (QHBoxLayout)
        QLayout* hboxLayout = item->layout();
        if (hboxLayout) {
            // Перебираем виджеты внутри QHBoxLayout
            for (int j = 0; j < hboxLayout->count(); ++j) {
                QLayoutItem* hboxItem = hboxLayout->itemAt(j);
                if (!hboxItem) {
                    continue;
                }

                // Проверяем, является ли виджет кнопкой
                QPushButton* button = qobject_cast<QPushButton*>(hboxItem->widget());
                if (button && button->text() == login) {
                    // Удаляем все виджеты из QHBoxLayout
                    while (QLayoutItem* childItem = hboxLayout->takeAt(0)) {
                        if (QWidget* childWidget = childItem->widget()) {
                            childWidget->deleteLater();  // Безопасно удаляем виджеты
                        }
                        delete childItem;  // Удаляем QLayoutItem
                    }

                    // Удаляем сам QHBoxLayout из вертикального макета
                    ui->UsersVerticalLayout->removeItem(item);

                    // Удаляем макет и элемент безопасно
                    hboxLayout->deleteLater();  // Используем deleteLater() для макета
                    delete item;  // Удаляем QLayoutItem

                    return;  // Выходим из функции, как только удалили нужный элемент
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
                QHBoxLayout* layout = new QHBoxLayout();
                QLabel* label = new QLabel("");
                QPushButton *loginButton = new QPushButton(value.toString());
                layout->addWidget(loginButton);
                layout->addWidget(label);
                ui->UsersVerticalLayout->addLayout(layout);

                connect(loginButton, &QPushButton::clicked, this, [=]() {
                    handleUserButtonClick(loginButton->text());
                });

                qDebug() << value.toString();
            }
        } else if (responseObject.contains("login")) {
            // Если добавляется новый пользователь
            QString login = responseObject["login"].toString();

            QHBoxLayout* layout = new QHBoxLayout();
            QLabel* label = new QLabel("");
            QPushButton *loginButton = new QPushButton(login);
            layout->addWidget(loginButton);
            layout->addWidget(label);
            ui->UsersVerticalLayout->addLayout(layout);

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
    QVector<QString> messages = db.takeAllMessagesFromThisChat(client_->getToLogin(), client_->getLogin());

    ui->textWith->show();
    ui->sendMessage->show();
    ui->messageLine->show();
    ui->textWith->clear();
    ui->chatWithLabel->setText(login);

    // Сброс счетчика сообщений (QLabel), предположим, что QLabel отображает счетчик
    for (int i = 0; i < ui->UsersVerticalLayout->count(); ++i) {
        QLayoutItem* item = ui->UsersVerticalLayout->itemAt(i);
        if (!item) {
            continue;
        }

        QLayout* hboxLayout = item->layout();
        if (hboxLayout) {
            // Перебираем виджеты внутри QHBoxLayout
            for (int j = 0; j < hboxLayout->count(); ++j) {
                QLayoutItem* hboxItem = hboxLayout->itemAt(j);
                if (!hboxItem) {
                    continue;
                }

                // Проверяем, является ли виджет кнопкой
                QPushButton* button = qobject_cast<QPushButton*>(hboxItem->widget());
                if (button && button->text() == login) {
                    // Найдена кнопка, теперь сбрасываем QLabel (счетчик сообщений)
                    QLabel* label = qobject_cast<QLabel*>(hboxLayout->itemAt(j + 1)->widget());
                    if (label) {
                        label->setText(" ");  // Сбрасываем текст метки
                    }
                    break;  // Прерываем цикл, так как нашли нужного пользователя
                }
            }
        }
    }

    // Отображение сообщений
    if (messages.isEmpty()) {
        ui->textWith->append("Пустой диалог!");
    } else {
        for (const QString& message : messages) {
            ui->textWith->append(message);
        }
    }

    db.saveChatWith(client_->getLogin(), client_->getToLogin());
    qDebug() << "Кто отправит: " << client_->getLogin();
    qDebug() << "Кому отправится: " << client_->getToLogin();
}

void MainWindow::onMessageReceived(const QString &sender, const QString &message, const bool& status)
{
    if (sender == client_->getToLogin())
    {
        // Отображение сообщения в основном чате
        ui->textWith->append(sender + ": " + message);
    }
    else
    {
        // Поиск соответствующей кнопки пользователя в списке
        for (int i = 0; i < ui->UsersVerticalLayout->count(); ++i) {
            QLayoutItem* item = ui->UsersVerticalLayout->itemAt(i);
            if (!item) {
                continue;  // Пропуск, если элемент не существует
            }

            QLayout* hboxLayout = item->layout();
            if (hboxLayout) {
                // Перебираем виджеты внутри QHBoxLayout
                for (int j = 0; j < hboxLayout->count(); ++j) {
                    QLayoutItem* hboxItem = hboxLayout->itemAt(j);
                    if (!hboxItem) {
                        continue;
                    }

                    // Проверяем, является ли виджет кнопкой
                    QPushButton* button = qobject_cast<QPushButton*>(hboxItem->widget());
                    if (button && button->text() == sender) {
                        // Найдена кнопка пользователя, теперь ищем QLabel для увеличения счетчика
                        QLabel* label = qobject_cast<QLabel*>(hboxLayout->itemAt(j + 1)->widget());
                        if (label) {
                            // Извлекаем текущее значение, преобразуем его в число и увеличиваем на 1
                            int currentValue = label->text().toInt();
                            label->setText(QString::number(currentValue + 1));
                        }
                        return;  // Прекращаем поиск, так как пользователь найден
                    }
                }
            }
        }
    }
}

