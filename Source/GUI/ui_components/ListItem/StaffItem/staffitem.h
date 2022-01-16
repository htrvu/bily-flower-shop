#ifndef STAFFITEM_H
#define STAFFITEM_H

#include "../../MyFrame/myframe.h"
#include "../../MyButton/mybutton.h"
#include "../../../../Components/Account/Staff.h"

struct StaffItemReturn {
    MyFrame* item;
    MyButton* btn;
};

class StaffItem : public MyFrame
{
public:
    StaffItem();
    static StaffItemReturn renderItem(Account* account, int height);
};

#endif // STAFFITEM_H
