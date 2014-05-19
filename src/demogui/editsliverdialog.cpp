#include "editsliverdialog.h"
#include "ui_editsliverdialog.h"

EditSliverDialog::EditSliverDialog(Sliver *sliver, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditSliverDialog)
{
    ui->setupUi(this);

    ui->siteNameEdit->setText(sliver->hostName);
    ui->addressEdit->setText(sliver->IPv6);
    ui->sliceNameEdit->setText(sliver->sliceName);
    ui->portEdit->setText(QString::number(sliver->port));


    localSliver.hostName = sliver->hostName;
    localSliver.infoMessage = sliver->infoMessage;
    localSliver.IPv4 = sliver->IPv4;
    localSliver.IPv6 = sliver->IPv6;
    localSliver.name = sliver->name;
    localSliver.sliceName = sliver->sliceName;
    localSliver.status = sliver->status;
    localSliver.port = sliver->port;
}

EditSliverDialog::~EditSliverDialog()
{
    delete ui;
}

Sliver EditSliverDialog::getSliver()
{
    return localSliver;
}

void EditSliverDialog::on_buttonBox_accepted()
{
    localSliver.hostName = ui->siteNameEdit->text();
    localSliver.IPv6 = ui->addressEdit->text();
    localSliver.name = ui->siteNameEdit->text();
    localSliver.port = ui->portEdit->text().toInt();
    localSliver.sliceName = ui->sliceNameEdit->text();
}
