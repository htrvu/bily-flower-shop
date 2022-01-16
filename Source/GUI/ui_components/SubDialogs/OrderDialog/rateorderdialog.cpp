#include "rateorderdialog.h"
#include "ui_rateorderdialog.h"

RateOrderDialog::RateOrderDialog(QWidget *parent) :
    MyDialog(parent),
    ui(new Ui::RateOrderDialog)
{
    ui->setupUi(this);
}

RateOrderDialog::~RateOrderDialog()
{
    delete ui;
}

void RateOrderDialog::clearInput()
{
    QList<QAbstractButton*> buttons = ui->rateInput->buttons();
    ui->rateInput->setExclusive(false);
    for (QAbstractButton* btn: buttons) {
        btn->setChecked(false);
    }
    ui->rateInput->setExclusive(true);
}

void RateOrderDialog::loadInfor(string orderID, int data)
{
    this->clearInput();
    _objID = orderID;
    ui->ID->setText(QString::fromStdString(orderID));
}

void RateOrderDialog::on_saveBtn_clicked()
{
    int rating = (-1) * (ui->rateInput->checkedId() + 1);

    emit this->saved(_objID, rating);

    this->clearInput();
    this->close();
}
