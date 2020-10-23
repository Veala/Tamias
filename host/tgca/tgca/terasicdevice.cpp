#include "terasicdevice.h"

TerasicDevice::TerasicDevice(QWidget *parent, QString name, QTextBrowser *tB) :
    BaseTerasicDevice(parent, name, tB), isLSCGatewayEnabled(0)
{
    lscGatewayThread.setDevice(this);
    int nameSize = name.size()+1;
    int memorySize = memSize + 6*sizeof(int);
    if (nameSize > 16) {
        message("Невозможно добавить НСК шлюз из-за превышения допустимой длины имени(>16 байт)");
    } else {
        sharedMemory.setNativeKey(name);
        if (sharedMemory.create(memorySize) == false) {
            message("Creating Shared Memory, ERROR: " + sharedMemory.errorString());
            return;
        }
        int zero[3] = {0, 0, 0};
        memcpy((char*)sharedMemory.data(), (char*)zero, 3*sizeof(int));
        memoryFrom = new char[memSize];
        memoryTo = new char[memSize];
#ifdef Q_OS_WIN32
        tgcaEvent = CreateEventA(NULL, TRUE, FALSE, QString(name + "_").toUtf8().data());
        rtl2Event = CreateEventA(NULL, TRUE, FALSE, QString(name + "__").toUtf8().data());
        if (tgcaEvent == NULL || rtl2Event == NULL) {
            DWORD windowsError = GetLastError();
            qDebug() << "if (tgcaEvent == NULL || rtl2Event == NULL) Error:" << (int)windowsError;
        }
#endif
    }
}

TerasicDevice::~TerasicDevice()
{
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~TerasicDevice() start";
#endif
#ifdef Q_OS_WIN32
    CloseHandle(tgcaEvent);
    CloseHandle(rtl2Event);
#endif
    sharedMemory.detach();
    delete memoryFrom;
    delete memoryTo;
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~TerasicDevice() end";
#endif
}

