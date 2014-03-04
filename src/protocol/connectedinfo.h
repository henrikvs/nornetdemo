#ifndef CONNECTEDINFO_H
#define CONNECTEDINFO_H

class ConnectedInfo
{
public:
    ConnectedInfo();
    bool isNode();
    void setIsNode();
    void setIsFrontEnd();
private:
    bool node;

};

#endif // CONNECTEDINFO_H
