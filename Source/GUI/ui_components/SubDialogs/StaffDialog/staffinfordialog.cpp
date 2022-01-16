#include "staffinfordialog.h"
#include "ui_staffinfordialog.h"

#include "../../../../Providers/AccountProvider/AccountProvider.h"
#include "../../../../Components/Account/Staff.h"

StaffInforDialog::StaffInforDialog(QWidget *parent) :
    MyDialog(parent),
    ui(new Ui::StaffInforDialog)
{
    ui->setupUi(this);
}

StaffInforDialog::~StaffInforDialog()
{
    delete ui;
}

void StaffInforDialog::loadInfor(string username, int data)
{
    Staff* staff = (Staff*)AccountProvider::getInstance()->getAccount(username);

    ui->name->setText(QString::fromStdWString(staff->name()));
    ui->username->setText("(" + QString::fromStdString(staff->username()) + ")");
    ui->address->setText(QString::fromStdWString(staff->address()));
    ui->phoneNumber->setText(QString::fromStdString(staff->phoneNumber()));
    ui->soldQuantity->setText(QString::number(staff->soldQuantity()));
    ui->baseSalary->setText(QString::fromStdWString(vndPrice(staff->baseSalary())));
    ui->bonus->setText(QString::fromStdWString(vndPrice(staff->bonus())));
    ui->totalSalary->setText(QString::fromStdWString(vndPrice(staff->salary())));
}

void StaffInforDialog::on_orderDetailClose_clicked()
{
    this->close();
}

