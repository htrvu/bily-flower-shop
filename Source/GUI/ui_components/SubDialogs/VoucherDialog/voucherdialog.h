#ifndef VOUCHERDIALOG_H
#define VOUCHERDIALOG_H

#include <QListWidgetItem>

#include "../flexdialog.h"

namespace Ui {
class VoucherDialog;
}

class VoucherDialog : public FlexDialog
{
    Q_OBJECT

public:
    explicit VoucherDialog(QWidget *parent = nullptr);
    ~VoucherDialog();

private:
    Ui::VoucherDialog *ui;

public:
    void setType(bool isEditForm);
    void loadInfor(string voucherID = "", int data = 0);
    void clearInput();

public slots:
    void on_productList_itemChanged(QListWidgetItem *item);
    void on_saveBtn_clicked();

signals:
    void saved(bool isOk);
};

#endif // VOUCHERDIALOG_H
