#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>
#include <QVector>

class Database {
public:
    static Database& instance();
    bool connectToDatabase();
    bool setUserData(const QString &username, const QString &password);
    bool getUserData(const QString &username, const QString &inputPassword);
    QVector<QString> takeAllMessagesFromThisChat(const QString& receiver, const QString& sender);
    bool saveMessage(const QString& message, const QString& reciever, const QString& sender);


private:
    Database();
    ~Database();
    QSqlDatabase db;
};

#endif // DATABASE_H
