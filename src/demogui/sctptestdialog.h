#ifndef SCTPTESTDIALOG_H
#define SCTPTESTDIALOG_H

#include <QDialog>
#include "sctpserver.h"
#include "sctpsocket.h"
namespace Ui {
class SctpTestDialog;
}

class SctpTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SctpTestDialog(QWidget *parent = 0);
    ~SctpTestDialog();

private slots:
    void on_startServerButton_clicked();

    void on_startClientButton_clicked();

    void on_addIpServerButton_clicked();

    void on_removeIpServerButton_clicked();

    void newConnection(int sd);

    void on_addIpClientButton_clicked();

    void on_removeIpClientButton_clicked();

private:
    Ui::SctpTestDialog *ui;
    SctpSocket *socket;
    SctpServer *server;
    SctpSocket *newSocket;
};

#endif // SCTPTESTDIALOG_H
