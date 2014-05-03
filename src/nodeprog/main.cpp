#include <QCoreApplication>
#include <QTextStream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTranslator>
#include "shell.h"
#include "nodeprog.h"
#include <QThread>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("nodeprog");
    QCoreApplication::setApplicationVersion(QString::number(NodeProg::VERSION));
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOption(QStringList() << "p" << "port", QCoreApplication::tr("Port to listen on"), QCoreApplication::tr("port"), "33555");
    QCommandLineOption nameOption(QStringList() << "n" << "name", QCoreApplication::tr("The name of this node"), QCoreApplication::tr("nodeName"), "default");
    QCommandLineOption relayOption(QStringList() << "r" << "relay", QCoreApplication::tr("Start in relay mode"), QCoreApplication::tr("host:port"));
    parser.addOption(portOption);
    parser.addOption(nameOption);
    parser.addOption(relayOption);

    parser.process(a);

    QString port = parser.value(portOption);
    QString name = parser.value(nameOption);
    QString relay = parser.value(relayOption);
    NodeProg nodeprog;
    qDebug() << "Name:" << name;
    qDebug() << "Port" << port;

    if (!relay.isEmpty()) {
        QStringList list = relay.split(":");
        QString relayHost = list[0];
        QString relayPort = list[1];
        qDebug() << "Relay mode enabled:" << relayHost << relayPort;
        nodeprog.enableRelay(relayHost, relayPort.toInt());
    }
    nodeprog.setName(name);
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
