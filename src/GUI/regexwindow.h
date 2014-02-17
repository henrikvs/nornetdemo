#ifndef REGEXWINDOW_H
#define REGEXWINDOW_H

#include <QDialog>
#include "textinterpreter.h"
namespace Ui {
class RegexWindow;
}

class RegexWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegexWindow(QWidget *parent = 0);
    ~RegexWindow();

private slots:
    void on_pushButton_clicked();
    void newResult(QStringList results, QString key);

    void on_pushButton_2_clicked();

private:
    Ui::RegexWindow *ui;
    TextInterpreter interp;
};

#endif // REGEXWINDOW_H
