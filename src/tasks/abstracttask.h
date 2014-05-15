#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H
#include <QObject>

class AbstractTask : public QObject
{
    Q_OBJECT
public:
    AbstractTask(int taskId, QObject *parent = 0);
    virtual ~AbstractTask();
    virtual void killTask() = 0;
    virtual void stopTask() = 0;
protected:
    int taskId;
signals:
    void finished();
};

#endif // ABSTRACTTASK_H
