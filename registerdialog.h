#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H
//
#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog() override;

signals:
    void registrationSuccessful(const QString &username);

private slots:
    void onSubmitButtonClicked();

private:
    void setToDatabase();

    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
