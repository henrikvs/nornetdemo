#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <QProcess>
#include <QObject>
#include <QDebug>
class SSHConnection : public QObject
{
    Q_OBJECT
public:
    SSHConnection();
    //void connectOutput(QObject &receiver, const char *slot);
    void makeConnection(QString username, QString domain, QString password);
    void execute(QString command);
    QString readAll();


private:
    QProcess process;
    QString program;
    QString username;
    QString domain;
    QString password;
    bool loggedIn;

signals:
    void outputSignal(QString output);
private slots:
    void newOutput();
};

#endif // SSHCONNECTION_H
