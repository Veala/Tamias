#ifndef PAUSETEST_H
#define PAUSETEST_H

#include "abstracttest.h"

class PauseTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit PauseTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *pause;
};

class pauseObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    QString pause;
};

#endif // PAUSETEST_H
