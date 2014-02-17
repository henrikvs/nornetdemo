#ifndef TEXTINTERPRETER_H
#define TEXTINTERPRETER_H

#include <QHash>
#include <QRegExp>
#include <QStringList>
class TextInterpreter : public QObject
{
    Q_OBJECT
public:
    TextInterpreter();
    void addRegex(QString exp, QString id);

signals:
    void signalNewMatch(QStringList values, QString id);

public slots:
    void slotNewInput(QString input);
private:
    QHash<QString, QRegExp> regs;
};

#endif // TEXTINTERPRETER_H
