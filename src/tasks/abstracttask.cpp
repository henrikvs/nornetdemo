#include "abstracttask.h"
#include <QDebug>

AbstractTask::AbstractTask(int taskId, QObject *parent) : QObject(parent), taskId(taskId)
{

}

AbstractTask::~AbstractTask()
{
    qDebug() << "Destroyed abstract task";
}

