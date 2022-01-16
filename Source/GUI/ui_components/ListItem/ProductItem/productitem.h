#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H
#include <QFrame>
#include "../../MyButton/mybutton.h"
#include "../../../../Components/Product/Product.h"

struct ProductItemReturn {
    QFrame* item;
    MyButton* editBtn;
    MyButton* removeBtn;
};

class ProductItem: public QFrame
{
    Q_OBJECT

public:
    ProductItem();

    static ProductItemReturn renderItem(Product* product, int height);
};

#endif // PRODUCTITEM_H
