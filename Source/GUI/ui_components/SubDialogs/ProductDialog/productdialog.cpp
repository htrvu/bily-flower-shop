#include "productdialog.h"
#include "ui_productdialog.h"

#include "../../../../Providers/ProductProvider/ProductProvider.h"
#include <QFileDialog>

ProductDialog::ProductDialog(QWidget *parent) :
    FlexDialog(parent),
    ui(new Ui::ProductDialog)
{
    ui->setupUi(this);
}

ProductDialog::~ProductDialog()
{
    delete ui;
}

void ProductDialog::setType(bool isEditForm)
{
    // _type will be TRUE if this manipulation relates to an editting
    FlexDialog::setType(isEditForm);

    this->clearInput();

    if (isEditForm) {
        ui->dialogType->setText("Chỉnh sửa sản phẩm");
    }
    else {
        ui->dialogType->setText("Thêm sản phẩm");
    }

    ui->description->setStyleSheet("QTextEdit {	padding: 4px; background-color: white; border: 1px solid #777; } QScrollBar:vertical {  border: 1px solid #999999;	border-radius: 5px;    background: transparent; width: 12px; margin: 0px 0px 0px 0px;} QScrollBar::handle:vertical { background: #f27d85;	border-radius: 5px;}QScrollBar::add-line:vertical { height: 0px; subcontrol-position: bottom; subcontrol-origin: argin;}QScrollBar::sub-line:vertical { height: 0 px; subcontrol-position: top; subcontrol-origin: margin;}");
}

void ProductDialog::loadInfor(string productID, int data)
{
    _objID = productID;

    Product* product = ProductProvider::getInstance()->getProduct(productID);

    QString name = QString::fromStdWString(product->name());
    uint categoryIndex = product->category();
    QString price = QString::number(product->originalPrice());
    QString stock = QString::number(product->stock());
    QString description = QString::fromStdWString(product->description());
    QString imageDir = QString::fromStdString(product->imageDir());

    ui->name->setText(name);
    ui->price->setText(price);
    ui->quantity->setText(stock);
    ui->description->setText(description);
    ui->category->setCurrentIndex(categoryIndex);

    ui->image->setWhatsThis(imageDir);
    QPixmap pix(imageDir);
    int width = ui->image->width();
    int height = ui->image->height();
    ui->image->setPixmap(pix.scaled(width - 32, height - 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->image->setAlignment(Qt::AlignCenter);
}

void ProductDialog::clearInput()
{
    ui->name->clear();
    ui->description->clear();
    ui->price->clear();
    ui->quantity->clear();
    ui->image->clear();
    ui->image->setWhatsThis("");
}

void ProductDialog::on_browseImageBtn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.bmp *.jpg)"));

    if (fileName.isEmpty()) {
        return;
    }

    QPixmap pix(fileName);
    int width = ui->image->width();
    int height = ui->image->height();
    ui->image->setPixmap(pix.scaled(width - 32, height - 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->image->setAlignment(Qt::AlignCenter);

    ui->image->setWhatsThis(fileName);
}

void ProductDialog::on_saveBtn_clicked()
{
    wstring name = ui->name->text().toStdWString();
    ullong price = ui->price->text().toULongLong();
    wstring description = ui->description->toPlainText().toStdWString();
    uint stock = ui->quantity->text().toUInt();
    string imageDir = ui->image->whatsThis().toStdString();
    uint categoryIndex = ui->category->currentIndex();

    if (name == L"" || _username == "" || description == L"" || imageDir == "") {
        emit this->added(false);
        return;
    }

    if (_type == true) {
        ProductProvider::getInstance()->editProduct(_objID, categoryIndex, name, description, price, stock, imageDir);
        emit this->edited(true);
    }
    else {
        // Add the product to "database"
        Product* product = new Product(categoryIndex, name, _username, description, price, stock, imageDir);
        ProductProvider::getInstance()->addProduct(product);
        emit this->added(true);
    }

    this->close();
}


