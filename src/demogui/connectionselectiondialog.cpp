#include "connectionselectiondialog.h"
#include "ui_connectionselectiondialog.h"

ConnectionSelectionDialog::ConnectionSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionSelectionDialog)
{
    ui->setupUi(this);
}

ConnectionSelectionDialog::~ConnectionSelectionDialog()
{
    delete ui;
}
