#include "settingsdialog.h"
#include "settings.h"
#include "slicemanager.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QString>
#include <QLineEdit>
#include <QInputDialog>
#include <QSettings>
#include <QMenu>
#include <QTableWidget>
#include "networkentity.h"
#include "editsliverdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    QSettings settings(Settings::programName, Settings::company);
    bool gatekeeperEnabled = settings.value(Settings::gatekeeperEnabled, true).toBool();
    QString gatekeeperUsername = settings.value(Settings::gatekeeperUsername, QString()).toString();
    QString gatekeeperHostname = settings.value(Settings::gatekeeperHostname, QString()).toString();
    QString sliceName = settings.value(Settings::sliceName, QString()).toString();

    bool relayEnabled = settings.value(Settings::relayEnabled, false).toBool();
    QString relayHostname = settings.value(Settings::relayHostname, QString()).toString();
    QString relayPort = settings.value(Settings::relayPort, QString()).toString();
    QString nodeprogRootUrl = settings.value(Settings::nodeprogRootUrl, QString()).toString();

    ui->setupUi(this);

    ui->gatekeeperBox->setChecked(gatekeeperEnabled);
    ui->usernameEdit->setText(gatekeeperUsername);
    ui->hostnameEdit->setText(gatekeeperHostname);

    ui->relayHostnameEdit->setText(relayHostname);
    ui->relayPortEdit->setText(relayPort);
    ui->relayBox->setChecked(relayEnabled);
    ui->sliceNameEdit->setText(sliceName);
    ui->rootUrlEdit->setText(nodeprogRootUrl);
    sliverKeyEdited = false;
    gatekeeperKeyEdited = false;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/*
void SettingsDialog::on_sliverKeyBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select private key");
    ui->sliverKeyEdit->setText(fileName);
}

void SettingsDialog::on_gatekeeperKeyBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select private key");
    ui->gatekeeperKeyEdit->setText(fileName);
}*/

void SettingsDialog::on_okButton_clicked()
{


    QSettings settings(Settings::programName, Settings::company);
    settings.setValue(Settings::gatekeeperEnabled, ui->gatekeeperBox->isChecked());
    settings.setValue(Settings::gatekeeperUsername, ui->usernameEdit->text());
    settings.setValue(Settings::gatekeeperHostname, ui->hostnameEdit->text());

    settings.setValue(Settings::relayEnabled, ui->relayBox->isChecked());
    settings.setValue(Settings::relayHostname, ui->relayHostnameEdit->text());
    settings.setValue(Settings::relayPort, ui->relayPortEdit->text());
    settings.setValue(Settings::sliceName, ui->sliceNameEdit->text());
    settings.setValue(Settings::nodeprogRootUrl, ui->rootUrlEdit->text());
    close();
}

void SettingsDialog::on_cancelButton_clicked()
{
    close();
}

/*
void SettingsDialog::on_sliverKeyEdit_textChanged(const QString &arg1)
{
    sliverKeyEdited = true;
}

void SettingsDialog::on_gatekeeperKeyEdit_textChanged(const QString &arg1)
{
    gatekeeperKeyEdited = true;
}



void SettingsDialog::on_addHostButton_clicked()
{
    bool ok;
    QString hostname = QInputDialog::getText(this, tr("Settings"), tr("Enter hostname"),QLineEdit::Normal,QString(), &ok);

    if (!ok) return;

    if (!Settings::sliceManager.sliverExists(hostname)) {
        qDebug() << "host does not exist, adding";
        QListWidgetItem *item = new QListWidgetItem(hostname, ui->sliversWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        Settings::sliceManager.createSliver(hostname, NetworkEntity::PORT);
    } else {
        qDebug() << "Host already exists";
    }

}

void SettingsDialog::on_deleteHostButton_clicked()
{
    QList<QListWidgetItem*> items = ui->sliversWidget->selectedItems();
    foreach (QListWidgetItem *item, items) {
        qDebug() << "Removing " << item->text();
        Settings::sliceManager.removeSliver(item->text());
        delete item;
    }
}

void SettingsDialog::on_editSiteButton_clicked()
{
    QList<QListWidgetItem*> items = ui->sliversWidget->selectedItems();
    foreach (QListWidgetItem *item, items) {
        qDebug() << "Editing" << item->text();
        Sliver *sliver = Settings::sliceManager.getSliver(item->text());
        EditSliverDialog *dialog = new EditSliverDialog(sliver);
        dialog->exec();
        Sliver edittedSliver = dialog->getSliver();
        //qDebug() << edittedSliver->name << edittedSliver->sliceName << edittedSliver->IPv6 << edittedSliver->port;
        Settings::sliceManager.editSliver(sliver->hostName, edittedSliver);
        item->setText(edittedSliver.hostName);
qDebug() << "Test";
}
    }*/

