#include "productcard.h"
#include "../../Providers/AccountProvider/AccountProvider.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

ProductCard::ProductCard()
{

}

MyFrame *ProductCard::renderCard(int width, int height, Product* product)
{
    Account* account = AccountProvider::getInstance()->getAccount(product->sellerUsername());

    QString cardStyle = "border-radius: 10px; border: 1px solid #ccc; background-color: white;";
    QString infoStyle = "QWidget { border: none; margin: 0; border-radius: 0; background-color: #66B7AF; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px;}";
    QString nameStyle = "QLabel { color: #FFDDD2; margin: 0; padding: 0; padding-top: 6px; border: none; font-size: 16px; font-weight: bold; }";
    QString priceStyle = "QLabel { margin: 0; padding: 0; padding-bottom: 8px; border: none; margin-top: -8px; font-size: 14px; font-weight: bold; color: white; }";

    if (account->role() == "ST") {
        infoStyle += "QWidget {background-color: #006d77;}";
    }

    MyFrame* card = new MyFrame();
    card->setFixedHeight(height);
    card->setMinimumWidth(width);
    card->setStyleSheet(cardStyle);
    card->setCursor(Qt::PointingHandCursor);

    // Attach the product's ID to this card
    card->setWhatsThis(QString::fromStdString(product->ID()));

    QVBoxLayout* temp = new QVBoxLayout;
    temp->setContentsMargins(0, 0, 0, 0);

    // Product's image
    QLabel* image = new QLabel();
    QPixmap pix(QString::fromStdString(product->imageDir()));

    int w = card->width(), h = 200;
    image->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    image->setAlignment(Qt::AlignCenter);
    image->setStyleSheet("border: none; margin: 0; border-radius: 0; border-top-left-radius: 10px; border-top-right-radius: 10px;");

    QWidget* info = new QWidget();
    info->setStyleSheet(infoStyle);

    QVBoxLayout* infoLayout = new QVBoxLayout;
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setAlignment(Qt::AlignCenter);
    infoLayout->setSpacing(0);

    QLabel* name = new QLabel;
    // Product's name
    name->setText(QString::fromStdWString(product->name()));
    name->setStyleSheet(nameStyle);
    name->setAlignment(Qt::AlignCenter);

    QLabel* price = new QLabel;
    // Product's price
    price->setText(QString::fromStdWString(vndPrice(product->salePrice())));
    price->setStyleSheet(priceStyle);
    price->setAlignment(Qt::AlignCenter);

    infoLayout->addWidget(name);
    infoLayout->addWidget(price);
    info->setLayout(infoLayout);

    temp->addWidget(image);
    temp->addWidget(info);

    temp->setStretch(0, 1);
    temp->setSpacing(0);

    card->setLayout(temp);

    return card;
}
