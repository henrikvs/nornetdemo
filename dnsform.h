#ifndef DNSFORM_H
#define DNSFORM_H

#include <QDialog>

namespace Ui {
class DNSForm;
}

class DNSForm : public QDialog
{
    Q_OBJECT

public:
    explicit DNSForm(QWidget *parent = 0);
    ~DNSForm();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DNSForm *ui;
};

#endif // DNSFORM_H
