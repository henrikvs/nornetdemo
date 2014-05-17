#ifndef SETTINGS_H
#define SETTINGS_H
#include "slicemanager.h"

class Settings {
public:
    static QString sliverKey;
    static QString sliceName;
    static QString gatekeeperKey;
    static QString gatekeeperEnabled;
    static QString programName;
    static QString company;
    static SliceManager sliceManager;
    static QString gatekeeperUsername;
    static QString gatekeeperHostname;
    static QString nodeprogRootUrl;

    static QString relayEnabled;
    static QString relayPort;
    static QString relayHostname;

};

#endif // SETTINGS_H
