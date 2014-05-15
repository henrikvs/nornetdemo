#ifndef SELECTTESTDIALOG_H
#define SELECTTESTDIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class SelectTestDialog;
}

class SelectTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTestDialog(bool ipv4, bool ipv6, QWidget *parent = 0);
    static const int CONNECTION_IPV4 = 1;
    static const int CONNECTION_IPV6 = 2;
    static const int TEST_TCP = 1;
    static const int TEST_PING = 2;
    int getConnectionType();
    int getTestType();
    int getDurationSeconds();
    bool isAccepted();

    ~SelectTestDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SelectTestDialog *ui;
    QButtonGroup ipGroup;
    int testType;
    int connectionType;
    bool accepted = false;
};

#endif // SELECTTESTDIALOG_H
