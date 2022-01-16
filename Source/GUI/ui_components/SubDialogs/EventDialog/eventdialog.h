#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include "../flexdialog.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public FlexDialog
{
    Q_OBJECT

public:
    explicit EventDialog(QWidget *parent = nullptr);
    ~EventDialog();

private:
    Ui::EventDialog *ui;

public:
    void clearInput();
    void setType(bool isEditForm);
    void loadInfor(string voucherID = "", int data = 0);

public slots:
    void on_saveBtn_clicked();

signals:
    void saved(int returnCode);
};

#endif // EVENTDIALOG_H