void TerasicDevice::startLSCGateway()
{
    qDebug() << "startLSCGateway() is OK";
    int ref_isLSCGatewayEnabled = 0*sizeof(int); isLSCGatewayEnabled = 1;
    int ref_RTL2Code = 1*sizeof(int), RTL2Code = 0;
    int ref_TGCACode = 2*sizeof(int), TGCACode = 0;
    int ref_size = 3*sizeof(int), size = 0;
    int ref_startAddr = 4*sizeof(int), startAddr = 0;
    int ref_count = 5*sizeof(int), count = 0;
    int ref_array = 6*sizeof(int);

#ifdef TIMING
    QElapsedTimer timerW;
    QElapsedTimer timerR;
    int msW=0, sW=0, minW=0;
    int msR=0, sR=0, minR=0;
#endif
    try {
        if (sharedMemory.isAttached() == false) {
            //message("Невозможно запустить НСК шлюз");
            throw QString("memory");
        }
        memcpy((char*)sharedMemory.data()+ref_isLSCGatewayEnabled, (char*)&isLSCGatewayEnabled, sizeof(int));
#ifdef Q_OS_LINUX
        int retVal;
        tv.tv_sec = 4; tv.tv_nsec = 0;
        int* uaddrRTL = (int*)((char*)sharedMemory.data()+ref_RTL2Code);
        int* uaddrTGCA = (int*)((char*)sharedMemory.data()+ref_TGCACode);
#endif
#ifdef Q_OS_WIN32
        DWORD retVal;
        DWORD  Milliseconds = 4000;
        BOOL b;
        b = ResetEvent(rtl2Event);
        b = ResetEvent(tgcaEvent);
        //qDebug() << "ResetEvent(rtl2Event), b=" << b;
#endif

        while (1) {
#ifdef Q_OS_LINUX
            retVal = futex(uaddrRTL, FUTEX_WAIT, 0, &tv, NULL, NULL);
            if (retVal == -1) {
                int error = errno;
                if (error == ETIMEDOUT) {
                    qDebug() << "ETIMEDOUT";
                    if (isLSCGatewayEnabled == 0) {
                        memcpy((char*)sharedMemory.data()+ref_isLSCGatewayEnabled, (char*)&isLSCGatewayEnabled, sizeof(int));
                        RTL2Code = 0;
                        memcpy((char*)sharedMemory.data()+ref_RTL2Code, (char*)&RTL2Code, sizeof(int));
                        //message(QString("Остановка работы шлюза НСК из TGCA"));
                        throw QString("stopped");
                    }
                    continue;
                } else if (error == EWOULDBLOCK) {
                    //qDebug() << "EWOULDBLOCK!!!!";
                }
            } else {
                //qDebug() << "error == 0";
            }
            //continue;
#endif
#ifdef Q_OS_WIN32
            retVal = WaitForSingleObject(rtl2Event, Milliseconds);
            if (retVal == WAIT_TIMEOUT) {
                qDebug() << "TIMEOUT";
                if (isLSCGatewayEnabled == 0) {
                    memcpy((char*)sharedMemory.data()+ref_isLSCGatewayEnabled, (char*)&isLSCGatewayEnabled, sizeof(int));
                    RTL2Code = 0;
                    memcpy((char*)sharedMemory.data()+ref_RTL2Code, (char*)&RTL2Code, sizeof(int));
                    b = ResetEvent(rtl2Event);
                    //qDebug() << "ResetEvent(rtl2Event), b=" << b;
                    //message(QString("Остановка работы шлюза НСК из TGCA"));
                    throw QString("stopped");
                }
                continue;
            } else if (retVal == WAIT_OBJECT_0) {
                //qDebug() << "WAIT_OBJECT_0";
                b = ResetEvent(rtl2Event);
                //qDebug() << "ResetEvent(rtl2Event), b=" << b;
            } else if (retVal == WAIT_FAILED) {
                qDebug() << "WAIT_FAILED";
            } else {
                qDebug() << "ELSE ERROR!!!";
            }
            b = ResetEvent(rtl2Event);
#endif
            if (isLSCGatewayEnabled == 0) {
                memcpy((char*)sharedMemory.data()+ref_isLSCGatewayEnabled, (char*)&isLSCGatewayEnabled, sizeof(int));
                RTL2Code = 0;
                memcpy((char*)sharedMemory.data()+ref_RTL2Code, (char*)&RTL2Code, sizeof(int));
                //message(QString("Остановка работы шлюза НСК из TGCA"));
                throw QString("stopped");
            }
            memcpy((char*)&RTL2Code, (char*)sharedMemory.data()+ref_RTL2Code, sizeof(int));
            //qDebug() << "RTL2Code: " << RTL2Code;
            switch (RTL2Code) {
            case ExchangeFormat::zero:

                break;
            case ExchangeFormat::write_f1:
                memcpy((char*)&size, (char*)sharedMemory.data()+ref_size, sizeof(int));
                memcpy(memoryFrom, (char*)sharedMemory.data()+ref_array, size);
#ifdef TIMING
                timerW.start();
#endif
                write_F1(memoryFrom, size);
#ifdef TIMING
                msW += timerW.elapsed();
                while (msW>=1000) {
                    msW-=1000;
                    sW+=1;
                }
                while (sW>=60) {
                    sW-=60;
                    minW+=1;
                }
#endif
                break;
            case ExchangeFormat::write_f2:
                memcpy((char*)&size, (char*)sharedMemory.data()+ref_size, sizeof(int));
                memcpy((char*)&startAddr, (char*)sharedMemory.data()+ref_startAddr, sizeof(int));
                memcpy(memoryFrom, (char*)sharedMemory.data()+ref_array, size);
#ifdef TIMING
                timerW.start();
#endif
                write_F2(startAddr, memoryFrom, size);
#ifdef TIMING
                msW += timerW.elapsed();
                while (msW>=1000) {
                    msW-=1000;
                    sW+=1;
                }
                while (sW>=60) {
                    sW-=60;
                    minW+=1;
                }
#endif
                break;
            case ExchangeFormat::read_f1:
                memcpy((char*)&size, (char*)sharedMemory.data()+ref_size, sizeof(int));
                memcpy(memoryFrom, (char*)sharedMemory.data()+ref_array, size);
#ifdef TIMING
                timerR.start();
#endif
                read_F1(memoryFrom, memoryTo, size);
#ifdef TIMING
                msR += timerR.elapsed();
                while (msR>=1000) {
                    msR-=1000;
                    sR+=1;
                }
                while (sR>=60) {
                    sR-=60;
                    minR+=1;
                }
#endif
                memcpy((char*)sharedMemory.data()+ref_array, memoryTo, size);
                break;
            case ExchangeFormat::read_f2:
                memcpy((char*)&startAddr, (char*)sharedMemory.data()+ref_startAddr, sizeof(int));
                memcpy((char*)&count, (char*)sharedMemory.data()+ref_count, sizeof(int));
#ifdef TIMING
                timerR.start();
#endif
                read_F2(startAddr, count, memoryTo);
#ifdef TIMING
                msR += timerR.elapsed();
                while (msR>=1000) {
                    msR-=1000;
                    sR+=1;
                }
                while (sR>=60) {
                    sR-=60;
                    minR+=1;
                }
#endif
                memcpy((char*)sharedMemory.data()+ref_array, memoryTo, count);
                break;
            default:
                isLSCGatewayEnabled = 0;
                memcpy((char*)sharedMemory.data()+ref_isLSCGatewayEnabled, (char*)&isLSCGatewayEnabled, sizeof(int));
                RTL2Code = 0;
                memcpy((char*)sharedMemory.data()+ref_RTL2Code, (char*)&RTL2Code, sizeof(int));
                //message(QString("Остановка работы шлюза НСК внешним ПО"));
                throw QString("stopped");
            }
            if (RTL2Code != ExchangeFormat::zero) {
                //this->thread()->sleep(10);
                TGCACode = RTL2Code; RTL2Code = 0;
                memcpy((char*)sharedMemory.data()+ref_RTL2Code, (char*)&RTL2Code, sizeof(int));
                memcpy((char*)sharedMemory.data()+ref_TGCACode, (char*)&TGCACode, sizeof(int));
#ifdef Q_OS_LINUX
                int fNumb = futex(uaddrTGCA, FUTEX_WAKE, 5, NULL, NULL, 0);
                //qDebug() << "fNumb: " << fNumb;
#endif
#ifdef Q_OS_WIN32
                BOOL b2 = SetEvent(tgcaEvent);
                //qDebug() << "SetEvent(tgcaEvent), b2= " << b2;
#endif
            }
        }
    } catch(const QString& exception) {
        qDebug() << "startLSCGateway() catch() is OK";
        isLSCGatewayEnabled = 0;
        memcpy((char*)sharedMemory.data()+ref_isLSCGatewayEnabled, (char*)&isLSCGatewayEnabled, sizeof(int));
        RTL2Code = 0;
        memcpy((char*)sharedMemory.data()+ref_RTL2Code, (char*)&RTL2Code, sizeof(int));
        //message(QString("Ошибка сокета при работе шлюза НСК: шлюз остановлен."));
        //throw QString(exception);
        lscGatewayThread.state = exception;
#ifdef TIMING
        qDebug() << "minW: " << QString::number(minW) << " sW: " << QString::number(sW) << " msW: " << QString::number(msW);
        qDebug() << "minR: " << QString::number(minR) << " sR: " << QString::number(sR) << " msR: " << QString::number(msR);
#endif
    }
}
