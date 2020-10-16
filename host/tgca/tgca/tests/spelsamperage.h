#ifndef SPELSAMPERAGE_H
#define SPELSAMPERAGE_H

#include "abstracttest.h"
#ifdef Q_OS_WIN32
#include <visa.h>
#include <windows.h>
#endif

class SpelsAmperage : public AbstractTest
{
    Q_OBJECT

public:
    explicit SpelsAmperage(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *deviceEditBC, *deviceEditRT, *instrument, *startDelay, *iterationDelay, *number;
    QComboBox *t_r, *outputBCstatus, *ampEnabled;
    QSpinBox *endExchangeTime, *iterPause, *iterNum;
    QCheckBox *impulseMode;
};

class DCThread : public QThread
{
    Q_OBJECT
#ifdef Q_OS_WIN32
    ViSession session, vi;
    ViChar buffer[5000];
#endif

public:
    void open() {
#ifdef Q_OS_WIN32
        output(QString("Открытие %1%2").arg(resrc).arg(" ... "));
        error = viOpenDefaultRM(&session);
        error = viOpen(session, resrc, VI_NULL, VI_NULL, &vi);
        if (error != VI_SUCCESS)
        {
            viClose(session);
            output(QString("Неисправность!\nУбедитесь что БП включен\nОшибка: %1").arg(error));
            err_handler(session, error);        //может не надо!!!!!!!!!!!!!!!
            Sleep(5000); // milli seconds
            throw QString("Open DC error");
        }

        error = viPrintf(vi, "*IDN?\n");
        error = viScanf(vi, "%t", buffer);
        output(QString("*IDN? -> %1").arg(buffer));
        if (error != VI_SUCCESS) {
            err_handler(vi, error);
            throw QString("Open DC error");
        }
#endif
    }

    void close() {
#ifdef Q_OS_WIN32
        viClear(vi);
        viClear(session);
        viClose(vi);
        viClose(session);
#endif
    }

private:
    void run() override {
//        error = viSetAttribute(vi, VI_ATTR_TMO_VALUE, VI_TMO_INFINITE);
//        if (error != VI_SUCCESS) {
//            err_handler(vi, error);
//            return;
//        }
//        error = viGetAttribute(vi, VI_ATTR_TMO_VALUE, buffer);
//        if (error != VI_SUCCESS) {
//            err_handler(vi, error);
//            return;
//        }
//        output(QString("VI_ATTR_TMO_VALUE: %1").arg(QString(buffer).toULongLong(nullptr,16)));

//        current.clear();
        QTime t;
        t.start();
        double sum=0;
        double sum_1_micro = 0;
        //Sleep(startDelay);
        thread()->msleep(startDelay);
        for (int i=0; i<n; i++) {
            if (isFinished) {
                //viClose(session);
                return;
            }
            //Sleep(iterationDelay);
            thread()->msleep(iterationDelay);
//            error = viPrintf(vi, "MEAS:CURR?\n");
//            if (error != VI_SUCCESS) {
//                err_handler(vi, error);
//                return;
//            }
//            error = viScanf(vi, "%s", buffer);
//            if (error != VI_SUCCESS) {
//                err_handler(vi, error);
//                return;
//            }
#ifdef Q_OS_WIN32
            error = viQueryf(vi, "MEAS:CURR?\n", "%s", buffer);
            if (error != VI_SUCCESS) {
                err_handler(vi, error);
                isFinished = true;
                return;
            }

//            current.append(QString(buffer).toDouble());
            sum+=QString(buffer).toDouble();
            sum_1_micro+=QString(buffer).toDouble();
//            output(QString("%1: %2 A;      Sum/N = %3 A").arg(i+1).arg(buffer).arg(sum_1_micro/((double)i+1)));
#endif
        }
        output(QString("------------------------------"));
        output(QString("Период следования сообщений, мс: %1").arg(t.elapsed()));
        output(QString("Период измерения токопотребления, ms: %1").arg(t.elapsed()/n));
        //output(QString("Среднее токопотребление за период следования сообщений, А: %1").arg(sum/(double)n));
        output(QString("Среднее токопотребление за период следования сообщений тестируемой микросхемы, А: %1").arg(sum_1_micro/(double)n));
        //outputStat(QString("amper"), sum/(double)n);
        outputStat(QString("amper"), sum_1_micro/(double)n);
        amper = QString::number(sum_1_micro/(double)n);
        //outputLog(QString::number(sum_1_micro/(double)n));
         //output(QString("Sum/N, A: %1").arg(QString::number((sum/(double)n)+1)));
        isFinished = true;
//        close();
//        viClose(vi);
//        viClose(session);
    }

#ifdef Q_OS_WIN32
    void err_handler(ViSession vi, ViStatus err){
        char err_msg[1024]={0};
        viStatusDesc (vi, err, err_msg);
        output(QString("Ошибка: %1").arg(err_msg));
        viClose(session);
        return;
    }
#endif

public:
#ifdef Q_OS_WIN32
    ViChar resrc[500];
    ViStatus error;
#endif
    int n, startDelay, iterationDelay;
    bool isFinished;
    QString amper;
//    QVector<double> current;

signals:
    void output(QString);
    void outputStat(QString, double);
    void outputLog(QString);
};

class spelsAmperageObjToThread : public absObjToThread
{
    Q_OBJECT

public:
    explicit spelsAmperageObjToThread(QObject* parent = 0);
    ~spelsAmperageObjToThread() {
        dcThread->quit();
        dcThread->wait();
        delete dcThread;
    }
    void configure(quint32 rt_bc = 0, quint32 type_man = 0, quint32 ena_codec = 1, quint32 addr = 0);
    void setCmdWord(quint32 t_r);
    void setDeviceToBC();
    void setDeviceToRT();

public slots:
    virtual void doWork();
public:
    Device *BC;
    Device *RT;
    Device *dev;
    QString subDevBC;
    QString subDevRT;
    DCThread *dcThread;
    int t_r;
    int endExchangeTime, iterPause, iterNum;
    QString outputBCstatus, ampEnabled;
    int impulseMode;
};


#endif // SPELSAMPERAGE_H
