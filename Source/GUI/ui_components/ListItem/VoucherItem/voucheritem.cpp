#include "voucheritem.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

VoucherItem::VoucherItem()
{

}

VoucherItemReturn VoucherItem::renderNormalItem(Voucher *voucher, int height, bool isEvent)
{
    MyFrame* itemWrapper = new MyFrame;
    itemWrapper->setFixedHeight(height);
    itemWrapper->setLineWidth(1);
    itemWrapper->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; }");

    // Attach the order's ID to this item
    QString ID = QString::fromStdString(voucher->ID());
    itemWrapper->setWhatsThis(ID);

    QHBoxLayout* wrapperLayout = new QHBoxLayout;
    wrapperLayout->setSpacing(8);
    wrapperLayout->setAlignment(Qt::AlignCenter);

    // -----------------------------------------------
    // Voucher's information

    QWidget* infor = new QWidget;
    QVBoxLayout* inforLayout = new QVBoxLayout;
    inforLayout->setSpacing(10);

    // Voucher's name and ID
    QWidget* line1 = new QWidget;
    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->setContentsMargins(0, 0, 0, 0);

    QLabel* name = new QLabel;
    name->setText(QString::fromStdWString(voucher->name()));
    name->setStyleSheet("border: none; font-weight: bold; color: #333;");
    line1Layout->addWidget(name);

    QWidget* spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    line1Layout->addWidget(spacer);

    QLabel* temp1 = new QLabel;
    temp1->setText(isEvent ? "Mã sự kiện:" : "Mã Voucher:");
    temp1->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line1Layout->addWidget(temp1);

    QLabel* voucherID = new QLabel;
    voucherID->setText(QString::fromStdString(voucher->ID()));
    voucherID->setStyleSheet("border: none; color: #333;");
    line1Layout->addWidget(voucherID);

    line1->setLayout(line1Layout);
    inforLayout->addWidget(line1);

    // Voucher's sale percent
    QWidget* line2 = new QWidget;
    QHBoxLayout* line2Layout = new QHBoxLayout;
    line2Layout->setAlignment(Qt::AlignLeft);
    line2Layout->setContentsMargins(0, 0, 0, 0);

    QLabel* temp2 = new QLabel;
    temp2->setText("Phần trăm giảm:");
    temp2->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line2Layout->addWidget(temp2);

    QLabel* percent = new QLabel;
    percent->setText(QString::number(voucher->salePercent()));
    percent->setStyleSheet("border: none;");
    line2Layout->addWidget(percent);

    line2->setLayout(line2Layout);
    inforLayout->addWidget(line2);
    infor->setLayout(inforLayout);

    wrapperLayout->addWidget(infor);

    // -----------------------------------------------
    // Buttons: edit and remove

    QString style = "margin-left: 4px; border: none; font-size: 14px; font-weight: bold; color: white; padding: 5px; border-radius: 12px;";

    MyButton* editBtn = new MyButton;
    editBtn->setText("Sửa");
    editBtn->setStyleSheet(style + "background-color: #66b7af;");
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setWhatsThis(ID);
    wrapperLayout->addWidget(editBtn);

    MyButton* removeBtn = new MyButton;
    removeBtn->setText("Xóa");
    removeBtn->setStyleSheet(style + "background-color: #f27d85;");
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setWhatsThis(ID);
    wrapperLayout->addWidget(removeBtn);

    // -----------------------------------------------
    // Set layout to item

    wrapperLayout->setStretch(0, 7);
    wrapperLayout->setStretch(1, 1);
    wrapperLayout->setStretch(2, 1);

    itemWrapper->setLayout(wrapperLayout);

    VoucherItemReturn result = { itemWrapper, editBtn, removeBtn };

    return result;
}

VoucherItemReturn VoucherItem::renderAdminItem(Voucher *voucher, int height)
{
    MyFrame* itemWrapper = new MyFrame;
    itemWrapper->setFixedHeight(height);
    itemWrapper->setLineWidth(1);
    itemWrapper->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; }");
    itemWrapper->setCursor(Qt::PointingHandCursor);

    // Attach the order's ID to this item
    QString ID = QString::fromStdString(voucher->ID());
    itemWrapper->setWhatsThis(ID);

    QHBoxLayout* wrapperLayout = new QHBoxLayout;
    wrapperLayout->setSpacing(8);
    wrapperLayout->setAlignment(Qt::AlignCenter);

    // -----------------------------------------------
    // Voucher's information

    QWidget* infor = new QWidget;
    QVBoxLayout* inforLayout = new QVBoxLayout;
    inforLayout->setSpacing(10);

    // Voucher's name and ID
    QWidget* line1 = new QWidget;
    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->setContentsMargins(0, 0, 0, 0);

    QLabel* name = new QLabel;
    name->setText(QString::fromStdWString(voucher->name()));
    name->setStyleSheet("border: none; font-weight: bold; color: #333;");
    line1Layout->addWidget(name);

    QWidget* spacer1 = new QWidget;
    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    line1Layout->addWidget(spacer1);

    QLabel* temp1 = new QLabel;
    temp1->setText("Mã sự kiện:");
    temp1->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line1Layout->addWidget(temp1);

    QLabel* voucherID = new QLabel;
    voucherID->setText(QString::fromStdString(voucher->ID()));
    voucherID->setStyleSheet("border: none; color: #333;");
    line1Layout->addWidget(voucherID);

    line1->setLayout(line1Layout);
    inforLayout->addWidget(line1);

    // Voucher's sale percent and creater
    QWidget* line2 = new QWidget;
    QHBoxLayout* line2Layout = new QHBoxLayout;
    line2Layout->setAlignment(Qt::AlignLeft);
    line2Layout->setContentsMargins(0, 0, 0, 0);

    QLabel* temp2 = new QLabel;
    temp2->setText("Phần trăm giảm:");
    temp2->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line2Layout->addWidget(temp2);

    QLabel* percent = new QLabel;
    percent->setText(QString::number(voucher->salePercent()));
    percent->setStyleSheet("border: none;");
    line2Layout->addWidget(percent);

    QWidget* spacer2 = new QWidget;
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    line2Layout->addWidget(spacer2);

    QLabel* temp3 = new QLabel;
    temp3->setText("Nhân viên tạo:");
    temp3->setStyleSheet("border: none; color: #777; font-weight: bold;");
    line2Layout->addWidget(temp3);

    QLabel* creater = new QLabel;
    creater->setText(QString::fromStdString(voucher->sellerUsername()));
    creater->setStyleSheet("border: none;");
    line2Layout->addWidget(creater);

    line2->setLayout(line2Layout);
    inforLayout->addWidget(line2);
    infor->setLayout(inforLayout);

    wrapperLayout->addWidget(infor);

    // -----------------------------------------------
    // Buttons: Remove

    QString style = "margin-left: 4px; border: none; font-size: 14px; font-weight: bold; color: white; padding: 5px; border-radius: 12px;";

    MyButton* removeBtn = new MyButton;
    removeBtn->setText("Xóa");
    removeBtn->setStyleSheet(style + "background-color: #f27d85;");
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setWhatsThis(ID);
    wrapperLayout->addWidget(removeBtn);

    // -----------------------------------------------
    // Set layout to item

    wrapperLayout->setStretch(0, 7);
    wrapperLayout->setStretch(1, 1);
    wrapperLayout->setStretch(2, 1);

    itemWrapper->setLayout(wrapperLayout);

    VoucherItemReturn result = { itemWrapper, nullptr, removeBtn };

    return result;
}
