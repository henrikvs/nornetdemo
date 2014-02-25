#include "dnsform.h"
#include "ui_dnsform.h"
#include <QtNetwork/QDnsLookup>

DNSForm::DNSForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DNSForm)
{
    ui->setupUi(this);
}

DNSForm::~DNSForm()
{
    delete ui;
}

void DNSForm::on_pushButton_clicked()
{

}

void MyObject::lookupServers()
{
    // Create a DNS lookup.
    dns = new QDnsLookup(this);
    connect(dns, SIGNAL(finished()),
            this, SLOT(handleServers()));

    // Find the XMPP servers for gmail.com
    dns->setType(QDnsLookup::LOC);
    dns->setName("_xmpp-client._tcp.gmail.com");
    dns->lookup();
}


void MyObject::handleServers()
{
    // Check the lookup succeeded.
    if (dns->error() != QDnsLookup::NoError) {
        qWarning("DNS lookup failed");
        dns->deleteLater();
        return;
    }

    // Handle the results.
    foreach (const QDnsServiceRecord &record, dns->serviceRecords()) {
        ...
    }
    dns->deleteLater();
}
