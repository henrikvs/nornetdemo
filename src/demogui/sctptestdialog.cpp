#include "sctptestdialog.h"
#include "ui_sctptestdialog.h"
#include <QThread>
#include <QTimer>

SctpTestDialog::SctpTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SctpTestDialog)
{
    ui->setupUi(this);
        qDebug() << QThread::currentThread();
}

SctpTestDialog::~SctpTestDialog()
{
    delete ui;
}

void SctpTestDialog::on_startServerButton_clicked()
{
    QThread *thread = new QThread;
    server = new SctpServer();
    server->moveToThread(thread);
    connect(server, SIGNAL(newConnection(int)), this, SLOT(newConnection(int)));
    connect(thread, &QThread::started, [this]() {
        qDebug() << QThread::currentThread();
        server->listen(QStringList() << "fe80::92e6:baff:fef0:53fc", 33555);
    });
    thread->start();
}

void SctpTestDialog::on_startClientButton_clicked()
{
    QThread *thread = new QThread;
    socket = new SctpSocket();
    socket->moveToThread(thread);
    connect(thread, &QThread::started, [this]() {
        qDebug() << QThread::currentThread();
        socket->connectTo(QString("fe80::92e6:baff:fef0:53fc"), 33555, QStringList() << "fe80::92e6:baff:fef0:53fc");
    });
    thread->start();
}

void SctpTestDialog::on_addIpServerButton_clicked()
{
    newSocket->addAddress(ui->addressEdit->text());
}

void SctpTestDialog::on_removeIpServerButton_clicked()
{
    newSocket->removeAddress(ui->addressEdit->text());
}

void SctpTestDialog::newConnection(int sd)
{
    qDebug() << "New connection thread:" << QThread::currentThread();
    qDebug() << "Connected";
    newSocket = new SctpSocket(sd, 33555);

    QThread *thread = new QThread;
    newSocket->moveToThread(thread);
    connect(thread, &QThread::started, [this]() {
        qDebug() << QThread::currentThread();
        newSocket->readData();
    });
    thread->start();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        static int i = 0;
        i++;
        newSocket->write("Test");

    });
    timer->start(4000);
}

void SctpTestDialog::on_addIpClientButton_clicked()
{
    socket->addAddress(ui->addressEdit->text());
}

void SctpTestDialog::on_removeIpClientButton_clicked()
{
    socket->removeAddress(ui->addressEdit->text());
}
