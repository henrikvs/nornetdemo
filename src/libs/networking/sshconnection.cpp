#include "sshconnection.h"
#include <QThread>
SSHConnection::SSHConnection()
{

}

void SSHConnection::addHost(QString username, QString domain)
{
    SSHHost host;
    host.domain = domain;
    host.username = username;
    tunnels << host;
}

void SSHConnection::makeConnection()
{
    QStringList arguments = getArguments();
    QString program = getProgram();
    runProgram(program, arguments);

}

/*
void SSHConnection::connectOutput(QObject &receiver, const char *slot)
{
    QObject::connect(&process, SIGNAL(process->readyRead()), &receiver, slot);
}*/



void SSHConnection::runProgram(QString program, QStringList arguments)
{
    qDebug() << "running: " << program << arguments;
    QObject::connect(&process, SIGNAL(readyRead()), this, SLOT(newOutput()));
    QObject::connect(&process, SIGNAL(finished(int)), this, SLOT(handleFinished()));
    process.start(program, arguments);
}


void SSHConnection::executeScript(QString path)
{

}

void SSHConnection::runCommand(QString input)
{
    input = input + "\n";
    QByteArray array = input.toLocal8Bit();
    const char *chars = array.data();
    process.write(chars);
}

void SSHConnection::executeCommand(QString command) {
    QStringList arguments = getArguments();
    arguments << command;
    QString program = getProgram();
    runProgram(program, arguments);
}

/*
void SSHConnection::makeConnection2(QString username, QString domain, QString password = QString())
{

}
*/



QStringList SSHConnection::getArguments()
{

    QStringList arguments;
    #ifdef __linux
         arguments <<"-o" << "StrictHostKeyChecking=no";
    #endif
    for (int i = 0; i < tunnels.size(); i++) {
        if (i != 0) {
            arguments << "ssh";
            arguments <<"-o" << "StrictHostKeyChecking=no";
        }

        arguments << "-A";
        if (i != tunnels.size() - 1) {
            arguments << "-t";
        }
        arguments << tunnels[i].username + "@" + tunnels[i].domain;
    }
    return arguments;
}

QString SSHConnection::getProgram()
{
#ifdef __linux
    return "ssh";
#else
    return "C:/Users/Henrik/Downloads/plink.exe";
#endif
}



void SSHConnection::newOutput()
{
    QProcess *pro = (QProcess*) sender();
    QByteArray arr = pro->readAll();
    qDebug() << arr;
    emit outputSignal(arr);
}

void SSHConnection::handleFinished()
{
    emit disconnected();
}


