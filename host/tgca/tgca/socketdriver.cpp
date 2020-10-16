#include "socketdriver.h"

SocketDriver::~SocketDriver() {
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~SocketDriver() start";
#endif
    if (socket->state() != QAbstractSocket::UnconnectedState)
        socket->abort();
    timer->stop();
    //mutex->unlock();
    delete mutex;
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~SocketDriver() end";
#endif
}

void SocketDriver::init()
{
    mutex = new QMutex();
    //mutex->lock();
    socket = new QTcpSocket(this);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(3000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTOSlot()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectedSlot()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWrittenSlot(qint64)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSlot(QAbstractSocket::SocketError)));
    //qDebug() << "SocketDriver::init(): the end;";
}

void SocketDriver::stop()
{
    timer->stop();
    //mutex->unlock();
    socket->abort();
}

void SocketDriver::connectedSlot()
{
    timer->stop();
    mutex->unlock();
    //qDebug() << "connectedSlot() OK";
}

void SocketDriver::disconnectedSlot()
{
//    timer->stop();                                    //НЕ!!! рабочий вариант для суперускоренной версии
//    mutex->unlock();                                  //НЕ!!! рабочий вариант для суперускоренной версии
    //qDebug() << "disconnectedSlot() OK";
}

void SocketDriver::readyReadSlot()
{

    qint64 bytesAvailable = socket->bytesAvailable();
    //qDebug() << "In readyReadSlot(); bytesAvailable(): " << bytesAvailable;
    qint64 bytesRead = socket->read(allData.tempArray, bytesAvailable);
    //qDebug() << "In readyReadSlot(); real socket->read() bytes: " << bytesRead;

    allData.allDataToRead-=bytesRead;
    allData.tempArray+=bytesRead;

    if (allData.allDataToRead != 0) {
        if (bytesRead != bytesAvailable)
            qDebug() << "Warning: return from readyReadSlot() with (bytesRead != bytesAvailable)!";
        //qDebug() << "In readyReadSlot(); allData.allDataToRead: " << allData.allDataToRead;
        return;
    }

    timer->stop();
    if (allData.format == write_f1 || allData.format == write_f2 || allData.format == write_echo || allData.format == cpy_on_hard) {
        if (allData.littleAnswer != allData.format) {
            qDebug() << "littleAnswer != allData.format";
            socket->abort();
        }
    }
    mutex->unlock();
}

void SocketDriver::bytesWrittenSlot(qint64 bytes)
{
    //qDebug() << "From bytesWrittenSlot(qint64 bytes); bytes: " << bytes;
    //qDebug() << "From bytesWrittenSlot(qint64 bytes); bytesToWrite(): " << socket->bytesToWrite();
//    allData.allDataToWrite-=bytes;
//    int pos = head_and_Data.size()-allData.allDataToWrite;
//    //qDebug() << "In bytesWrittenSlot(qint64 bytes); bytesToWrite(): " << socket->bytesToWrite();
//    if (allData.allDataToWrite)
//        socket->write(head_and_Data.data() + pos, allData.allDataToWrite);
}

void SocketDriver::errorSlot(QAbstractSocket::SocketError error)
{
    qDebug() << "SocketDriver::errorSlot(): error: " << error;
    qDebug() << QString("SocketDriver::errorSlot(): errorString: %1").arg(socket->errorString());
}

void SocketDriver::timerTOSlot()
{
    qDebug() << "SocketDriver::timerTOSlot()";
    socket->abort();
    mutex->unlock();
}

void SocketDriver::tryToConnect(QString ip, ushort port)
{
    socket->setProxy(QNetworkProxy::NoProxy);
    //qDebug() << "SocketDriver::tryToConnect(): before mutex.lock();";
    mutex->lock();
    //qDebug() << "SocketDriver::tryToConnect(): after mutex.lock();";
    timer->start();
    socket->connectToHost(QHostAddress(ip), port);
}

void SocketDriver::tryToDisconnect()
{
    socket->abort();                                                            //рабочий вариант для суперускоренной версии
//    //qDebug() << "SocketDriver::tryToDisconnect(): before mutex.lock();";      //НЕ!!! рабочий вариант для суперускоренной версии
//    mutex->lock();                                                              //НЕ!!! рабочий вариант для суперускоренной версии
//    //qDebug() << "SocketDriver::tryToDisconnect(): after mutex.lock();";       //НЕ!!! рабочий вариант для суперускоренной версии
//    timer->start();                                                             //НЕ!!! рабочий вариант для суперускоренной версии
//    socket->disconnectFromHost();                                               //НЕ!!! рабочий вариант для суперускоренной версии

}

void SocketDriver::tryToExchange()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
        return;
    timer->start();
    head_and_Data.clear();
    allData.littleAnswer = 0;
    switch (allData.format) {
    case write_f1:
        head_and_Data = cmdHead(allData.format, allData.size).append(allData.writeArray, allData.size);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    case write_f2:
        head_and_Data = cmdHead(allData.format, allData.size + 4).append((char*)&allData.startAddr, 4).append(allData.writeArray, allData.size);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    case write_echo:
        head_and_Data = cmdHead(allData.format, allData.size).append(allData.text.toStdString().c_str(), allData.size);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    case read_f1:
        head_and_Data = cmdHead(allData.format, allData.size).append(allData.writeArray, allData.size);
        allData.allDataToRead = allData.size;
        allData.tempArray = allData.readArray;
        break;
    case read_f2:
        head_and_Data = cmdHead(allData.format, allData.size).append((char*)&allData.startAddr, 4).append((char*)&allData.count, 4);
        allData.allDataToRead = allData.count;
        allData.tempArray = allData.readArray;
        break;
    case cpy_on_hard:
        head_and_Data = cmdHead(allData.format, allData.size).append((char*)&allData.fromAddr, 4).append((char*)&allData.count, 4).append((char*)&allData.toAddr, 4);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    default:
        qDebug() << "Error on SocketDriver thread: unknown format from tryToExchange()!";
        return;
    }
    allData.allDataToWrite = head_and_Data.size();
    allData.curPointer = 0;
    mutex->lock();
    //qDebug() << "Before socket->write() in tryExchange()!";
    socket->write(head_and_Data.data(), allData.allDataToWrite);
    //qDebug() << "After socket->write() in tryExchange()!";
}
