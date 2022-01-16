#ifndef SELLERITEM_H
#define SELLERITEM_H

#include <QFrame>
#include "../../MyButton/mybutton.h"
#include "../../../../Components/Account/Seller.h"

struct SellerItemReturn {
    QFrame* item;
    MyButton* btn;
};

class SellerItem : public QFrame
{
public:
    SellerItem();
    static SellerItemReturn renderItem(Account* account, int height);
};

#endif // SELLERITEM_H
