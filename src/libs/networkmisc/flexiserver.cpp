//Adapted from http://beej.us/guide/bgnet/output/html/multipage/clientserver.html#simpleserver
/*
#include <QDebug>
#include "flexiserver.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

FlexiServer::FlexiServer(QObject *parent) :
    QObject(parent)
{

}

// get sockaddr, IPv4 or IPv6:
void * FlexiServer::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void FlexiServer::listConnections()
{
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, "7777", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        emit error("Error getting address info");
        return;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        qDebug() << p->ai_protocol << p->ai_canonname << p->ai_addr << p->ai_addrlen << "\n";
    }
}

void FlexiServer::listen()
{
    QString portQString = QString::number(port);
    const char* portChar = portQString.toStdString().c_str();
    int new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, portChar, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        emit error("Error getting address info");
        return;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            emit error("Error setting socket options");
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            shutdown(sockfd, 2);
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        emit error("Problem binding to connection");
        return;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (::listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        emit error("Error while listening");
        return;
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            emit error("Error during accept");
            emit finished();
            return;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        emit newConnection((qintptr)new_fd);
    }

}

void FlexiServer::listenAsync()
{
    QThread *thread = new QThread(this);
    QObject::connect(thread, SIGNAL(started()), this, SLOT(listen()), Qt::DirectConnection);
    thread->start();
    qDebug() <<"Started";

}

void FlexiServer::setPort(int port)
{
    this->port = port;
}

void FlexiServer::stopListening()
{
    close(sockfd);
    qDebug() << "closed socket";
}
*/
