#include "slicemanager.h"
#include <QList>
#include <QFile>
#include <QDir>

QDataStream &operator<<(QDataStream &out, const Sliver *sliver)
{
    qDebug() << "outing";
    out << sliver->hostName << sliver->IPv4 << sliver->IPv6 << sliver->name << sliver->port << sliver->sliceName << sliver->status;
    return out;
}

QDataStream &operator>>(QDataStream &in, Sliver *&sliver)
{
    qDebug() << "Inning";
    sliver = new Sliver();
    in >> sliver->hostName >> sliver->IPv4 >> sliver->IPv6 >> sliver->name >> sliver->port >> sliver->sliceName >>sliver->status;
    return in;
}


SliceManager::SliceManager()
{/*
    Sliver *sliver = new Sliver;
    sliver->IPv4 = "testest";
    sliver->IPv6 = "::1";
    sliver->name = "default";
    sliver->sliceName = "srl_nndemo";
    sliver->domain = "tullinloekka.simula.nornet";
    sliver->port = 33555;
    slivers << sliver;
*/

}

QList<Sliver*> SliceManager::getSlivers()
{
    return slivers.values();
}

int SliceManager::sliverCount()
{
    return slivers.size();
}

void SliceManager::createSliver(QString hostname, QString sliceName, int port)
{
    Sliver *sliver = new Sliver();
    sliver->sliceName = sliceName;
    sliver->name = hostname;
    sliver->hostName = hostname;
    sliver->port = port;
    slivers[hostname] = sliver;

}

void SliceManager::removeSliver(QString name)
{
    slivers.remove(name);
}

bool SliceManager::sliverExists(QString hostname)
{
    return slivers.contains(hostname);
}

void SliceManager::readSliversFromFile(QString file)
{
    QFile readfile(QDir::currentPath() + "/" + file);
    if (!readfile.open(QIODevice::ReadOnly)) {
        qDebug() << "failed to open file";
    }
    QDataStream in(&readfile);
    in.setVersion(QDataStream::Qt_5_0);

    in >> slivers;
    readfile.close();
    qDebug() << "Read from file";
}

void SliceManager::writeSliversToFile(QString file)
{
    QFile writefile(QDir::currentPath() + "/" + file);
    if (!writefile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file";
        return;
    }
    QDataStream out(&writefile);
    out.setVersion(QDataStream::Qt_5_0);
    out << slivers;
    writefile.close();
    qDebug() << "Wrote to file";
}

Sliver *SliceManager::getSliver(QString siteName)
{
    return slivers[siteName];
}

void SliceManager::editSliver(QString siteName, QString sliceName, QString ipv6Addr, int port)
{
    if (!slivers.contains(siteName)) {
        qDebug() << "sliver not found";
        return;
    }
    Sliver *sliver = slivers[siteName];
    sliver->hostName = siteName;
    sliver->sliceName = sliceName;
    sliver->IPv6 = ipv6Addr;
    sliver->port = port;
}
