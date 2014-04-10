#include "abstracttask.h"
#include <QDebug>

AbstractTask::AbstractTask(QObject *parent) : QObject(parent)
{

}

AbstractTask::~AbstractTask()
{
    qDebug() << "Destroyed abstract task";
}
