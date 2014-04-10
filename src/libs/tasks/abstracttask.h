#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H
#include <QObject>

class AbstractTask : public QObject
{
    Q_OBJECT
public:
    AbstractTask(QObject *parent = 0);
    virtual ~AbstractTask();
};

#endif // ABSTRACTTASK_H
