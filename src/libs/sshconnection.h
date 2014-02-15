#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <QProcess>
#include <QObject>

class SSHConnection : public QObject
{
    Q_OBJECT
public:
    SSHConnection();
    SSHConnection(QString username, QString domain, QString password);
    //void connectOutput(QObject &receiver, const char *slot);
    void makeConnection();
    void execute(QString command);

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
