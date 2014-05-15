#include "networklibglobal.h"

#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QList>

struct SSHHost {
    QString username;
    QString domain;
    QString password;
};

class NETWORKLIB_EXPORT SSHConnection : public QObject
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
    void disconnected();
private slots:
    void newOutput();
    void handleFinished();

};

#endif // SSHCONNECTION_H
