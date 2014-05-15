#ifndef TEXTINTERPRETER_H
#define TEXTINTERPRETER_H

#include <QMap>
#include <QMapIterator>
#include <QRegExp>
#include <QStringList>
#include <QList>
class RegexHelper : public QObject
{
    Q_OBJECT
public:
    RegexHelper();
    void addRegex(QString exp, QString id);
    static QStringList getFirstGroup(QString text, QString regex);
    static QString getFirst(QString text, QString regex);

signals:
    void signalNewMatch(QStringList values, QString id); //If we want to use it in a seperate thread

public slots:
    void slotNewInput(QString input); //If we want to use it in a seperate thread
private:
    QMap<QString, QRegExp> regs;
};

#endif // TEXTINTERPRETER_H
