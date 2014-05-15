#ifndef MYPAGE_H
#define MYPAGE_H
#include <QObject>
#include <QWebPage>

/**
 * @brief The MyPage class is derived from the QWebPage class, and it's purpose is simply to enable printing of
 * java messages.
 */
class MyPage : public QWebPage
{
    Q_OBJECT
public:
    MyPage(QObject * parent = 0 );
protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
};
#endif // MYPAGE_H
