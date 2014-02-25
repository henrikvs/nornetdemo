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

