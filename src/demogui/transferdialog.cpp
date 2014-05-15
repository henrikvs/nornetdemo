#include "transferdialog.h"
#include "transferrequestmessage.h"
#include "ui_transferdialog.h"

TransferDialog::TransferDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransferDialog)
{
    ui->setupUi(this);

}

TransferDialog::~TransferDialog()
{
    delete ui;
}

void TransferDialog::on_pushButton_clicked()
{
    TransferTask *task = new TransferTask(0, TransferRequestMessage::TRANSFER_TYPE_TCP, "localhost", "localhost", 3,33555, this);
    connect(task, SIGNAL(newStatus(TransferStatusMessage&)), this, SLOT(newStatus(TransferStatusMessage&)));
    connect(task, &TransferTask::finished, []() {
        qDebug() << "We're done";
    });
    task->start();
}

void TransferDialog::newStatus(TransferStatusMessage &message)
{
    qDebug() << "new status";
    ui->textEdit->append(message.data.mbps);
}
