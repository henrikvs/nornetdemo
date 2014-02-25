#ifndef TEXTINTERPRETER_H
#define TEXTINTERPRETER_H

#include <QMap>
#include <QMapIterator>
#include <QRegExp>
#include <QStringList>
class RegexHelper : public QObject
{
    Q_OBJECT
public:
    RegexHelper();
    void addRegex(QString exp, QString id);

signals:
    void signalNewMatch(QStringList values, QString id);

public slots:
    void slotNewInput(QString input);
private:
    QMap<QString, QRegExp> regs;
};

#endif // TEXTINTERPRETER_H
