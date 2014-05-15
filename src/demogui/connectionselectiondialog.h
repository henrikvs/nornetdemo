#ifndef CONNECTIONSELECTIONDIALOG_H
#define CONNECTIONSELECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectionSelectionDialog;
}

class ConnectionSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionSelectionDialog(QWidget *parent = 0);
    ~ConnectionSelectionDialog();

private:
    Ui::ConnectionSelectionDialog *ui;
};

#endif // CONNECTIONSELECTIONDIALOG_H
