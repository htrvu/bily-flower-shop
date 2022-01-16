#include "voucherdialog.h"
#include "ui_voucherdialog.h"

#include "../../../../Providers/ProductProvider/ProductProvider.h"
#include "../../../../Providers/VoucherProvider/VoucherProvider.h"
#include "../../../../Providers/AccountProvider/AccountProvider.h"

#include "../../../../Components/Voucher/Voucher.h"
#include "../../../../Components/Account/Seller.h"


VoucherDialog::VoucherDialog(QWidget *parent) :
    FlexDialog(parent),
    ui(new Ui::VoucherDialog)
{
    ui->setupUi(this);
}


VoucherDialog::~VoucherDialog()
{
    delete ui;
}

void VoucherDialog::setType(bool isEditForm)
{
    // _type will be TRUE if this manipulation relates to an editting
    FlexDialog::setType(isEditForm);

    this->clearInput();

    if (isEditForm) {
        ui->title->setText("Chỉnh sửa Voucher");
    }
    else {
        ui->title->setText("Tạo Voucher");
    }

    ui->productList->setStyleSheet("QListWidget {font-size: 16px; color: #333; padding: 4px; background-color: white; border: 1px solid #777;}QListWidget::item:selected { color: #333;    background-color: white;	border: none;} QScrollArea { border-radius: 15px; background-color: white;} QScrollBar:vertical { border: 1px solid #999999; border-radius: 5px; background: transparent; width: 12px; margin: 0px 0px 0px 0px;} QScrollBar::handle:vertical { background: #f27d85;border-radius: 5px;}QScrollBar::add-line:vertical { height: 0px; subcontrol-position: bottom;   subcontrol-origin: margin;} QScrollBar::sub-line:vertical {height: 0 px; subcontrol-position: top; subcontrol-origin: margin;}");
}

// Load information for adding or editing voucher
// If adding, voucherID is an empty string
void VoucherDialog::loadInfor(string voucherID, int data)
{
    Seller* seller = (Seller*)(AccountProvider::getInstance()->getAccount(_username));

    list<string> allProductIDs = seller->products();

    ProductProvider* productProvider = ProductProvider::getInstance();
    unordered_map<string, int> appliedProductStores;

    if (_type == true) {
        _objID = voucherID;

        Voucher* voucher = VoucherProvider::getInstance()->getVoucher(voucherID);

        ui->name->setText(QString::fromStdWString(voucher->name()));
        ui->percent->setText(QString::number(voucher->salePercent()));

        list<string> appliedProductIDs = voucher->productIDs();
        for (auto i = appliedProductIDs.begin(); i != appliedProductIDs.end(); i++) {
            appliedProductStores[*i] = 1;
        }
    }

    QListWidgetItem* chooseAllItem  = new QListWidgetItem("Tất cả");
    chooseAllItem->setCheckState(Qt::Unchecked);
    chooseAllItem->setWhatsThis("all");
    ui->productList->addItem(chooseAllItem);

    int count = 0;
    for (auto i = allProductIDs.begin(); i != allProductIDs.end(); i++) {
        string ID = *i;

        Product* product = productProvider->getProduct(ID);

        QString display = QString::fromStdString(ID);
        display = display + " - " + QString::fromStdWString(product->name());

        QListWidgetItem* item = new QListWidgetItem(display);
        // Attach the ID to this item
        item->setWhatsThis(QString::fromStdString(ID));

        if (appliedProductStores.find(ID) != appliedProductStores.end()) {
            item->setCheckState(Qt::Checked);
            count++;
        } else {
            item->setCheckState(Qt::Unchecked);
        }

        ui->productList->addItem(item);
    }

    if (count == allProductIDs.size()) {
        chooseAllItem->setCheckState(Qt::Checked);
    }
}

void VoucherDialog::clearInput()
{
    ui->name->clear();
    ui->percent->clear();
    ui->productList->clear();
}

void VoucherDialog::on_productList_itemChanged(QListWidgetItem *item)
{
    QString data = item->whatsThis();
    if (data == "all") {
        bool isChooseAll = (item->checkState() == Qt::Checked);

        int nItem = ui->productList->count();
        for (int i = 1; i < nItem; i++) {
            QListWidgetItem* item = ui->productList->item(i);
            if (isChooseAll) {
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        }
        return;
    }

    // Uncheck the "check all" item when some item unchecked
    if (item->checkState() == Qt::Unchecked) {
        ui->productList->item(0)->setCheckState(Qt::Unchecked);
    }
}

void VoucherDialog::on_saveBtn_clicked()
{
    wstring name = ui->name->text().toStdWString();

    bool check;
    double percent = ui->percent->text().toDouble(&check);

    if (!check || name.length() == 0 || percent < 0 || percent > 100) {
        emit this->saved(false);
        return;
    }

    list<string> appliedProductIDs;

    // Apply to all products of seller
    if (ui->productList->item(0)->checkState() == Qt::Checked) {
        Seller* seller = (Seller*)AccountProvider::getInstance()->getAccount(_username);
        appliedProductIDs = seller->products();
    }
    else {
        int nItem = ui->productList->count();
        int count = 0;
        for (int i = 0; i < nItem; i++) {
            QListWidgetItem* item = ui->productList->item(i);
            if (item->checkState() == Qt::Checked) {
                count++;
                appliedProductIDs.push_back(item->whatsThis().toStdString());
            }
        }

        if (count == 0) {
            emit this->saved(false);
            return;
        }
    }


    if (_type == true) {
        VoucherProvider::getInstance()->editVoucher(_objID, name, percent, appliedProductIDs);
    } else {
        Voucher* voucher = new Voucher("", name, percent, _username, appliedProductIDs);
        VoucherProvider::getInstance()->addVoucher(voucher);
    }

    emit this->saved(true);
    this->close();
}



