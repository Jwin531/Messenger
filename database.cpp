#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("messenger");
    db.setUserName("postgres");
    db.setPassword("G41z2pva");
}

Database::~Database() {
    db.close();
}

Database& Database::instance() {
    static Database instance;
    return instance;
}

QByteArray generateSalt(int length = 16) {
    QByteArray salt;
    for (int i = 0; i < length; ++i) {
        salt.append(QRandomGenerator::global()->generate() % 256);
    }
    return salt;
}

QByteArray hashPassword(const QString &password, const QByteArray &salt) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(salt);
    hash.addData(password.toUtf8());
    return hash.result();
}

bool verifyPassword(const QString &inputPassword, const QByteArray &storedSalt, const QByteArray &storedHash) {
    return hashPassword(inputPassword, storedSalt) == storedHash;
}

bool Database::connectToDatabase() {
    if (!db.open()) {
        qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    QString createUsersTable = R"(CREATE TABLE IF NOT EXISTS users (
        user_id SERIAL PRIMARY KEY,
        login VARCHAR(30) NOT NULL UNIQUE,
        status VARCHAR(10) NOT NULL DEFAULT 'offline',
        password BYTEA NOT NULL,
        salt BYTEA NOT NULL,
        session_id VARCHAR(255) UNIQUE
    );)";

    QString messages = R"(CREATE TABLE IF NOT EXISTS messages (
        id SERIAL PRIMARY KEY,
        sender_id VARCHAR(30),
        receiver_id VARCHAR(30),
        content TEXT NOT NULL,
        deliver_status BOOLEAN,
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );)";

    QString chats = R"(CREATE TABLE IF NOT EXISTS chats (
        id SERIAL PRIMARY KEY,
        user1_id INT REFERENCES users(user_id),
        user2_id INT REFERENCES users(user_id)
    );)";

    if (!query.exec(createUsersTable)) {
        qDebug() << "Ошибка создания таблицы users:" << query.lastError().text();
        return false;
    }

    if (!query.exec(messages)) {
        qDebug() << "Ошибка создания таблицы chats:" << query.lastError().text();
        return false;
    }

    if (!query.exec(chats)) {
        qDebug() << "Ошибка создания таблицы chat_messages:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::setUserData(const QString &username, const QString &password) {
    if (!db.isOpen() && !db.open()) {
        qDebug() << "База данных не открыта:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(INSERT INTO users (login, password, salt) VALUES (:login, :password, :salt))");
    query.bindValue(":login", username);
    QByteArray salt = generateSalt();
    query.bindValue(":salt", salt);
    query.bindValue(":password", hashPassword(password, salt));

    if (!query.exec()) {
        qDebug() << "Не удалось сохранить данные пользователя в таблицу users:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::getUserData(const QString &username, const QString &inputPassword) {
    if (!db.isOpen() && !db.open()) {
        qDebug() << "База данных не открыта:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(SELECT password, salt FROM users WHERE login = :login)");
    query.bindValue(":login", username);

    if (!query.exec()) {
        qDebug() << "Ошибка запроса к базе данных:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QByteArray storedHash = query.value("password").toByteArray();
        QByteArray storedSalt = query.value("salt").toByteArray();

        if (verifyPassword(inputPassword, storedSalt, storedHash)) {
            qDebug() << "Пароль верный!";
            return true;
        } else {
            qDebug() << "Пароль неверный!";
            return false;
        }
    } else {
        qDebug() << "Пользователь не найден!";
        return false;
    }
}

bool Database::updateUserStatus(const QString& username, const QString status) {
    QSqlQuery query(db);
    query.prepare(R"(UPDATE users SET status = :status WHERE login = :login)");
    query.bindValue(":login", username);
    query.bindValue(":status", status);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления статуса пользователя:" << username << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::saveMessage(const QString &username, const QString& message) {
    QSqlQuery query(db);
    query.prepare(R"(INSERT INTO chat_messages (sender_id, message) VALUES((SELECT user_id FROM users WHERE login = :username), :message))");
    query.bindValue(":username", username);
    query.bindValue(":message", message);

    if (!query.exec()) {
        qDebug() << "Ошибка при сохранении сообщения:" << query.lastError();
        return false;
    }
    return true;
}

bool Database::saveSessionId(const QString& sessionId,const QString& username)
{
    QSqlQuery query(db);
    query.prepare(R"(UPDATE users SET session_id = :sessionId WHERE login = :login)");
    query.bindValue(":login", username);
    query.bindValue(":sessionId",sessionId);

    if (!query.exec()) {
        qDebug() << "Ошибка обнновления id сессии:" << query.lastError();
        return false;
    }
    return true;
}

bool Database::resetSessionId(const QString& username) {
    QSqlQuery query(db);
    query.prepare(R"(UPDATE users SET session_id = NULL WHERE login = :login)");
    query.bindValue(":login", username);

    if (!query.exec()) {
        qDebug() << "Ошибка сброса id сессии:" << query.lastError();
        return false;
    }
    return true;
}

QVector<QString> Database::takeAllMessagesFromThisChat(const QString& receiver, const QString& sender)
{
    QVector<QString> messages;
    QSqlQuery query(db);

    query.prepare(R"(SELECT content FROM messages WHERE sender_id = :sender AND receiver_id = :receiver)");
    query.bindValue(":receiver", receiver);
    query.bindValue(":sender", sender);

    if (query.exec()) {
        while (query.next()) {
            QString message = query.value(0).toString();
            messages.append(message);
        }
    } else {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }

    return messages;
}

