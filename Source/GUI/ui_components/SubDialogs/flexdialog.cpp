#include "flexdialog.h"

FlexDialog::FlexDialog(QWidget *parent) : MyDialog(parent)
{

}

FlexDialog::~FlexDialog()
{

}

void FlexDialog::setType(bool type)
{
    _type = type;
}
