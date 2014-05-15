#include <QCoreApplication>
#include <QTextStream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTranslator>
#include "shell.h"
#include "signal.h"
#include "nodeprog.h"
#include <QThread>
#include <QDebug>
#include <QTimer>
volatile sig_atomic_t sigint = 0;
void signalHandler(int sig) {
    sigint = 1;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("nodeprog");
    QCoreApplication::setApplicationVersion(QString::number(NodeProg::VERSION));
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOption(QStringList() << "p" << "port", QCoreApplication::tr("Port to listen on"), QCoreApplication::tr("port"), QString::number(NetworkEntity::PORT));
    QCommandLineOption relayOption(QStringList() << "r" << "relay", QCoreApplication::tr("Start in relay mode"), QCoreApplication::tr("host:port"));
    QCommandLineOption iperfIpv4portOption(QStringList() << "4" << "iperf-port-ipv4", QCoreApplication::tr("The port iperf will listen to Ipv4 requests on"), QCoreApplication::tr("port"), QString::number(NetworkEntity::IPERF_IPV4PORT));
    QCommandLineOption iperfIpv6portOption(QStringList() << "6" << "iperf-port-ipv6", QCoreApplication::tr("The port iperf will listen to Ipv6 requests on"), QCoreApplication::tr("port"), QString::number(NetworkEntity::IPERF_IPV6PORT));
    parser.addOption(portOption);
    parser.addOption(relayOption);

    parser.addOption(iperfIpv4portOption);
    parser.addOption(iperfIpv6portOption);

    parser.process(a);

    QString port = parser.value(portOption);
    int iperf4Port = parser.value(iperfIpv4portOption).toInt();
    int iperf6Port = parser.value(iperfIpv6portOption).toInt();
    QString relay = parser.value(relayOption);
    NodeProg nodeprog;
    nodeprog.setIperf4Port(iperf4Port);
    nodeprog.setIperf6Port(iperf6Port);
    //for shutting down the program
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&nodeprog]() {
        if (sigint) {
            sigint = 1;
            nodeprog.shutDown(NetworkEntity::EXIT_TYPE_NORMAL);
        }
    });
    signal(SIGINT, signalHandler);
    timer.start(1000);
    //QObject::connect(&nodeprog,SIGNAL(shutDownComplete(int)), &nodeprog, SLOT(shutDown(int)

    qDebug() << "Port" << port;

    if (!relay.isEmpty()) {
        QStringList list = relay.split(":");
        QString relayHost = list[0];
        QString relayPort = list[1];
        qDebug() << "Relay mode enabled:" << relayHost << relayPort;
        nodeprog.enableRelay(relayHost, relayPort.toInt());
    }
    nodeprog.start(port.toInt());
    //Shell shell;
    //QThread thread;
    //shell.moveToThread(&thread);
    //QObject::connect(&thread, SIGNAL(started()), &shell, SLOT(start()));
    //QObject::connect(&shell, SIGNAL(newOutput(QString)), &nodeprog, SLOT(newStdIn(QString)), Qt::QueuedConnection);

    //QObject::connect(&shell, SIGNAL(shellClosing()), &thread, SLOT(quit()));
    //QObject::connect(&thread, SIGNAL(finished()), &nodeprog, SLOT(shutDown()));
    //thread.start();
    qDebug()<< "Started";
    return a.exec();
}
