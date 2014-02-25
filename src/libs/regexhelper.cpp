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
