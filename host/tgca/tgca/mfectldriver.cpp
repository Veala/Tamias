#include "mfectldriver.h"

MfectlDriver::~MfectlDriver() {
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~MfectlDriver() start";
#endif
    if (process->state() != QProcess::NotRunning) {
       // process->abort();   //TODO
    }
    timer->stop();
    //mutex->unlock();
    delete mutex;
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~MfectlDriver() end";
#endif
}

void MfectlDriver::init()
{
    mutex = new QMutex();
    process = new QProcess(this);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(3000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTOSlot()));
    connect(process, SIGNAL(started()), this, SLOT(startedSlot()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedSlot(int, QProcess::ExitStatus)));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStanOutSlot()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStanErrSlot()));
    connect(process, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWrittenSlot(qint64)));
    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(errorSlot(QProcess::ProcessError)));
    //qDebug() << "MfectlDriver::init(): the end;";
}

void MfectlDriver::stop()
{
    timer->stop();
    //mutex->unlock();
    //process->abort(); //TODO
}

void MfectlDriver::startedSlot()
{
    //надо получить вывод от redayRead о том, что устройство успешно открыто, останавливать таймер и разблокировать мьютекс; если устройство не открылось успешно, то ждать сигнала finished, останавливать таймер и мьютекс; если же ничего не произошло, останавливать программу.
//    timer->stop();
//    mutex->unlock();
    qDebug() << "startedSlot() OK";
}

void MfectlDriver::finishedSlot(int exitCode, QProcess::ExitStatus exitStatus)
{
    timer->stop();
    mutex->unlock();
    qDebug() << "finishedSlot() OK";
    qDebug() << "exitCode: " << exitCode;
    qDebug() << "exitStatus: " << exitStatus;
}

void MfectlDriver::readyReadStanOutSlot()
{
    qDebug() << "readyReadStanOutSlot: ";
    timer->stop();
    switch (allData.format) {
    case start_mfectl:
        qDebug() << process->readAllStandardOutput();
        break;
    case stop_mfectl:
        qDebug() << process->readAllStandardOutput();
        break;
    case write_reg:
        qDebug() << process->readAllStandardOutput();
        break;
    case read_reg:
        qDebug() << process->readAllStandardOutput();
        break;
    default:
        qDebug() << "Error on MfectlDriver thread: unknown format from tryToExchange()!";
        return;
    }

    mutex->unlock();

//    qint64 bytesAvailable = process->bytesAvailable();
//    //qDebug() << "In readyReadSlot(); bytesAvailable(): " << bytesAvailable;
//    qint64 bytesRead = process->read(allData.tempArray, bytesAvailable);
//    //qDebug() << "In readyReadSlot(); real process->read() bytes: " << bytesRead;

//    allData.allDataToRead-=bytesRead;
//    allData.tempArray+=bytesRead;

//    if (allData.allDataToRead != 0) {
//        if (bytesRead != bytesAvailable)
//            qDebug() << "Warning: return from readyReadSlot() with (bytesRead != bytesAvailable)!";
//        //qDebug() << "In readyReadSlot(); allData.allDataToRead: " << allData.allDataToRead;
//        return;
//    }

//    timer->stop();
//    if (allData.format == write_f1 || allData.format == write_f2 || allData.format == write_echo || allData.format == cpy_on_hard) {
//        if (allData.littleAnswer != allData.format) {
//            qDebug() << "littleAnswer != allData.format";
//            //process->abort();    //TODO
//        }
//    }
    //    mutex->unlock();
}

void MfectlDriver::readyReadStanErrSlot()
{
    qDebug() << "readyReadStanErrSlot: ";
    timer->stop();
    switch (allData.format) {
    case start_mfectl:
        qDebug() << process->readAllStandardError();
        break;
    case stop_mfectl:
        qDebug() << process->readAllStandardError();
        break;
    case write_reg:
        qDebug() << process->readAllStandardError();
        break;
    case read_reg:
        qDebug() << process->readAllStandardError();
        break;
    default:
        qDebug() << "Error on MfectlDriver thread: unknown format from tryToExchange()!";
        return;
    }

    mutex->unlock();
}

void MfectlDriver::bytesWrittenSlot(qint64 bytes)
{
    qDebug() << "From bytesWrittenSlot(qint64 bytes); bytes: " << bytes;
    //qDebug() << "From bytesWrittenSlot(qint64 bytes); bytesToWrite(): " << process->bytesToWrite();
//    allData.allDataToWrite-=bytes;
//    int pos = head_and_Data.size()-allData.allDataToWrite;
//    //qDebug() << "In bytesWrittenSlot(qint64 bytes); bytesToWrite(): " << process->bytesToWrite();
//    if (allData.allDataToWrite)
//        process->write(head_and_Data.data() + pos, allData.allDataToWrite);
}

void MfectlDriver::errorSlot(QProcess::ProcessError error)
{
    qDebug() << "MfectlDriver::errorSlot(): error: " << error;
    qDebug() << QString("MfectlDriver::errorSlot(): errorString: %1").arg(process->errorString());
}

void MfectlDriver::timerTOSlot()
{
    qDebug() << "MfectlDriver::timerTOSlot()";
    process->closeWriteChannel();    //TODO
   // mutex->unlock();
}

void MfectlDriver::tryToStart(QString program, QString args)
{
    //qDebug() << "MfectlDriver::tryToConnect(): before mutex.lock();";
    mutex->lock();
    //qDebug() << "MfectlDriver::tryToConnect(): after mutex.lock();";
    timer->start();

    process->start(program + args);
}

void MfectlDriver::tryToStop()
{
    //process->abort();    //TODO                                                            //рабочий вариант для суперускоренной версии
    //qDebug() << "MfectlDriver::tryToDisconnect(): before mutex.lock();";
    mutex->lock();
    //qDebug() << "MfectlDriver::tryToDisconnect(): after mutex.lock();";
    timer->start();
    process->closeWriteChannel();

}

void MfectlDriver::tryToExchange()
{
    if (process->state() == QProcess::NotRunning)
        return;

    mutex->lock();
    timer->start();

    //qDebug() << "Before process->write() in tryExchange()!";
    switch (allData.format) {
    case write_reg:
         // m_Mfectl.write(m_simpleWriteCmd.arg(QString::number(addr >> 5)).arg(QString::number(addr & 0x1F, 16)).arg(QString::number(data, 16)).toUtf8());
        process->write(m_simpleWriteCmd.arg(QString::number(allData.addr & 0x1F, 16)).arg(QString::number(*allData.data, 16)).toUtf8());
        break;
    case read_reg:
        // m_Mfectl.write(m_simpleWriteCmd.arg(QString::number(addr >> 5)).arg(QString::number(addr & 0x1F, 16)).toUtf8());
        process->write(m_simpleReadCmd.arg(QString::number(allData.addr & 0x1F, 16)).toUtf8());
        break;
    default:
        qDebug() << "Error on MfectlDriver thread: unknown format from tryToExchange()!";
        return;
    }
    //qDebug() << "After process->write() in tryExchange()!";
}
