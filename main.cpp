#include "mainwindow.h"
#include "logindialog.h"
#include "database.h"
#include <QApplication>
#include <thread>
#include "Server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database& db = Database::instance();  // Используем singleton экземпляр
    if (!db.connectToDatabase()) {
        return -1;
    }

    LoginDialog loginDialog;

    QObject::connect(&loginDialog, &LoginDialog::loginSuccessful, [](const QString &username) {
        qDebug() << "Пользователь вошел в систему:" << username;
    });

    if(loginDialog.exec() == QDialog::Accepted)
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
