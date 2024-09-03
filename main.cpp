#include "mainwindow.h"
#include "logindialog.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database db;
    if (!db.connectToDatabase()) {
        return -1;
    }

    LoginDialog loginDialog;
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

