#ifndef EDITSLIVERDIALOG_H
#define EDITSLIVERDIALOG_H
#include "sliver.h"
#include <QDialog>

namespace Ui {
class EditSliverDialog;
}

class EditSliverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSliverDialog(Sliver *sliver, QWidget *parent = 0);
    ~EditSliverDialog();
    Sliver getSliver();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditSliverDialog *ui;
    Sliver localSliver;
};

#endif // EDITSLIVERDIALOG_H
