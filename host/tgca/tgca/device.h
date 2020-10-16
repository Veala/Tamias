#ifndef DEVICE_H
#define DEVICE_H

#include "baseterasicdevice.h"
#include "lscgatewaythread.h"
#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <linux/futex.h>
#include <sys/time.h>
#endif
#ifdef Q_OS_WIN32
#include <windows.h>
#include <synchapi.h>
#endif
//#define TIMING

class Device : public BaseTerasicDevice
{
    Q_OBJECT
    int memSize = 16384;
    QSharedMemory sharedMemory;
    char *memoryFrom, *memoryTo;
    int isLSCGatewayEnabled;
#ifdef Q_OS_LINUX
    struct timespec {
        long tv_sec;
        long tv_nsec;
    } tv;
    int futex(int *uaddr, int op, int val, const struct timespec *timeout, int *uaddr2, int val3) {
        return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
    }
#endif
#ifdef Q_OS_WIN32
    HANDLE tgcaEvent;
    HANDLE rtl2Event;
#endif

public:
    explicit Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    virtual ~Device();
    void startLSCGateway();
    LSCGatewayThread lscGatewayThread;

public slots:
    void setLSCGatewayEnabled(int e) { qDebug() << "setLSCGatewayEnabled"; isLSCGatewayEnabled = e; }
};

#endif // DEVICE_H
