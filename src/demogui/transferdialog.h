#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include "transfertask.h"
#include "transferstatusmessage.h"

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferDialog(QWidget *parent = 0);
    ~TransferDialog();

private slots:
    void on_pushButton_clicked();
    void newStatus(TransferStatusMessage &message);

private:
    Ui::TransferDialog *ui;
};

#endif // TRANSFERDIALOG_H
