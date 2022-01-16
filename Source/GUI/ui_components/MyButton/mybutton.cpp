#include "mybutton.h"

MyButton::MyButton(QPushButton* parent) : QPushButton(parent)
{

}

void MyButton::mousePressEvent(QMouseEvent *event)
{
    emit this->buttonClicked(this->whatsThis());
}
