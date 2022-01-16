#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <string>
using namespace std;

class MyDialog : public QDialog
{
    Q_OBJECT

protected:
    string _username;
    string _objID;

public:
    explicit MyDialog(QWidget *parent = nullptr);
    ~MyDialog();

public:
    void setUsername(const string& username);
    void setObjID(const string& objID);
    virtual void loadInfor(string objID = "", int data = 0) = 0;
};

#endif // MYDIALOG_H
