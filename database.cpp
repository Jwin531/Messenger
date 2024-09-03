#include "database.h"

Database::Database()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("messenger");
    db.setUserName("postgres");
    db.setPassword("G41z2pva");
}

Database::~Database()
{
    db.close();
}

bool Database::connectToDatabase()
{
    if (!db.open())
    {
        qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    QString authorization = R"(CREATE TABLE IF NOT EXISTS users_data (
                                id SERIAL PRIMARY KEY,
                                login VARCHAR(30) NOT NULL,
                                password VARCHAR(255) NOT NULL
                             );)";

    if (!query.exec(authorization))
    {
        qDebug() << "Ошибка создания таблицы users_data:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::setUserData(const QString &username,const QString &password)
{
    if (!db.isOpen() && !db.open()) {
        qDebug() << "База данных не открыта:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(INSERT INTO users_data (login, password) VALUES (:login, :password))");
    query.bindValue(":login", username);
    query.bindValue(":password", password);

    if(!query.exec())
    {
        qDebug() << "Не получилось сохранить данные пользователя в таблицу users_data:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::getUserData(const QString &username, const QString &password)
{
    if (!db.isOpen() && !db.open()) {
        qDebug() << "База данных не открыта:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(SELECT password FROM users_data WHERE login = :login)");
    query.bindValue(":login", username);

    if (!query.exec()) {
        qDebug() << "Не получилось выполнить запрос к таблице users_data:" << db.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();

        if (storedPassword == password) {
            return true;
        } else {
            qDebug() << "Пароли не совпадают";
            return false;
        }
    } else {
        qDebug() << "Пользователь не найден";
        return false;
    }
}
