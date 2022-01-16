#ifndef ORDERDETAILDIALOG_H
#define ORDERDETAILDIALOG_H

#include "../mydialog.h"

namespace Ui {
class OrderDetailDialog;
}

class OrderDetailDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit OrderDetailDialog(QWidget *parent = nullptr);
    ~OrderDetailDialog();

private:
    Ui::OrderDetailDialog *ui;

public:
    void loadInfor(string orderID = "", int data = 0);

private slots:
    void on_orderDetailClose_clicked();
};

#endif // ORDERDETAILDIALOG_H
