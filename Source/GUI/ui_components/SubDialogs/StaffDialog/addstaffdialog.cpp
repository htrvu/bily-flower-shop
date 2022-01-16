#include "addstaffdialog.h"
#include "ui_addstaffdialog.h"

#include "../../../../Providers/AccountProvider/AccountProvider.h"
#include "../../../../Components/Account/Seller.h"
#include "../../../../Components/Account/Admin.h"

AddStaffDialog::AddStaffDialog(QWidget *parent) :
    MyDialog(parent),
    ui(new Ui::AddStaffDialog)
{
    ui->setupUi(this);
}

AddStaffDialog::~AddStaffDialog()
{
    delete ui;
}

void AddStaffDialog::loadInfor(string ID, int data) {
    ui->sellerList->clear();

    list<Account*> sellers = AccountProvider::getInstance()->getAccounts(SELLER);

    for (auto i = sellers.begin(); i != sellers.end(); i++) {
        Seller* seller = (Seller*)*i;

        QString display = QString::fromStdString(seller->username());
        display = display + " - " + QString::fromStdWString(seller->name());

        QListWidgetItem* item = new QListWidgetItem(display);
        item->setCheckState(Qt::Unchecked);

        // Attach the seller username to this item
        item->setWhatsThis(QString::fromStdString(seller->username()));

        ui->sellerList->addItem(item);
    }

    ui->sellerList->setStyleSheet("QListWidget {font-size: 16px; color: #333; padding: 4px; background-color: white; border: 1px solid #777;}QListWidget::item:selected { color: #333;    background-color: white;	border: none;} QScrollArea { border-radius: 15px; background-color: white;} QScrollBar:vertical { border: 1px solid #999999; border-radius: 5px; background: transparent; width: 12px; margin: 0px 0px 0px 0px;} QScrollBar::handle:vertical { background: #f27d85;border-radius: 5px;}QScrollBar::add-line:vertical { height: 0px; subcontrol-position: bottom;   subcontrol-origin: margin;} QScrollBar::sub-line:vertical {height: 0 px; subcontrol-position: top; subcontrol-origin: margin;}");
}

void AddStaffDialog::on_saveBtn_clicked()
{
    int nItem = ui->sellerList->count();

    int count = 0;
    for (int i = 0; i < nItem; i++) {
        QListWidgetItem* item = ui->sellerList->item(i);
        if (item->checkState() == Qt::Checked) {
            count++;
            Admin::getInstance()->addStaff(item->whatsThis().toStdString());
        }
    }

    if (count == 0) {
        emit this->saved(false);
    } else {
        emit this->saved(true);
        this->close();
    }
}

