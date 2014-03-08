#ifndef INFOTASK_H
#define INFOTASK_H
#include <QObject>
#include "nodeinfomessage.h"
/*!
 * \brief Used to collect information about the machine it's running on.
 * The result is extracted by connecting to the ::newInfoMessage signal.
 */
class InfoTask : public QObject
{
    Q_OBJECT
public:
    explicit InfoTask(QObject *parent = 0);
public slots:
    void start();
signals:
    void newInfoMessage(NodeInfoMessage *message);
    void finished();
};

#endif // STATUSTASK_H
