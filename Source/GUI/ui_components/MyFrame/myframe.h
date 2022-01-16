#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QFrame>

class MyFrame: public QFrame
{
    Q_OBJECT

public:
    MyFrame(QFrame* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(QString data, QFrame* obj = nullptr);
};

#endif // MYWIDGET_H
