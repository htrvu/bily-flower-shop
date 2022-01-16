#include "myframe.h"

MyFrame::MyFrame(QFrame* parent) : QFrame(parent)
{

}

void MyFrame::mousePressEvent(QMouseEvent *event)
{
    emit this->clicked(this->whatsThis(), this);
}
