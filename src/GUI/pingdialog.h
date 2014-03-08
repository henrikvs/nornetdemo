#ifndef PINGDIALOG_H
#define PINGDIALOG_H

#include <QDialog>
#include "pingreply.h"
namespace Ui {
class PingDialog;
}

class PingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PingDialog(QWidget *parent = 0);
    ~PingDialog();

private slots:
    void on_pingButton_clicked();
    void pingReplySlot(AbstractMessage *reply);

private:
    Ui::PingDialog *ui;
};

#endif // PINGDIALOG_H
