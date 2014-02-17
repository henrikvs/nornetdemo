#include "textinterpreter.h"
#include <QDebug>
TextInterpreter::TextInterpreter()
{
}

void TextInterpreter::addRegex(QString exp, QString id)
{
    if (exp != "") {
        regs[id] = QRegExp(exp);
    }
}

void TextInterpreter::slotNewInput(QString input)
{
    QHashIterator<QString, QRegExp> i(regs);

    while (i.hasNext()) {
        i.next();
        QRegExp rx = i.value();
        int pos = 0;
        while ((pos = rx.indexIn(input, pos)) != -1) {
            emit signalNewMatch(rx.capturedTexts(), i.key());
            pos += rx.matchedLength();
            qDebug()<< pos;
        }
    }

}
