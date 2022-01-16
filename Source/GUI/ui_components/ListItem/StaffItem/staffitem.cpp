#include "staffitem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

StaffItem::StaffItem()
{

}

StaffItemReturn StaffItem::renderItem(Account *account, int height)
{
    Staff* staff = (Staff*)account;

    MyFrame* item = new MyFrame;
    item->setFixedHeight(height);
    item->setLineWidth(1);
    item->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: white; border-radius: 15px; } QLabel { font-size: 16px; border: none; }");
    item->setCursor(Qt::PointingHandCursor);

    // Attach the username to this item
    item->setWhatsThis(QString::fromStdString(account->username()));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setAlignment(Qt::AlignVCenter);
    layout->setContentsMargins(15, 0, 15, 0);

    // Staff's username
    QLabel* username = new QLabel();
    username->setText(QString::fromStdString(account->username()));
    layout->addWidget(username);

    // Staff's name
    QLabel* name = new QLabel();
    name->setText(QString::fromStdWString(account->name()));
    name->setStyleSheet("color: #333; font-weight: bold;");
    layout->addWidget(name);

    // Staff's salary
    QWidget* salary = new QWidget();
    QHBoxLayout* salaryLayout = new QHBoxLayout();

    salaryLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* temp = new QLabel;
    temp->setText("Lương tháng này:");
    temp->setStyleSheet("border: none; color: #777; font-weight: bold;");
    salaryLayout->addWidget(temp);

    QLabel* baseSalary = new QLabel();
    baseSalary->setText(QString::fromStdWString(vndPrice(staff->salary())));
    salaryLayout->addWidget(baseSalary);

    salaryLayout->setStretch(0, 0);
    salaryLayout->setStretch(1, 1);

    salary->setLayout(salaryLayout);
    layout->addWidget(salary);

    // Button
    QString btnStyle = "border: none; font-size: 14px; font-weight: bold; color: white; padding: 4px 6px; border-radius: 12px;";
    MyButton* btn = new MyButton;
    btn->setText("Sa thải");
    btn->setStyleSheet(btnStyle + "background-color: #f27d85;");
    btn->setCursor(Qt::PointingHandCursor);

    // Attach the username to this button
    btn->setWhatsThis(QString::fromStdString(account->username()));

    layout->addWidget(btn);

    layout->setStretch(0, 1);
    layout->setStretch(1, 3);
    layout->setStretch(2, 4);
    layout->setStretch(3, 1);

    item->setLayout(layout);

    StaffItemReturn result = {item, btn};
    return result;
}
