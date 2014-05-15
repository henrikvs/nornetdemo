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

    void on_sliverKeyBrowseButton_clicked();

    void on_gatekeeperKeyBrowseButton_clicked();

    void on_okButton_clicked();

    void on_sliverKeyEdit_textChanged(const QString &arg1);

    void on_gatekeeperKeyEdit_textChanged(const QString &arg1);

    void on_cancelButton_clicked();

    void on_addHostButton_clicked();

    void on_deleteHostButton_clicked();

private:
    Ui::SettingsDialog *ui;
    bool sliverKeyEdited;
    bool gatekeeperKeyEdited;

};

#endif // SETTINGSDIALOG_H
