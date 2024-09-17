#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

signals:
    void registrationSuccessful(const QString &username);

public slots:
    void setToDatabase();
    void onSubmitButtonClicked();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
