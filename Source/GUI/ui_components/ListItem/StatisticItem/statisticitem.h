#ifndef STATISTICITEM_H
#define STATISTICITEM_H

#include <QFrame>
#include "../../../../Components/Product/Product.h"

class StatisticItem: public QFrame
{
public:
    StatisticItem();

    static QFrame* renderItem(Product* product, int height);
};

#endif // STATISTICITEM_H
