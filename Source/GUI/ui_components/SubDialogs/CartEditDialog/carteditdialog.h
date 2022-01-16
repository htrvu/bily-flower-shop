#ifndef CARTEDITDIALOG_H
#define CARTEDITDIALOG_H

#include "../mydialog.h"

namespace Ui {
class CartEditDialog;
}

class CartEditDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit CartEditDialog(QWidget *parent = nullptr);
    ~CartEditDialog();

private:
    Ui::CartEditDialog *ui;

public:
    void loadInfor(string productID = "", int quantity = 0);

private slots:
    void on_saveBtn_clicked();

signals:
    void saved(int quantity, string productID);
};

#endif // CARTEDITDIALOG_H
