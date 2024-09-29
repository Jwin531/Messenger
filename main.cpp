#include "mainwindow.h"
#include "logindialog.h"
#include "database.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Database& db = Database::instance();
    if (!db.connectToDatabase()) {
        return -1;
    }

    LoginDialog loginDialog;
    MainWindow w;

    if (loginDialog.exec() == QDialog::Accepted) {
        w.show();
        return a.exec();
    } else {
        return 0;
    }
}
