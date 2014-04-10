#ifndef SSLFORM_H
#define SSLFORM_H

#include <QWidget>

namespace Ui {
class SSLForm;
}

class SSLForm : public QWidget
{
    Q_OBJECT

public:
    explicit SSLForm(QWidget *parent = 0);
    ~SSLForm();

private slots:
    void on_connectButton_clicked();

private:
    Ui::SSLForm *ui;
};

#endif // SSLFORM_H
