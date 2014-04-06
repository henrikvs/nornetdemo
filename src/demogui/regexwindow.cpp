#include <QObject>
#include <QDebug>
#include "ui_regexwindow.h"
#include "regexwindow.h"

RegexWindow::RegexWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegexWindow)
{
    ui->setupUi(this);
    connect(&interp, SIGNAL(signalNewMatch(QStringList,QString)),this, SLOT(newResult(QStringList,QString)));
}

RegexWindow::~RegexWindow()
{
    delete ui;
}

void RegexWindow::on_pushButton_clicked()
{
    ui->resultText->clear();
    interp.slotNewInput(ui->textText->toPlainText());

}

void RegexWindow::newResult(QStringList results, QString key)
{
    ui->resultText->append(key + ": ");
    for (int i = 1 ; i < results.size(); ++i) {
        ui->resultText->append(results.at(i));
    }
    qDebug() << "new res";
}

void RegexWindow::on_pushButton_2_clicked()
{
    interp.addRegex(ui->regexText->toPlainText(), ui->keyText->toPlainText());
    ui->regexText->clear();
    ui->keyText->clear();
}
