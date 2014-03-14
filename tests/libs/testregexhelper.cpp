#include "testregexhelper.h"

TestRegexHelper::TestRegexHelper(QObject *parent) :
    QObject(parent)
{
}

void TestRegexHelper::initTestCase()
{

}

void TestRegexHelper::testNumberRegex()
{
    RegexHelper helper;
    QString key = "numbers";
    helper.addRegex("(\\d+)", key);
    QList<QStringList> expected;
    QStringList list;
    list << "123" << key;
    expected << list;
    testRegex(helper, "123", expected);
}

void TestRegexHelper::testMultipleRegex()
{
    QList<QStringList> expected;
    RegexHelper helper;
    QString numbersKey = "numbers";
    helper.addRegex("(\\d)", numbersKey);
    QStringList numbersList;
    expected << (QStringList()  << "1" << numbersKey);
    expected << (QStringList()  << "2" << numbersKey);
    expected << (QStringList()  << "3" << numbersKey);

    QString stringKey = "string";
    helper.addRegex("(hallo)", stringKey);
    QStringList stringList;
    stringList << "hallo" << stringKey;
    expected << stringList;
    testRegex(helper, "123hallo", expected);
}

void TestRegexHelper::testMultipleResults()
{
    QList<QStringList> expected;
    RegexHelper helper;
    QString key = "etc";
    helper.addRegex("\\d+(hallo)(\\d{3})", key);
    expected << (QStringList()  << "hallo" << "444" << key);

    testRegex(helper, "123hallo444", expected);
}

void TestRegexHelper::testDigOutput()
{

}

void TestRegexHelper::testStatic1()
{
    QString expectedN = "63 25 4.800";
    QString expectedE = "10 24 5.760";
    QString text = "bymarka.uninett.ntnu.nornet. 86400 IN   LOC     63 25 4.800 N 10 24 5.760 E 50.00m 5m 20m 20m";
    QStringList res = RegexHelper::getFirstGroup(text, "LOC\\s*(\\d.*)\\s+N\\s*(\\d.*)\\s+E");

    qDebug() << "lenght: " << res.size();

    QCOMPARE(res[1], expectedN);
    QCOMPARE(res[2], expectedE);
}

void TestRegexHelper::testStatic2()
{
    QString expected = "srl-nndemo.kongsbakken.uninett.uit.nornet";
    QString text = " global options: +cmd 148.4.1.10.in-addr.arpa. 86400  IN      PTR     srl-nndemo.kongsbakken.uninett.uit.nornet.";


    QString res = RegexHelper::getFirst(text, "PTR\\s*(\\S*)\\.");
    QCOMPARE(res, expected);
}

void TestRegexHelper::testStaticNoRes()
{
    QString expected = "";
    QString text = "this is a text";
    QString res = RegexHelper::getFirst(text, "(something)");
    QCOMPARE(res, expected);
}

void TestRegexHelper::testStaticNoRes2()
{
    int expected = 0;
    QString text = "this is a text";
    QStringList resText = RegexHelper::getFirstGroup(text, "(any).*(thing).*(here).*");
    int res = resText.size();
    QCOMPARE(res, expected);
}



void TestRegexHelper::cleanupTestCase()
{


}

void TestRegexHelper::testRegex(RegexHelper &regexHelper, QString text, QList<QStringList> expectedResults)
{

    QSignalSpy spy(&regexHelper, SIGNAL(signalNewMatch(QStringList,QString)));

    regexHelper.slotNewInput(text);
    for (int i = 0; i < spy.size(); i++) {
        QList<QVariant> &arguments = spy[i];
        QStringList results = arguments.at(0).toStringList();
        results.removeFirst();
        QString k = arguments.at(1).toString();
        QCOMPARE(results << k, expectedResults[i]);
    }
}

