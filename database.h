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
    Database();
    ~Database();
    bool connectToDatabase();
    bool getUserData(const QString &username,const QString &password);
    bool setUserData(const QString &username, const QString &password);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
