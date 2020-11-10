#ifndef MFECTLDRIVER_H
#define MFECTLDRIVER_H

#include <QObject>
#include <QMutex>
#include <QProcess>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include "funclib.h"

class MfectlDriver : public QObject
{
    Q_OBJECT

public:
    explicit MfectlDriver(QObject *parent = 0) : QObject(parent), m_simpleReadCmd("%1"), m_simpleWriteCmd("%1=%2") { } //, timer(NULL), socket(NULL), mutex(NULL) { }
    ~MfectlDriver();
    QProcess::ProcessState getMfectlState() {
        return process->state();
    }
    //state по read write придумать или переменные

    struct AllData {
        ExchangeFormatSwitch411 format;
        ushort addr;
        ushort *data;
    } allData;
    QMutex* mutex;

private:
    QProcess* process;
    QTimer* timer;

    QString m_simpleReadCmd;
    QString m_simpleWriteCmd;

private slots:
    void init();
    void stop();
    void startedSlot();
    void finishedSlot(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStanOutSlot();
    void readyReadStanErrSlot();
    void bytesWrittenSlot(qint64);
    void errorSlot(QProcess::ProcessError);
    void timerTOSlot();

public slots:
    void tryToStart(QString program, QString args);
    void tryToStop();
    void tryToExchange();
};


#endif // MFECTLDRIVER_H


