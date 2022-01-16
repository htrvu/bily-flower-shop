#ifndef ORDERITEM_H
#define ORDERITEM_H

#include "../../MyFrame/myframe.h"
#include "../../MyButton/mybutton.h"
#include "../../../../Components/Order/Order.h"

#include <QPushButton>

struct OrderItemReturn {
    MyFrame* item;
    MyButton* btn1;     // Rate or remove for Buyer, Accept for Seller
    MyButton* btn2;     // Remove for Seller
};

class OrderItem: public MyFrame
{
    Q_OBJECT

public:
    OrderItem();

    static OrderItemReturn renderItem(Order* order, int height, bool isBuyer);
};

#endif // ORDERITEM_H
