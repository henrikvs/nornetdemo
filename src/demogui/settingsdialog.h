#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:

    void on_okButton_clicked();


    void on_cancelButton_clicked();

private:
    Ui::SettingsDialog *ui;
    bool sliverKeyEdited;
    bool gatekeeperKeyEdited;

};

#endif // SETTINGSDIALOG_H
