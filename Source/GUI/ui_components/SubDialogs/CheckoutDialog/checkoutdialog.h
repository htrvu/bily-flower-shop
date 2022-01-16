#ifndef CHECKOUTDIALOG_H
#define CHECKOUTDIALOG_H

#include "../mydialog.h"

namespace Ui {
class CheckoutDialog;
}

class CheckoutDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit CheckoutDialog(QWidget *parent = nullptr);
    ~CheckoutDialog();

private:
    Ui::CheckoutDialog *ui;

private:
    uint _quantity;

public:
    void loadInfor(string productID = "", int quantity = 0);

private slots:
    void on_checkoutBtn_clicked();

signals:
    void checkoutFinished(int errorCode, string data);
};

#endif // CHECKOUTDIALOG_H
