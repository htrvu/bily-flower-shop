#ifndef CARTITEM_H
#define CARTITEM_H
#include <QFrame>
#include "../../MyButton/mybutton.h"
#include "../../../../Components/Product/Product.h"

struct CartItemReturn {
    QFrame* item;
    MyButton* editBtn;
    MyButton* removeBtn;
};

class CartItem: public QFrame
{
    Q_OBJECT

public:
    CartItem();

    static CartItemReturn renderItem(Product* product, uint quantity, int height, bool isForCheckout = false);
};

#endif // CARTITEM_H
