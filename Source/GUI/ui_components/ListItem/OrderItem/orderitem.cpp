#include "orderitem.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../../../../Providers/ProductProvider/ProductProvider.h"
#include "../../../../Providers/AccountProvider/AccountProvider.h"

OrderItem::OrderItem()
{

}

OrderItemReturn OrderItem::renderItem(Order *order, int height, bool isBuyer)
{
    QString ID = QString::fromStdString(order->ID());

    MyFrame* itemWrapper = new MyFrame;
    itemWrapper->setFixedHeight(height);
    itemWrapper->setLineWidth(1);
    itemWrapper->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; }");
    itemWrapper->setCursor(Qt::PointingHandCursor);

    // Attach the order's ID to this item
    itemWrapper->setWhatsThis(ID);

    QHBoxLayout* wrapperLayout = new QHBoxLayout;
    wrapperLayout->setSpacing(8);
    wrapperLayout->setAlignment(Qt::AlignCenter);

    Product* product = ProductProvider::getInstance()->getProduct(order->productID());

    // -----------------------------------------------
    // Product's image
    QLabel* image = new QLabel;
    image->setFixedWidth(95);
    QString imageDir = QString::fromStdString(product->imageDir());
    QString style= "border-image: url(" + imageDir + ") 0 0 0 0 stretch stretch; border: none;";
    image->setStyleSheet(style);
    wrapperLayout->addWidget(image);

    // -----------------------------------------------
    // Order's information

    QWidget* infor = new QWidget;
    QVBoxLayout* inforLayout = new QVBoxLayout;
    inforLayout->setSpacing(10);

    // Line 1: Product's name and order's ID order's date
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

    // ID
    QLabel* temp1 = new QLabel;
    temp1->setText("Mã đơn hàng:");
    temp1->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line1Layout->addWidget(temp1);

    QLabel* displayID = new QLabel;
    displayID->setText(ID);
    displayID->setStyleSheet("border: none; color: #333;");
    line1Layout->addWidget(displayID);

    line1->setLayout(line1Layout);
    inforLayout->addWidget(line1);

    // Line 2: Buyer and product's quantity
    QWidget* line2 = new QWidget;
    QHBoxLayout* line2Layout = new QHBoxLayout;
    line2Layout->setAlignment(Qt::AlignLeft);
    line2Layout->setContentsMargins(0, 0, 0, 0);

    if (!isBuyer) {
        QLabel* temp = new QLabel;
        temp->setText("Người mua:");
        temp->setStyleSheet("border: none; color: #777; font-weight: bold;");
        line2Layout->addWidget(temp);

        QLabel* buyer = new QLabel;
        wstring name = AccountProvider::getInstance()->getAccount(order->buyerUsername())->name();
        buyer->setText(QString::fromStdWString(name));
        buyer->setStyleSheet("border: none; padding-right: 4px; border-right-width: 1px; border-right-color: #ccc; border-right-style: solid");
        line2Layout->addWidget(buyer);
    }

    QLabel* temp2 = new QLabel;
    temp2->setText("Số lượng:");
    temp2->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line2Layout->addWidget(temp2);

    QLabel* quantity = new QLabel;
    quantity->setText(QString::number(order->quantity()));
    quantity->setStyleSheet("border: none;");
    line2Layout->addWidget(quantity);

    line2->setLayout(line2Layout);
    inforLayout->addWidget(line2);

    // Line 3: Order's price and date
    QWidget* line3 = new QWidget;
    QHBoxLayout* line3Layout = new QHBoxLayout;
    line3Layout->setAlignment(Qt::AlignLeft);
    line3Layout->setContentsMargins(0, 0, 0, 0);

    // Price
    QLabel* temp3 = new QLabel;
    temp3->setText("Tổng tiền:");
    temp3->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line3Layout->addWidget(temp3);

    QLabel* price = new QLabel;
    price->setText(QString::fromStdWString(vndPrice(order->saleCost())));
    price->setStyleSheet("border: none; font-size: 18px; font-weight: bold; color: #f27d85;");
    line3Layout->addWidget(price);

    QWidget* spacer2 = new QWidget;
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    line3Layout->addWidget(spacer2);

    QLabel* date = new QLabel;
    QString dateString = QString::fromStdString(order->date()->toString());
    date->setText(dateString);
    date->setStyleSheet("border: none; color: #333;");
    line3Layout->addWidget(date);

    line3->setLayout(line3Layout);
    inforLayout->addWidget(line3);

    infor->setLayout(inforLayout);
    wrapperLayout->addWidget(infor);

    // -----------------------------------------------
    // Buttons
    MyButton* btn1 = nullptr;
    MyButton* btn2 = nullptr;
    bool accepted = order->status();
    QString btnStyle = "border: none; font-size: 14px; font-weight: bold; color: white; padding: 6px 10px; border-radius: 15px;";

    // Render order for Seller
    if (!isBuyer) {
        // Accept and reject order buttons
        if (!accepted) {
            QWidget* btns = new QWidget;
            QVBoxLayout* btnsLayout = new QVBoxLayout;
            btnsLayout->setAlignment(Qt::AlignRight);
            btnsLayout->setContentsMargins(0, 0, 0, 0);
            btnsLayout->setSpacing(8);

            btn1 = new MyButton;
            btn1->setText("Xác nhận");
            btn1->setStyleSheet(btnStyle + "background-color: #66b7af;");
            btn1->setCursor(Qt::PointingHandCursor);
            btn1->setWhatsThis(ID);
            btnsLayout->addWidget(btn1);

            btn2 = new MyButton;
            btn2->setText("Từ chối");
            btn2->setStyleSheet(btnStyle + "background-color: #f27d85;");
            btn2->setCursor(Qt::PointingHandCursor);
            btn2->setWhatsThis(ID);
            btnsLayout->addWidget(btn2);

            btns->setLayout(btnsLayout);
            wrapperLayout->addWidget(btns);
        }
    }
    // Render order for Buyer
    else {
        btn1 = new MyButton;
        btn1->setCursor(Qt::PointingHandCursor);
        btn1->setWhatsThis(ID);

        // Rate or remove order button
        QString text = "Hủy";
        QString backgroundColor = "#f27d85";

        if (accepted) {
            if (order->isRated()) {
                backgroundColor = "#66b7af";
                text = "Đã đánh giá";
                btn1->setCursor(Qt::ArrowCursor);
            } else {
                text = "Đánh giá";
            }
        }

        btnStyle += "background-color:" + backgroundColor + ";";
        QString margin = accepted ? "2" : "6";
        btnStyle += "margin-left: " + margin + "px; margin-right: " + margin + "px;";

        btn1->setText(text);
        btn1->setStyleSheet(btnStyle);

        wrapperLayout->addWidget(btn1);
    }

    // -----------------------------------------------
    // Set layout to item

    wrapperLayout->setStretch(0, 1);
    wrapperLayout->setStretch(1, 6);
    wrapperLayout->setStretch(2, 1);

    itemWrapper->setLayout(wrapperLayout);

    OrderItemReturn result = { itemWrapper, btn1, btn2 };

    return result;
}
