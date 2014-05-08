#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H
#include <QObject>

class AbstractTask : public QObject
{
    Q_OBJECT
public:
    AbstractTask(int taskId, QObject *parent = 0);
    virtual ~AbstractTask();
protected:
    int taskId;
};

#endif // ABSTRACTTASK_H
