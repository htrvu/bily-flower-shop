#include "statisticitem.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

StatisticItem::StatisticItem()
{

}

QFrame *StatisticItem::renderItem(Product *product, int height)
{
    QFrame* itemWrapper = new QFrame;
    itemWrapper->setFixedHeight(height);
    itemWrapper->setLineWidth(1);
    itemWrapper->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 14px; }");

    QHBoxLayout* wrapperLayout = new QHBoxLayout;
    wrapperLayout->setSpacing(0);
    wrapperLayout->setContentsMargins(5, 5, 0, 5);

    // -----------------------------------------------
    // Product's image

    QString imageDir = QString::fromStdString(product->imageDir());
    QLabel* image = new QLabel;
    image->setFixedWidth(69);
    QString style= "border-image: url(" + imageDir + ") 0 0 0 0 stretch stretch; border: none;";
    image->setStyleSheet(style);
    wrapperLayout->addWidget(image);

    // -----------------------------------------------
    // Product's information: name, salePrice and soldQuantity

    QWidget* infor = new QWidget;
    QVBoxLayout* inforLayout = new QVBoxLayout;
    inforLayout->setContentsMargins(8, 5, 0, 5);

    // Product's name
    QLabel* name = new QLabel;
    name->setText(QString::fromStdWString(product->name()));
    name->setStyleSheet("border: none; font-weight: bold; color: #333;");
    inforLayout->addWidget(name);

    // Product's salePrice
    QLabel* price = new QLabel;
    price->setText(QString::fromStdWString(vndPrice(product->salePrice())));
    price->setStyleSheet("border: none; font-weight: bold; color: #f27d85;");
    inforLayout->addWidget(price);

    // Product's soldQuantity
    QWidget* line = new QWidget;
    QHBoxLayout* lineLayout = new QHBoxLayout;
    lineLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* temp = new QLabel;
    temp->setText("Đã bán:");
    temp->setStyleSheet("border: none; color: #777; font-weight: bold;");
    lineLayout->addWidget(temp);

    QLabel* soldQuantity = new QLabel;
    soldQuantity->setText(QString::number(product->sold()));
    soldQuantity->setStyleSheet("border: none; font-weight: bold; color: #333;");
    lineLayout->addWidget(soldQuantity);

    lineLayout->setStretch(0, 0);
    lineLayout->setStretch(1, 1);
    line->setLayout(lineLayout);
    inforLayout->addWidget(line);

    infor->setLayout(inforLayout);
    wrapperLayout->addWidget(infor);

    wrapperLayout->setStretch(0, 1);
    wrapperLayout->setStretch(1, 3);

    itemWrapper->setLayout(wrapperLayout);
    return itemWrapper;
}
