#ifndef SHELLCOMMAND_H
#define SHELLCOMMAND_H
#include <QObject>
#include <QString>
class ShellCommand : public QObject
{
    Q_OBJECT
public:
    ShellCommand();
public slots:
    void newStdIn(QString s);
};

#endif // SHELLCOMMAND_H
