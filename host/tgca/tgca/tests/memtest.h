#ifndef MEMTEST_H
#define MEMTEST_H

#include "abstracttest.h"

class MemTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit MemTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
    void activatedMemType(QString type);
protected:
    virtual void startTest();
private:
    QComboBox *memType, *mode, *inversion, *oput;
    QLineEdit *startData, *deviceEdit;
    QSpinBox *incData, *iteration;
    QCheckBox *checkReadData;
};

class memObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    uint data, datainc, inverse, output;
    long inCycle;
    QString memType, mode;
    Device *dev;
    QString subDev;
    int checkReadData;

    void setWinmode(int mode);
};

#endif // MEMTEST_H
