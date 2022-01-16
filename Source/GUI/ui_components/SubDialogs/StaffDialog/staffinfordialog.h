#ifndef STAFFINFORDIALOG_H
#define STAFFINFORDIALOG_H

#include "../mydialog.h"

namespace Ui {
class StaffInforDialog;
}

class StaffInforDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit StaffInforDialog(QWidget *parent = nullptr);
    ~StaffInforDialog();

private:
    Ui::StaffInforDialog *ui;

public:
    void loadInfor(string username = "", int data = 0);

private slots:
    void on_orderDetailClose_clicked();
};

#endif // STAFFINFORDIALOG_H
