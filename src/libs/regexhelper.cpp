#include "regexhelper.h"
#include <QDebug>
RegexHelper::RegexHelper()
{
}

void RegexHelper::addRegex(QString exp, QString id)
{
    if (exp != "") {
        regs[id] = QRegExp(exp);
    }
}

QStringList RegexHelper::getFirstGroup(QString text, QString regex)
{
    QRegExp reg(regex);
    int pos = reg.indexIn(text);
    if (pos == -1) {
        return QStringList();
    } else {
        return reg.capturedTexts();
    }
}

QString RegexHelper::getFirst(QString text, QString regex)
{
    QRegExp reg(regex);
    int pos = reg.indexIn(text);
    if (pos > -1) {
        return reg.cap(1);
    } else {
        return QString();
    }
}

void RegexHelper::slotNewInput(QString input)
{
    QMapIterator<QString, QRegExp> i(regs);
    while (i.hasNext()) {
        i.next();
        QRegExp rx = i.value();
        int pos = 0;
        while ((pos = rx.indexIn(input, pos)) != -1) {
            emit signalNewMatch(rx.capturedTexts(), i.key());
            pos += rx.matchedLength();
        }
    }

}
