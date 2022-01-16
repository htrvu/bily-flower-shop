#include "carteditdialog.h"
#include "ui_carteditdialog.h"
#include <QPixmap>

#include "../../../../Providers/ProductProvider/ProductProvider.h"

CartEditDialog::CartEditDialog(QWidget *parent) :
    MyDialog(parent),
    ui(new Ui::CartEditDialog)
{
    ui->setupUi(this);
}

CartEditDialog::~CartEditDialog()
{
    delete ui;
}

void CartEditDialog::loadInfor(string productID, int quantity)
{
    _objID = productID;
    Product* product = ProductProvider::getInstance()->getProduct(productID);

    ui->name->setText(QString::fromStdWString(product->name()));
    ui->price->setText(QString::fromStdWString(vndPrice(product->salePrice())));
    QString stock = QString::number(product->stock());
    ui->stock->setText(stock + " sản phẩm số sẵn");
    ui->quantity->setText("1");

    QPixmap pix(QString::fromStdString(product->imageDir()));
    int width = ui->image->width();
    int height = ui->image->height();
    ui->image->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->image->setAlignment(Qt::AlignCenter);
}

// Return: <quantity, ID>.
// When errors occur, quantity will be negative
//     -1: invalid input
//     -2: the quantity to be added is larger than stock quantiy
void CartEditDialog::on_saveBtn_clicked()
{
    bool check;
    uint quantity = ui->quantity->text().toUInt(&check);

    if (!check || quantity == 0) {
        emit this->saved(-1, _objID);
        return;
    }

    uint stock = ProductProvider::getInstance()->getProduct(_objID)->stock();

    if (quantity > stock) {
        emit this->saved(-2, _objID);
        return;
    }

    emit this->saved(quantity, _objID);
    this->close();
}

