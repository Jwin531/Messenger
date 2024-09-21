#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QCoreApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QByteArray>

class Database
{
public:
    static Database& instance();  // Метод для получения единственного экземпляра
    ~Database();

    bool connectToDatabase();
    bool getUserData(const QString &username, const QString &password);
    bool setUserData(const QString &username, const QString &password);
    bool updateUserStatus(const QString &username, const QString status);
    bool saveMessage(const QString &username, const QString& message);

private:
    Database();  // Конструктор в private для предотвращения создания экземпляров
    Database(const Database&) = delete;  // Запрещаем копирование
    void operator=(const Database&) = delete;  // Запрещаем присваивание

    QSqlDatabase db;
};

#endif // DATABASE_H
