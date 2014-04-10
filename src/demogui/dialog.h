#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <QProcess>
#include "sshconnection.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_connectButton_clicked();
    void newOutput(QString output);

    void on_enterButton_clicked();

private:
    Ui::Dialog *ui;
    SSHConnection connection;
};

#endif // DIALOG_H
