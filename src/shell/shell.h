#ifndef SHELL_H
#define SHELL_H
#include <QObject>

class Shell : public QObject
{
    Q_OBJECT
public:
    Shell();
public slots:
    void start();
signals:
    void newOutput(QString output);
};

#endif // SHELL_H
