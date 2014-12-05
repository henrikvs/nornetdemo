#include "nornetconstants.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpressionMatch>
NorNetConstants::NorNetConstants()
{
    re.setPattern("NorNet_Provider(\\d+)='\"(.+?)\"\\s+\"(.+?)\"");
}

void NorNetConstants::loadISPsFromFile(QString filename)
{

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            QString indexString = match.captured(1);
            int index = indexString.toInt();
            QString longName = match.captured(2);
            QString shortName = match.captured(3);
            ISPInfo info;
            if (index >= 0 && index <= 255) {
                ispProviders[index].fullName = longName;
                ispProviders[index].shortName = shortName;
                ispProviders[index].index = index;
            }
        }
    }

    file.close();

}

void NorNetConstants::loadSitenamesFromFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList list = line.split(QRegularExpression("-"));
        QString shortName = list[0].trimmed();
        QString longName = list[1].trimmed();
        siteNames[shortName] = longName;
    }

    file.close();
}

QString NorNetConstants::getFullISPName(int index)
{
    if (index >= 0 && index <= 255) {
        return ispProviders[index].fullName;
    }
    return QString::number(index);
}

QString NorNetConstants::getShortISPName(int index)
{
    if (index >= 0 && index <= 255) {
        return ispProviders[index].shortName;
    }
    return QString::number(index);
}

QString NorNetConstants::getFullSiteName(QString abbr)
{
    if (siteNames.contains(abbr)) {
        return siteNames[abbr];
    } else {
        return abbr;
    }
}
