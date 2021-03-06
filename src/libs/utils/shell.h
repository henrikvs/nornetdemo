#include "utilslibglobal.h"

#ifndef SHELL_H
#define SHELL_H
#include <QObject>

class UTILSLIB_EXPORT Shell : public QObject
{
    Q_OBJECT
public:
    Shell(QString terminate  = "exit");
public slots:
    void start();
signals:
    void newOutput(QString output);
    void shellClosing();

private:
    QString terminate;
};

#endif // SHELL_H
