#include "selleritem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

SellerItem::SellerItem()
{

}

SellerItemReturn SellerItem::renderItem(Account *account, int height)
{
    Seller* seller = (Seller*)account;

    QFrame* item = new QFrame;
    item->setFixedHeight(height);
    item->setLineWidth(1);
    item->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; border: none; }");

    // Attach the username to this item
    item->setWhatsThis(QString::fromStdString(account->username()));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setAlignment(Qt::AlignVCenter);
    layout->setContentsMargins(15, 0, 15, 0);

    // Seller's username
    QLabel* username = new QLabel();
    username->setText(QString::fromStdString(account->username()));
    layout->addWidget(username);

    // Seller's name
    QLabel* name = new QLabel();
    name->setText(QString::fromStdWString(account->name()));
    name->setStyleSheet("color: #333; font-weight: bold;");
    layout->addWidget(name);

    // Seller's product quantity
    QWidget* quantity = new QWidget();
    QHBoxLayout* quantityLayout = new QHBoxLayout();

    quantityLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* temp = new QLabel;
    temp->setText("Số sản phẩm đã đăng:");
    temp->setStyleSheet("border: none; color: #777; font-weight: bold;");
    quantityLayout->addWidget(temp);

    QLabel* quantityNum = new QLabel();
    quantityNum->setText(QString::number(seller->products().size()));
    quantityLayout->addWidget(quantityNum);

    quantityLayout->setStretch(0, 0);
    quantityLayout->setStretch(1, 1);

    quantity->setLayout(quantityLayout);
    layout->addWidget(quantity);

    // Button
    QString btnStyle = "border: none; font-size: 14px; font-weight: bold; color: white; padding: 4px 8px; border-radius: 12px;";
    MyButton* btn = new MyButton;
    btn->setText("Hủy hợp đồng");
    btn->setStyleSheet(btnStyle + "background-color: #f27d85;");
    btn->setCursor(Qt::PointingHandCursor);

    // Attach the username to this button
    btn->setWhatsThis(QString::fromStdString(account->username()));

    layout->addWidget(btn);

    layout->setStretch(0, 1);
    layout->setStretch(1, 3);
    layout->setStretch(2, 4);
    layout->setStretch(3, 1);

    item->setLayout(layout);

    SellerItemReturn result = {item, btn};
    return result;
}
