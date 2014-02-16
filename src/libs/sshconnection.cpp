#include "sshconnection.h"

SSHConnection::SSHConnection()
{
}


/*
void SSHConnection::connectOutput(QObject &receiver, const char *slot)
{
    QObject::connect(&process, SIGNAL(process->readyRead()), &receiver, slot);
}*/

void SSHConnection::makeConnection(QString username, QString domain, QString password = QString())
{
    loggedIn = false;
    this->program = "C:/Users/Henrik/Downloads/plink.exe";
    QStringList arguments;
    arguments << username + "@" + domain;
    process.setParent(this);
    QObject::connect(&process, SIGNAL(readyRead()), this, SLOT(newOutput()));

    process.start(program, arguments);

    if (password.isNull()) {

    } else {
        process.waitForStarted();
        execute(password);
    }

}

void SSHConnection::execute(QString command)
{
    QByteArray array = command.toLocal8Bit();
    const char *chars = array.data();
    process.write(chars);
    process.write("\n");
}

QString SSHConnection::readAll()
{
    QByteArray arr = process.readAll();
    qDebug() << arr;
    return arr;
}

void SSHConnection::newOutput()
{
    emit outputSignal(process.readAll());
}


