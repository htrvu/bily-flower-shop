#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H
#include "../MyFrame/myframe.h"
#include "../../../Components/Product/Product.h"

class ProductCard: public MyFrame
{
    Q_OBJECT

public:
    ProductCard();

    static MyFrame* renderCard(int width, int height, Product* product);
};

#endif // PRODUCTCARD_H
