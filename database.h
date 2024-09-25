#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>

class Database {
public:
    static Database& instance();
    bool connectToDatabase();
    bool setUserData(const QString &username, const QString &password);
    bool getUserData(const QString &username, const QString &inputPassword);
    bool updateUserStatus(const QString& username, const QString status);
    bool saveMessage(const QString &username, const QString& message);
    bool saveSessionId(const QString& sessionId,const QString& username);
    bool resetSessionId(const QString& username);

private:
    Database();
    ~Database();
    QSqlDatabase db;
};

#endif // DATABASE_H
