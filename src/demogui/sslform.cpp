#include <QDebug>

#include "sslform.h"
#include "ui_sslform.h"

SSLForm::SSLForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SSLForm)
{
    ui->setupUi(this);
}

SSLForm::~SSLForm()
{
    delete ui;
}

void SSLForm::on_connectButton_clicked()
{
    qDebug() << "Test";
}
