#include "mypage.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

MyPage::MyPage(QObject *parent): QWebPage(parent){
}

void MyPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID){
   QString logEntry = message +" on line:"+ QString::number(lineNumber) +" Source:"+ sourceID;
   qDebug()<<logEntry;

   /** can be omitted */
   QFile file("C:\\Data\\debug.txt");
   if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
       return;
   QTextStream out(&file);
   out << logEntry  << endl;
}
