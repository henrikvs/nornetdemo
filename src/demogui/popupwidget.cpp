#include "popupwidget.h"
#include "ui_popupwidget.h"

PopupWidget::PopupWidget(QString text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupWidget)
{
    ui->setupUi(this);
    ui->textLabel->setText(text);
    setAttribute(Qt::WA_ShowWithoutActivating);
}

PopupWidget::~PopupWidget()
{
    delete ui;
}
