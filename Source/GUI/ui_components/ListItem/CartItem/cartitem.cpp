#include "cartitem.h"
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

CartItem::CartItem()
{

}

CartItemReturn CartItem::renderItem(Product* product, uint quantity, int height, bool isForCheckout)
{
    QFrame* item = new QFrame;
    item->setFixedHeight(height);
    item->setLineWidth(1);
    item->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; }");

    QHBoxLayout* layout = new QHBoxLayout;

    // -----------------------------------------------
    // Product's image
    QString imageDir = QString::fromStdString(product->imageDir());
    QLabel* image = new QLabel;
    image->setFixedWidth(80);
    QString style= "border-image: url(" + imageDir + ") 0 0 0 0 stretch stretch; border: none;";
    image->setStyleSheet(style);
    layout->addWidget(image);

    // -----------------------------------------------
    // Product's name and price

    QWidget* infor = new QWidget;
    QVBoxLayout* inforLayout = new QVBoxLayout;
    inforLayout->setAlignment(Qt::AlignVCenter);

    QLabel* name = new QLabel;
    name->setText(QString::fromStdWString(product->name()));
    name->setStyleSheet("border: none; font-size: 16px; font-weight: bold; color: #333;");
    inforLayout->addWidget(name);

    QLabel* price = new QLabel;
    price->setText(QString::fromStdWString(vndPrice(product->salePrice())));
    price->setStyleSheet("border: none; font-size: 18px; font-weight: bold; color: #f27d85;");
    inforLayout->addWidget(price);

    infor->setLayout(inforLayout);
    layout->addWidget(infor);

    // -----------------------------------------------
    // Product's quantity in cart

    QWidget* itemQuantity = new QWidget;
    itemQuantity->setStyleSheet("font-size: 16px; border: none;");
    QHBoxLayout* quantityLayout = new QHBoxLayout;

    QLabel* temp = new QLabel;
    temp->setText("Số lượng:");
    temp->setStyleSheet("color: #777; font-weight: bold;");
    QLabel* number = new QLabel;
    number->setText(QString::number(quantity));

    quantityLayout->addWidget(temp);
    quantityLayout->addWidget(number);
    itemQuantity->setLayout(quantityLayout);

    layout->addWidget(itemQuantity);


    // If this item is displayed in checkout page, we return the result now
    if (isForCheckout) {
        quantityLayout->setAlignment(Qt::AlignRight);
        layout->setStretch(0, 4);
        layout->setStretch(1, 12);
        layout->setStretch(2, 4);

        item->setLayout(layout);
        CartItemReturn result = {item, nullptr, nullptr};
        return result;
    }


    // -----------------------------------------------
    // Change quantity and remove item button
    QString ID = QString::fromStdString(product->ID());
    QString btnStyle = "border: none; font-size: 14px; font-weight: bold; color: white; padding: 5px 5px; border-radius: 13px;";

    MyButton* editBtn = new MyButton;
    editBtn->setText("Sửa");
    editBtn->setStyleSheet(btnStyle + "background-color: #66b7af;");
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setWhatsThis(ID);
    layout->addWidget(editBtn);

    MyButton* removeBtn = new MyButton;
    removeBtn->setText("Xóa");
    removeBtn->setStyleSheet(btnStyle + "background-color: #f27d85;");
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setWhatsThis(ID);
    layout->addWidget(removeBtn);

    // set layout to item
    layout->setStretch(0, 1);
    layout->setStretch(1, 4);
    layout->setStretch(2, 1);
    layout->setStretch(3, 1);
    layout->setStretch(4, 1);

    item->setLayout(layout);

    CartItemReturn result = {item, editBtn, removeBtn};
    return result;
}
