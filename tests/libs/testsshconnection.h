#ifndef TESTSSHCONNECTION_H
#define TESTSSHCONNECTION_H

#include "AutoTest.h"
#include "SSHConnection.h"

class TestSSHConnection : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test1();
    void cleanupTestCase();
private:
    SSHConnection connection;
};

DECLARE_TEST(TestSSHConnection)
#endif // TESTSSHCONNECTION_H
