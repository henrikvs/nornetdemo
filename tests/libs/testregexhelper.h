#ifndef TESTREGEXHELPER_H
#define TESTREGEXHELPER_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QHash>
#include <QSignalSpy>
#include <QList>
#include <QPair>
#include "regexhelper.h"
#include "AutoTest.h"

typedef QPair<QString,QString> QStringPair;

class TestRegexHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestRegexHelper(QObject *parent = 0);

signals:

private slots:
    void initTestCase();
    void testNumberRegex();
    void testMultipleRegex();
    void testMultipleResults();
    void testDigOutput();
    void testStatic1();
    void testStatic2();
    void testStaticNoRes();
    void testStaticNoRes2();
    void cleanupTestCase();
private:
    void testRegex(RegexHelper &regexHelper, QString text, QList<QStringList> expectedResults);
};

DECLARE_TEST(TestRegexHelper)
#endif // TESTREGEXHELPER_H
