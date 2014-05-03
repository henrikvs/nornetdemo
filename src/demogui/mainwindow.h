#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionSSL_triggered();

    void on_actionRegex_test_triggered();

    void on_actionPing_test_triggered();

    void on_actionTransfer_test_triggered();

    void on_actionMap_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
