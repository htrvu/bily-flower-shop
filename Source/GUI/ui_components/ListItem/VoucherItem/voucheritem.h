#ifndef VOUCHERITEM_H
#define VOUCHERITEM_H

#include "../../MyButton/mybutton.h"
#include "../../MyFrame/myframe.h"
#include "../../../../Components/Voucher/Voucher.h"

#include <QPushButton>

struct VoucherItemReturn {
    MyFrame* item;
    MyButton* editBtn;
    MyButton* removeBtn;
};

class VoucherItem: public MyFrame
{
    Q_OBJECT

public:
    VoucherItem();

public:
    static VoucherItemReturn renderNormalItem(Voucher* voucher, int height, bool isEvent = false);
    static VoucherItemReturn renderAdminItem(Voucher* voucher, int height);
};
#endif // VOUCHERITEM_H
