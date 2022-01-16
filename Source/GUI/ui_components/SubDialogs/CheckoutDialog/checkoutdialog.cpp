#include "checkoutdialog.h"
#include "ui_checkoutdialog.h"

#include "../../../../Providers/ProductProvider/ProductProvider.h"
#include "../../../../Providers/OrderProvider/OrderProvider.h"
#include "../../../../Providers/AccountProvider/AccountProvider.h"
#include "../../../../Components/Account/Buyer.h"

#include "../../ListItem/CartItem/cartitem.h"

CheckoutDialog::CheckoutDialog(QWidget *parent) :
    MyDialog(parent),
    ui(new Ui::CheckoutDialog)
{
    ui->setupUi(this);
}

CheckoutDialog::~CheckoutDialog()
{
    delete ui;
}

void CheckoutDialog::loadInfor(string productID, int quantity)
{
    Buyer* buyer = (Buyer*)AccountProvider::getInstance()->getAccount(_username);

    ui->address->setText(QString::fromStdWString(buyer->address()));
    ui->note->clear();

    _objID = "";
    _quantity = 0;

    ProductProvider* productProvider = ProductProvider::getInstance();

    // Render items
    int height = 100;
    list<pair<string, uint>> items;
    ullong totalCost;

    // If this is a checkout for buyer's cart
    if (quantity == 0) {
        items = buyer->cart()->items();
        totalCost = buyer->cart()->totalCost();
        ui->checkoutScroll->setWhatsThis("");
    }
    // Otherwise, this is a checkout a quick purchase from home page
    else {
        _objID = productID;
        _quantity = quantity;

        items.push_back({productID, quantity});

        Product* product = productProvider->getProduct(productID);
        totalCost = quantity * product->salePrice();
    }
    ui->totalCost->setText(QString::fromStdWString(vndPrice(totalCost)));

    // Render items
    QFrame* container = new QFrame;
    container->setStyleSheet("background-color: white; ");
    QVBoxLayout* list = new QVBoxLayout;

    list->setAlignment(Qt::AlignTop);
    list->setSpacing(8);

    for (auto i = items.begin(); i != items.end(); i++) {
        string ID = i->first;
        uint quantity = i->second;
        Product* product = productProvider->getProduct(ID);

        CartItemReturn result = CartItem::renderItem(product, quantity, height, true);

        list->addWidget(result.item);
    }

    container->setLayout(list);

    ui->checkoutScroll->setWidgetResizable(true);
    ui->checkoutScroll->setWidget(container);

    ui->checkoutScroll->setStyleSheet("QScrollArea {border-radius: 15px; background-color: white;} QScrollBar:vertical { border: 1px solid #999999; border-radius: 5px; background: transparent; width: 12px; margin: 0px 0px 0px 0px;}QScrollBar::handle:vertical {background: #f27d85;border-radius: 5px;} QScrollBar::add-line:vertical {height: 0px; subcontrol-position: bottom; subcontrol-origin: margin;}QScrollBar::sub-line:vertical { height: 0 px; subcontrol-position: top; subcontrol-origin: margin;}");
    ui->scrollAreaWidgetContents->setStyleSheet("QWidget#scrollAreaWidgetContents { background-color: white; border: 1px solid #ccc;	border-radius: 15px;}");
}

void CheckoutDialog::on_checkoutBtn_clicked()
{
    bool payWithWallet = ui->payOption->currentIndex() == 1;
    wstring address = ui->address->text().toStdWString();
    wstring note = ui->note->text().toStdWString();

    pair<int, string> result = OrderProvider::getInstance()->checkoutHandling(_username, _objID, _quantity, payWithWallet, address, note);

    emit this->checkoutFinished(result.first, result.second);

    if (result.first == 0) {
        this->close();
        return;
    }
}

