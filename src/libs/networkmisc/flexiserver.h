/*#ifndef FLEXISERVER_H
#define FLEXISERVER_H

#include <QObject>
#include <QThread>

//Q_DECLARE_METATYPE(qintptr);

class FlexiServer : public QObject
{
    Q_OBJECT
public:
    explicit FlexiServer(QObject *parent = 0);
    void listenAsync();
    void setPort(int port);
    void stopListening();
    void listConnections();


signals:
    void newConnection(qintptr socketDescriptor);
    void error(QString error);
    void finished();
private:
    void* get_in_addr(struct sockaddr *sa);
    int port;
    QString ip;
     int sockfd;
    static const int BACKLOG = 10;
public slots:
        void listen();

};

#endif // FLEXISERVER_H
*/
