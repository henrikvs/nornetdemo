#include "nodeinfoform.h"
#include "ui_nodeinfoform.h"

NodeInfoForm::NodeInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeInfoForm)
{
    ui->setupUi(this);
}

NodeInfoForm::~NodeInfoForm()
{
    delete ui;
}
