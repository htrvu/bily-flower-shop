#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <string>

class MyButton: public QPushButton
{
    Q_OBJECT

public:
    MyButton(QPushButton* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void buttonClicked(QString data);
};

#endif // MYBUTTON_H
