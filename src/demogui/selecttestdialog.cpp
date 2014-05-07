#include "selecttestdialog.h"
#include "ui_selecttestdialog.h"
#include <QRadioButton>
#include <QDebug>

SelectTestDialog::SelectTestDialog(bool ipv4, bool ipv6, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTestDialog)
{
    ui->setupUi(this);

    if (ipv6) {
        QRadioButton *ipv6RadioOption = new QRadioButton("IPv6", this);
        ipGroup.addButton(ipv6RadioOption);
        ui->ipSelectionLayout->addWidget(ipv6RadioOption);
        ipv6RadioOption->setChecked(true);

    }

    if (ipv4) {
        QRadioButton *ipv4RadioOption = new QRadioButton("IPv4", this);
        ipGroup.addButton(ipv4RadioOption);
        ui->ipSelectionLayout->addWidget(ipv4RadioOption);
        //ui->ipSelectionLayout->addWidget();
        ipv4RadioOption->setChecked(true);
    }
}

int SelectTestDialog::getConnectionType()
{
    return connectionType;
}

int SelectTestDialog::getTestType()
{
    return testType;
}

SelectTestDialog::~SelectTestDialog()
{
    delete ui;
}

void SelectTestDialog::on_buttonBox_accepted()
{
    QAbstractButton *checked = ipGroup.checkedButton();
    if (checked == NULL) {
        qDebug() << "No option is checked";
        return;
    }
    qDebug() << "Checked: " << checked->text();
    if (checked->text() == "IPv4") {
        connectionType = CONNECTION_IPV4;
    } else if (checked->text() == "IPv6") {
        connectionType = CONNECTION_IPV6;
    }

    QString test = ui->testComboBox->currentText();
    if (test == "TCP bandwidth") {
        testType = TEST_TCP;
    } else if (test == "Ping") {
        testType = TEST_PING;
    } else {
        testType = 0;
    }
}
