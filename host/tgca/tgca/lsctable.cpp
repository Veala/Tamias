#include "lsctable.h"

LSCTable::LSCTable() : devCount(0), nativeHeadKey("LSC.devices")
{
    sharedMemory.setNativeKey(nativeHeadKey);

    int memorySize = 600 + sizeof(devCount); //4 bytes for count; 600 - 10 devices: 60 bytes for 1 device;
    if (sharedMemory.create(memorySize) == false) {
        qDebug() << "LSCTable(QObject* parent): sharedMemory.create(memorySize) ERROR: " << sharedMemory.errorString();
        return;
    }
    memcpy((char*)sharedMemory.data(), (char*)&devCount, sizeof(devCount));
    key_t unix_key = 0;
#ifdef Q_OS_LINUX
    unix_key = ftok(QFile::encodeName(nativeHeadKey).constData(), qHash(QFile::encodeName(nativeHeadKey), 'Q'));
#endif
    //qDebug() << "table key:" << (uint)unix_key;
}

LSCTable::~LSCTable()
{
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~LSCTable() start";
#endif
    sharedMemory.detach();
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~LSCTable() end";
#endif
}

void LSCTable::addDevice(QString name)
{
    if (!sharedMemory.isAttached()) {
        qDebug() << "Невозможно добавить шлюз НСК устройства из-за ошибки: " << sharedMemory.errorString();
        return;
    }
    if (devCount == 10) {
        qDebug() << "Невозможно добавить НСК шлюз из-за превышения допустимого количества устройств(>10)";
        return;
    }
    int nameSize = name.size()+1;
    if (nameSize>16) {
        qDebug() << "Невозможно добавить НСК шлюз из-за превышения допустимой длины имени(>16 байт)";
        return;
    }

    int ref1 = sizeof(devCount) + devCount*60;
    memcpy((char*)sharedMemory.data()+ref1, (char*)&nameSize, sizeof(nameSize));
    int ref2 = ref1 + sizeof(nameSize);
    memcpy((char*)sharedMemory.data()+ref2, name.toUtf8().data(), nameSize);
    int ref3 = ref2 + 16;
    //qDebug() << "sizeof(key_t):" << sizeof(key_t);
    key_t unix_key=0;
    #ifdef Q_OS_LINUX
    unix_key = ftok(QFile::encodeName(name).constData(), qHash(QFile::encodeName(name), 'Q'));
    #endif
    memcpy((char*)sharedMemory.data()+ref3, (char*)(&unix_key), sizeof(key_t));
    devCount++;
    memcpy((char*)sharedMemory.data(), (char*)&devCount, sizeof(devCount));
    //qDebug() << "devCount" << devCount;
}

void LSCTable::delDevice(QString name)
{
    if (!sharedMemory.isAttached()) {
        qDebug() << "Невозможно удалить шлюз НСК устройства из-за ошибки: " << sharedMemory.errorString();
        return;
    }
    int nameSize = name.size()+1;
    int nameSize_shared;
    char* readName = new char[nameSize];
    int ref1 = sizeof(devCount);
    int ref2 = ref1+sizeof(nameSize);
    int i = 0;
    for (; i<devCount; i++) {
        memcpy((char*)&nameSize_shared, (char*)sharedMemory.data()+ref1+i*60, sizeof(nameSize));
        if (nameSize == nameSize_shared) {
            memcpy(readName, (char*)sharedMemory.data()+ref2+i*60, nameSize);
            //qDebug() << readName;
            //qDebug() << QString(readName);
            if (QString(readName) == name) break;
        }
    }
    delete readName;
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "Deleting device #" << i;
#endif
    if (i>=devCount) {
        qDebug() << "i>=devCount: НСК устройство не найдено";
        return;
    }
    int n = devCount*60 - (i+1)*60;
    memcpy(tempArray, (char*)sharedMemory.data()+ref1+60*(i+1), n);
    memcpy((char*)sharedMemory.data()+ref1+60*i, tempArray, n);
    devCount--;
    memcpy((char*)sharedMemory.data(), (char*)&devCount, sizeof(devCount));
}
