#include "mydialog.h"

MyDialog::MyDialog(QWidget *parent) : QDialog(parent)
{

}

MyDialog::~MyDialog()
{

}

void MyDialog::setUsername(const string &username)
{
    _username = username;
}

void MyDialog::setObjID(const string &objID)
{
    _objID = objID;
}
