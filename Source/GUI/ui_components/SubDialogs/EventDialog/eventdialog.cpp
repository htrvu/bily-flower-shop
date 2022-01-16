#include "eventdialog.h"
#include "ui_eventdialog.h"

#include "../../../../Providers/VoucherProvider/VoucherProvider.h"

EventDialog::EventDialog(QWidget *parent) :
    FlexDialog(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::clearInput()
{
    ui->name->clear();
    ui->percent->clear();
}

void EventDialog::setType(bool isEditForm)
{
    // _type will be TRUE if this manipulation relates to an editting
    FlexDialog::setType(isEditForm);
    this->clearInput();

    if (isEditForm) {
        ui->title->setText("Chỉnh sửa sự kiện");
    }
    else {
        ui->title->setText("Tạo sự kiện");
    }
}


// Load information for adding or editing voucher
// If adding, voucherID is an empty string
void EventDialog::loadInfor(string voucherID, int data)
{
    _objID = voucherID;
    if (_type == true) {
        Voucher* voucher = VoucherProvider::getInstance()->getVoucher(voucherID);

        ui->name->setText(QString::fromStdWString(voucher->name()));
        ui->percent->setText(QString::number(voucher->salePercent()));
    }
}

void EventDialog::on_saveBtn_clicked()
{
    // Signal's code:
    // 1: updated ok
    // 0: added ok
    // -1: invalid input
    int returnCode = 0;

    wstring name = ui->name->text().toStdWString();

    bool check;
    double percent = ui->percent->text().toDouble(&check);

    if (!check || name.length() == 0 || percent < 0 || percent > 100) {
        emit this->saved(-1);
        return;
    }

    if (_type == true) {
        VoucherProvider::getInstance()->editVoucher(_objID, name, percent);
        returnCode = 1;
    } else {
        Voucher* voucher = new Voucher("", name, percent, _username);
        VoucherProvider::getInstance()->addVoucher(voucher);
        _objID = voucher->ID();
        returnCode = 0;
    }

    emit this->saved(returnCode);
    this->close();
}

