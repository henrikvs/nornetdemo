#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QList>
#include "regexhelper.h"
struct SSHHost {
    QString username;
    QString domain;
    QString password;
};

class SSHConnection : public QObject
{
    Q_OBJECT
public:
    SSHConnection();
    //void connectOutput(QObject &receiver, const char *slot);
    void addHost(QString username, QString domain);
    void makeConnection();
    void executeCommand(QString command);
    void executeScript(QString path);
    void runCommand(QString input);
private:
    void runProgram(QString program, QStringList arguments);

private:
    QStringList getArguments();
    QString getProgram();
    QProcess process;
    QString program;
    QString username;
    QString domain;
    QList<SSHHost> tunnels;


signals:
    void outputSignal(QString output);
    void connected();
private slots:
    void newOutput();
};

#endif // SSHCONNECTION_H
