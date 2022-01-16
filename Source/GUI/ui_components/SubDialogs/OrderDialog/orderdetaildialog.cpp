#include "orderdetaildialog.h"
#include "ui_orderdetaildialog.h"

#include "../../../../Providers/ProductProvider/ProductProvider.h"
#include "../../../../Providers/AccountProvider/AccountProvider.h"
#include "../../../../Providers/OrderProvider/OrderProvider.h"

OrderDetailDialog::OrderDetailDialog(QWidget *parent) :
    MyDialog(parent),
    ui(new Ui::OrderDetailDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Chi tiết đơn hàng");
}

OrderDetailDialog::~OrderDetailDialog()
{
    delete ui;
}

void OrderDetailDialog::on_orderDetailClose_clicked()
{
    this->close();
}

void OrderDetailDialog::loadInfor(string orderID, int data)
{
    Order* order = OrderProvider::getInstance()->getOrder(orderID);
    Product* product = ProductProvider::getInstance()->getProduct(order->productID());
    Account* seller = AccountProvider::getInstance()->getAccount(order->sellerUsername());
    Account* buyer = AccountProvider::getInstance()->getAccount(order->buyerUsername());

    ui->orderID->setText(QString::fromStdString(order->ID()));
    ui->date->setText(QString::fromStdString(order->date()->toString()));
    ui->sellerName->setText(QString::fromStdWString(seller->name()));
    ui->buyerName->setText(QString::fromStdWString(buyer->name()));
    ui->buyerPhoneNumber->setText(QString::fromStdString(buyer->phoneNumber()));

    if (order->status()) {
        ui->status->setText("Đã duyệt");
        ui->status->setStyleSheet("font-weight: bold; color: #62b7af;");
    } else {
        ui->status->setText("Chưa duyệt");
        ui->status->setStyleSheet("font-weight: bold; color: #f27d85;");
    }

    if (order->isRated()) {
        ui->rate->setText("Đã đánh giá");
        ui->rate->setStyleSheet("font-weight: bold; color: #62b7af;");
    } else {
        ui->rate->setText("Chưa đánh giá");
        ui->rate->setStyleSheet("font-weight: bold; color: #f27d85;");
    }

    ui->productName->setText(QString::fromStdWString(product->name()));
    ui->productQuantity->setText(QString::number(order->quantity()));
    ui->address->setText(QString::fromStdWString(order->address()));

    if (order->note().length() > 0) {
        ui->note->setText(QString::fromStdWString(order->note()));
    } else {
        ui->note->setText("Không có ghi chú");
    }

    if (order->payWithWallet()) {
        ui->payOption->setText("Thanh toán bằng Ví Bily");
    } else {
        ui->payOption->setText("Thanh toán khi nhận hàng");
    }

    ullong saleCost = order->saleCost();
    ullong originalCost = order->originalCost();
    if (originalCost != saleCost) {
        int percent = 100 - 100.0 * saleCost / originalCost;
        QString text = "Giảm " + QString::number(percent) + "%";
        ui->voucher->setText(text);
    } else {
        ui->voucher->setText("Không có voucher nào được áp dụng");
    }

    ui->totalCost->setText(QString::fromStdWString(vndPrice(order->saleCost())));
}

