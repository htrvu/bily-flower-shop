#ifndef RATEORDERDIALOG_H
#define RATEORDERDIALOG_H

#include "../mydialog.h"

namespace Ui {
class RateOrderDialog;
}

class RateOrderDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit RateOrderDialog(QWidget *parent = nullptr);
    ~RateOrderDialog();

private:
    Ui::RateOrderDialog *ui;

public:
    void clearInput();
    void loadInfor(string orderID = "", int data = 0);

private slots:
    void on_saveBtn_clicked();

signals:
    void saved(string orderID, int rating);
};

#endif // RATEORDERDIALOG_H
