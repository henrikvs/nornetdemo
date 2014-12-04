#include "slicemanager.h"
#include "networkentity.h"
#include <QList>
#include <QFile>
#include <QDir>

QDataStream &operator<<(QDataStream &out, Sliver *sliver)
{
    out << sliver->hostName << sliver->IPv4 << sliver->IPv6 << sliver->port <<  sliver->getStatus();
    return out;
}

QDataStream &operator>>(QDataStream &in, Sliver *&sliver)
{
    sliver = new Sliver();
    int status;
    in >> sliver->hostName >> sliver->IPv4 >> sliver->IPv6 >>  sliver->port >> status;
    sliver->setStatus(Sliver::STATUS_OFFLINE);
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
    readSliversFromFile();

}

QList<Sliver*> SliceManager::getSlivers()
{
    return model.getNodes();
}

int SliceManager::sliverCount()
{
    return model.getNodes().size();
}

void SliceManager::createSliver(QString hostname, int port)
{
    Sliver *sliver = new Sliver();
    sliver->hostName = hostname;
    sliver->port = port;
    model.addNode(sliver);

}

void SliceManager::removeSliver(QString name)
{
    model.removeNode(name);
}

bool SliceManager::sliverExists(QString hostname)
{
    return model.contains(hostname);
}

void SliceManager::readSliversFromFile(QString file)
{
    QFile readfile(QDir::currentPath() + "/" + file);
    if (!readfile.open(QIODevice::ReadOnly)) {
        qDebug() << "failed to open file";
    }
    QDataStream in(&readfile);
    in.setVersion(QDataStream::Qt_5_0);
    QList<Sliver*> slivers;
    in >> slivers;
    model.setNodes(slivers);
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
    out << model.getNodes();
    writefile.close();
    qDebug() << "Wrote to file";
}

Sliver *SliceManager::getSliver(QString siteName)
{
    return model.getNode(siteName);
}

NodeModel* SliceManager::getModel()
{
    return &model;
}

void SliceManager::editSliver(QString nodeName, Sliver newNode)
{
    model.editNode(nodeName, newNode);
}

void SliceManager::importNodesFromFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QTextStream in(&file);

    QList<Sliver*> slivers;
    while(!in.atEnd()) {
        QString hostName = in.readLine().trimmed();
        Sliver *sliver = new Sliver();
        sliver->hostName = hostName;
        sliver->port = NetworkEntity::PORT;
        slivers << sliver;
    }

    if (!slivers.empty()) {
        model.setNodes(slivers);
    }

    file.close();
}
