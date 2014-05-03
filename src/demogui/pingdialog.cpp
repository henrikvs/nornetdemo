#include "pingdialog.h"
#include "ui_pingdialog.h"
#include "pingtask.h"
#include "pingreply.h"

PingDialog::PingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PingDialog)
{
    ui->setupUi(this);


}

PingDialog::~PingDialog()
{
    delete ui;
}

void PingDialog::on_pingButton_clicked()
{
    QString ip = ui->pingText->toPlainText();
    PingTask *task = new PingTask(0, ip, "TODO",  this);
    connect(task, &PingTask::newPing, this, &PingDialog::pingReplySlot);
    task->start();

}

void PingDialog::pingReplySlot(const AbstractMessage &reply)
{
    qDebug() << "new ping";
    //ui->pingOutputText->append(reply->data.ms);
}
