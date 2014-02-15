#include "sshconnection.h"

SSHConnection::SSHConnection()
{
}

SSHConnection::SSHConnection(QString username, QString domain, QString password = QString())
{
    loggedIn = false;
    this->program = "C:/Users/Henrik/Downloads/plink.exe";
    this->username = username;
    this->domain = domain;
    this->password = password;
}

/*
void SSHConnection::connectOutput(QObject &receiver, const char *slot)
{
    QObject::connect(&process, SIGNAL(process->readyRead()), &receiver, slot);
}*/

void SSHConnection::makeConnection()
{
    QStringList arguments;
    arguments << username + "@" + domain;
    process.setParent(this);
    QObject::connect(&process, SIGNAL(process->readyRead()), this, SLOT(newOutput()));

    process.start(program, arguments);

    if (password.isNull()) {

    } else {
        process.waitForStarted();
        process.write(password.toLocal8Bit().data());
        process.write("\n");
    }

}

void SSHConnection::execute(QString command)
{

}

void SSHConnection::newOutput()
{
    emit outputSignal(process.readAll());
}


