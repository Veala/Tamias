#ifndef SWITCH411BASETEST_H
#define SWITCH411BASETEST_H

#include "abstracttest.h"

class Switch411BaseTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit Switch411BaseTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *deviceEdit;

};

class switch411BaseObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    Switch411Device *dev;
};

#endif // SWITCH411BASETEST_H
