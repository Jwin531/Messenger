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

Database& Database::instance() {
    static Database instance;  // Статическая локальная переменная гарантирует, что она будет создана один раз
    return instance;
}

QByteArray generateSalt(int length = 16)
{
    QByteArray salt;
    for (int i = 0; i < length; ++i) {
        salt.append(QRandomGenerator::global()->generate() % 256);
    }
    return salt;
}

QByteArray hashPassword(const QString &password, const QByteArray &salt)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(salt);
    hash.addData(password.toUtf8());
    return hash.result();
}

bool verifyPassword(const QString &inputPassword, const QByteArray &storedSalt, const QByteArray &storedHash) {
    QByteArray hashedInputPassword = hashPassword(inputPassword, storedSalt);
    return hashedInputPassword == storedHash;
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
                                status VARCHAR(10) NOT NULL DEFAULT 'offline',
                                password BYTEA NOT NULL,
                                salt BYTEA NOT NULL
                             );)";

    if (!query.exec(authorization))
    {
        qDebug() << "Ошибка создания таблицы users_data:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::setUserData(const QString &username, const QString &password)
{
    if (!db.isOpen() && !db.open()) {
        qDebug() << "База данных не открыта:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(INSERT INTO users_data (login, password, salt) VALUES (:login, :password, :salt))");
    query.bindValue(":login", username);
    QByteArray salt = generateSalt();
    query.bindValue(":salt", salt); // Используем бинарный формат для соли
    QByteArray hashedPassword = hashPassword(password, salt);
    query.bindValue(":password", hashedPassword); // Используем бинарный формат для хэша

    if(!query.exec()) {
        qDebug() << "Не получилось сохранить данные пользователя в таблицу users_data:" << query.lastError().text();
        return false;
    }
    return true;
}


bool Database::getUserData(const QString &username, const QString &inputPassword)
{
    if (!db.isOpen() && !db.open()) {
        qDebug() << "База данных не открыта:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(SELECT password, salt FROM users_data WHERE login = :login)");
    query.bindValue(":login", username);

    // Выполняем запрос
    if (!query.exec()) {
        qDebug() << "Ошибка запроса к базе данных:" << query.lastError().text();
        return false;
    }

    // Проверяем, есть ли результат
    if (query.next()) {
        QByteArray storedHash = query.value("password").toByteArray(); // Получаем хэш пароля как бинарные данные
        QByteArray storedSalt = query.value("salt").toByteArray(); // Получаем соль как бинарные данные

        // Проверяем пароль
        if (verifyPassword(inputPassword, storedSalt, storedHash)) {
            qDebug() << "Password is correct!";
            return true;
        } else {
            qDebug() << "Password is incorrect!";
            return false;
        }
    } else {
        qDebug() << "User not found!";
        return false;
    }
}

bool Database::updateUserStatus(const QString& username,const QString status)
{
    QSqlQuery query(db);
    query.prepare(R"(UPDATE users_data SET status = :status WHERE login = :login)");
    query.bindValue(":login", username);
    query.bindValue(":status",status);

    if(!query.exec())
    {
        qDebug() << "Ошибка обновления статуса пользователя:"<< username << query.lastError().text();
        return false;
    }
    return true;
}

