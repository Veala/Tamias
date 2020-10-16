#ifndef SPIPART_H
#define SPIPART_H

#include "abstracttest.h"
#include <QTableWidget>

class SpiPart : public AbstractTest
{
    Q_OBJECT
public:
    explicit SpiPart(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QTableWidget* table;
    QLineEdit *deviceEdit, *linesEdit;
    QCheckBox* cycleChBox;
};

class spiObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    QList<uint> addr, data;
    int cycle, lines;
    Device *dev;
    QString subDev;

};

#endif // SPIPART_H
