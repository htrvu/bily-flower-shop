#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include "../flexdialog.h"

namespace Ui {
class ProductDialog;
}

class ProductDialog : public FlexDialog
{
    Q_OBJECT

public:
    explicit ProductDialog(QWidget *parent = nullptr);
    ~ProductDialog();

private:
    Ui::ProductDialog *ui;

public:
    void setType(bool isEditForm);
    void clearInput();
    void loadInfor(string productID = "", int data = 0);

public slots:
    void on_browseImageBtn_clicked();
    void on_saveBtn_clicked();

signals:
    void edited(bool);
    void added(bool);
};

#endif // PRODUCTDIALOG_H
