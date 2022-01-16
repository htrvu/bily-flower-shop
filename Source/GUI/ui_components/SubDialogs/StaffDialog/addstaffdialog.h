#ifndef ADDSTAFFDIALOG_H
#define ADDSTAFFDIALOG_H

#include "../mydialog.h"

namespace Ui {
class AddStaffDialog;
}

class AddStaffDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit AddStaffDialog(QWidget *parent = nullptr);
    ~AddStaffDialog();

private:
    Ui::AddStaffDialog *ui;

public:
    void loadInfor(string pseudoID = "", int data = 0);

private slots:
    void on_saveBtn_clicked();

signals:
    void saved(bool isOk);
};

#endif // ADDSTAFFDIALOG_H
