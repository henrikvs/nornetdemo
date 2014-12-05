#ifndef NORNETCONSTANTS_H
#define NORNETCONSTANTS_H

#include <QString>
#include <QRegularExpression>
#include <QHash>

struct ISPInfo {
    int index;
    QString fullName;
    QString shortName;
};

class NorNetConstants
{
public:
    NorNetConstants();

    void loadISPsFromFile(QString filename);
    void loadSitenamesFromFile(QString filename);
    QString getFullISPName(int index);
    QString getShortISPName(int index);
    QString getFullSiteName(QString abbr);

private:
    ISPInfo ispProviders[256];
    QHash<QString, QString> siteNames;

    QRegularExpression re;
};

#endif // NORNETCONSTANTS_H
