#include "productitem.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

ProductItem::ProductItem()
{

}

// Return a struct including <item, editBtn, removeBtn>
ProductItemReturn ProductItem::renderItem(Product* product, int height)
{
    QFrame* itemWrapper = new QFrame;
    itemWrapper->setFixedHeight(height);
    itemWrapper->setLineWidth(1);
    itemWrapper->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; }");

    QHBoxLayout* wrapperLayout = new QHBoxLayout;
    wrapperLayout->setSpacing(8);

    // -----------------------------------------------

    // Product's image
    QString imageDir = QString::fromStdString(product->imageDir());
    QLabel* image = new QLabel;
    image->setFixedWidth(85);
    QString style= "border-image: url(" + imageDir + ") 0 0 0 0 stretch stretch; border: none;";
    image->setStyleSheet(style);
    wrapperLayout->addWidget(image);

    // -----------------------------------------------
    // Product's information layout

    QWidget* infor = new QWidget;
    QVBoxLayout* inforLayout = new QVBoxLayout;

    // Product's name and quantity
    QWidget* line1 = new QWidget;
    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->setContentsMargins(0, 0, 0, 0);

    QLabel* name = new QLabel;
    name->setText(QString::fromStdWString(product->name()));
    name->setStyleSheet("border: none; font-weight: bold; color: #333;");
    line1Layout->addWidget(name);

    QWidget* spacer1 = new QWidget;
    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    line1Layout->addWidget(spacer1);

    QLabel* temp1 = new QLabel;
    temp1->setText("Số lượng kho:");
    temp1->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line1Layout->addWidget(temp1);

    QLabel* quantity = new QLabel;
    quantity->setText(QString::number(product->stock()));
    quantity->setStyleSheet("border: none;");
    line1Layout->addWidget(quantity);

    line1->setLayout(line1Layout);
    inforLayout->addWidget(line1);

    // Product's Price and ID
    QWidget* line2 = new QWidget;
    QHBoxLayout* line2Layout = new QHBoxLayout;
    line2Layout->setContentsMargins(0, 0, 0, 0);

    QLabel* price = new QLabel;
    price->setText(QString::fromStdWString(vndPrice(product->salePrice())));
    price->setStyleSheet("QLabel { border: none; color: #f27d85; font-size: 18px; font-weight: bold; }");
    line2Layout->addWidget(price);

    QWidget* spacer2 = new QWidget;
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    line2Layout->addWidget(spacer2);

    QLabel* temp2 = new QLabel;
    temp2->setText("Mã sản phẩm:");
    temp2->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line2Layout->addWidget(temp2);

    QLabel* productID = new QLabel;
    productID->setText(QString::fromStdString(product->ID()));
    productID->setStyleSheet("border: none;");
    line2Layout->addWidget(productID);

    line2->setLayout(line2Layout);

    inforLayout->addWidget(line2);


    // Set information layout
    infor->setLayout(inforLayout);
    wrapperLayout->addWidget(infor);

    // -----------------------------------------------
    // Buttons: Edit and remove

    QString ID = QString::fromStdString(product->ID());

    QString btnStyle = "border: none; font-size: 14px; font-weight: bold; color: white; padding: 5px 13px; border-radius: 12px;";
    QWidget* btns = new QWidget;
    QVBoxLayout* btnsLayout = new QVBoxLayout;
    btnsLayout->setAlignment(Qt::AlignRight);
    btnsLayout->setContentsMargins(0, 0, 0, 0);

    MyButton* editBtn = new MyButton;
    editBtn->setText("Sửa");
    editBtn->setStyleSheet(btnStyle + "background-color: #66b7af;");
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setWhatsThis(ID);
    btnsLayout->addWidget(editBtn);

    MyButton* removeBtn = new MyButton;
    removeBtn->setText("Xóa");
    removeBtn->setStyleSheet(btnStyle + "background-color: #f27d85;");
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setWhatsThis(ID);
    btnsLayout->addWidget(removeBtn);

    btns->setLayout(btnsLayout);

    wrapperLayout->addWidget(btns);

    // -----------------------------------------------
    // Set layout to item
    wrapperLayout->setStretch(0, 1);
    wrapperLayout->setStretch(1, 7);
    wrapperLayout->setStretch(2, 1);

    itemWrapper->setLayout(wrapperLayout);

    return {itemWrapper, editBtn, removeBtn};
}
