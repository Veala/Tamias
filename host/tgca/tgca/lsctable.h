#ifndef LSCTable_H
#define LSCTable_H

#include <QSharedMemory>
#include <QDebug>
#include "funclib.h"

#ifdef Q_OS_LINUX
#include <QFile>
#include <sys/types.h>
#include <sys/ipc.h>
#else
#ifdef Q_OS_WIN32
typedef int key_t;
#endif
#endif

class LSCTable
{
    QSharedMemory sharedMemory;
    QString nativeHeadKey;
    int devCount;
    char tempArray[600];
public:
    LSCTable();
    ~LSCTable();
    void addDevice(QString name);
    void delDevice(QString name);
};

#endif // LSCTable_H
