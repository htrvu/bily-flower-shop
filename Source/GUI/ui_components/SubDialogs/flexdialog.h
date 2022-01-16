#ifndef FLEXDIALOG_H
#define FLEXDIALOG_H

#include "mydialog.h"

class FlexDialog : public MyDialog
{
    Q_OBJECT

public:
    explicit FlexDialog(QWidget *parent = nullptr);
    ~FlexDialog();

protected:
    bool _type;

public:
    virtual void setType(bool type);
    virtual void loadInfor(string objID = "", int data = 0) = 0;
};

#endif // FLEXDIALOG_H
