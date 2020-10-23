#include "baseterasicdevice.h"

BaseTerasicDevice::BaseTerasicDevice(QWidget *parent, QString name, QTextBrowser *tB) :
    BaseDevice(parent, name, terasic, tB)
{
    connection.setName(name);
    toSocketDriver.setObjectName(name);
    connect(&connection, SIGNAL(checkDevice(bool)), this, SLOT(checkDevice()));
    //connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(tcpInit()), &socketDriver, SLOT(init()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(stopAll()), &socketDriver, SLOT(stop()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(tcpConnect(QString,ushort)), &socketDriver, SLOT(tryToConnect(QString,ushort)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(tcpDisconnect()), &socketDriver, SLOT(tryToDisconnect()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(tcpExchange()), &socketDriver, SLOT(tryToExchange()), Qt::BlockingQueuedConnection);
    socketDriver.moveToThread(&toSocketDriver);
    toSocketDriver.setObjectName(name);
    socketDriver.setObjectName(name);
    toSocketDriver.start();
    emit tcpInit();
    socketDriver.mutex->lock();
    message(tr("Устройство добавлено"));
}

BaseTerasicDevice::~BaseTerasicDevice()
{
    //---------------------------------когда тесты работают - добавить корректное удаление девайса
    //sock->abort();
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~BaseTerasicDevice() start";
#endif
    emit stopAll();
    socketDriver.mutex->unlock();
    toSocketDriver.quit();
    //toSocketDriver.terminate();
    toSocketDriver.wait();
    emit sigDelete(getName());
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "Device " << getName() << " deleted.";
#endif
    message(tr("Устройство удалено"));
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~BaseTerasicDevice() end";
#endif
}

void BaseTerasicDevice::tryToConnect()
{
    socketDriver.mutex->unlock();
    emit tcpConnect(connection.getServerIP(), connection.getServerPORT().toUShort());
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("connection");
}

void BaseTerasicDevice::tryToDisconnect()
{
//    socketDriver.mutex->unlock();                                               //НЕ!!! рабочий вариант для суперускоренной версии
    emit tcpDisconnect();                                                       //Для всего
//    //qDebug() << "before mutex lock";
//    socketDriver.mutex->lock();                                                 //НЕ!!! рабочий вариант для суперускоренной версии
//    //qDebug() << "after mutex lock";
//    if (socketDriver.getSocketState() != QAbstractSocket::UnconnectedState)     //НЕ!!! рабочий вариант для суперускоренной версии
//        throw QString("connection");                                            //НЕ!!! рабочий вариант для суперускоренной версии
//    //qDebug() << "socket state: " << socketDriver.getSocketState();
}

void BaseTerasicDevice::write_F1(char* writeArray, int size)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::write_f1;
    socketDriver.allData.size = size;
    socketDriver.allData.writeArray = writeArray;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseTerasicDevice::write_F2(int startAddr, char *writeArray, int size)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::write_f2;
    socketDriver.allData.size = size;
    socketDriver.allData.writeArray = writeArray;
    socketDriver.allData.startAddr = startAddr;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseTerasicDevice::write_Echo(QString &text)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::write_echo;
    socketDriver.allData.text = text;
    socketDriver.allData.size = text.size()+1;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseTerasicDevice::read_F1(char *writeArray, char *readArray, int wrsize)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::read_f1;
    socketDriver.allData.writeArray = writeArray;
    socketDriver.allData.readArray = readArray;
    socketDriver.allData.size = wrsize;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseTerasicDevice::read_F2(int startAddr, int count, char *readArray)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::read_f2;
    socketDriver.allData.startAddr = startAddr;
    socketDriver.allData.count = count;
    socketDriver.allData.size = 8; //sizeof(count) + sizeof(startAddr)
    socketDriver.allData.readArray = readArray;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseTerasicDevice::cpyOnHard(int fromAddr, int count, int toAddr)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::cpy_on_hard;
    socketDriver.allData.fromAddr = fromAddr;
    socketDriver.allData.count = count;
    socketDriver.allData.toAddr = toAddr;
    socketDriver.allData.size = 12; //sizeof(fromAddr) + sizeof(count) + sizeof(toAddr)
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseTerasicDevice::writeReg(BaseReg *reg)
{
    write_F1((char*)reg, RegLen);
}

void BaseTerasicDevice::readReg(BaseReg *reg)
{
    read_F1((char*)reg, (char*)reg+BaseRegLen, BaseRegLen);
}

void BaseTerasicDevice::writeRegs(QVector<BaseReg *>& regs)
{
    QByteArray array;
    foreach (BaseReg* r, regs) {
        array.append((char*)r, RegLen);
    }
    write_F1(array.data(), array.size());
}

void BaseTerasicDevice::readRegs(QVector<BaseReg *>& regs)
{
    QByteArray array;
    QByteArray answer; answer.resize(regs.size()*BaseRegLen);
    foreach (BaseReg* r, regs) {
        array.append((char*)r, BaseRegLen);
    }
    read_F1(array.data(), answer.data(), regs.size()*BaseRegLen);
    for (int i=0; i<regs.size(); i++) {
        *((quint32*)regs[i]+1) = *((quint32*)answer.data()+i);
    }
}

void BaseTerasicDevice::showSettings()
{
    connection.show();
}

void BaseTerasicDevice::saveSettings()
{
    BaseDevice::saveSettings();
    QSettings devini(getFileSettingsName(), QSettings::IniFormat);
    devini.clear();
    devini.setValue("Device/IP", connection.getServerIP());
    devini.setValue("Device/port", connection.getServerPORT());
    devini.setValue("Host/IP", connection.getHostIP());
}

bool BaseTerasicDevice::setSettingsFromFile(QString fileName)
{
    if (!BaseDevice::setSettingsFromFile(fileName))
        return false;

    QSettings settings(fileName, QSettings::IniFormat);
    if (settings.status() != QSettings::NoError)
    {
        qDebug() << "Error: " << settings.status();
        setFileSettingsName("");
        return false;
    }

    connection.setServerIP(settings.value("Device/IP").toString());
    connection.setServerPORT(settings.value("Device/port").toString());
    connection.setHostIP(settings.value("Host/IP").toString());

    setFileSettingsName(fileName);
    return true;
}

void BaseTerasicDevice::doError(QAbstractSocket::SocketError err)
{
    //message(tr("Ошибка: %1").arg());
}

void BaseTerasicDevice::checkDevice()
{
    QTcpSocket sock1;
    sock1.setProxy(QNetworkProxy::NoProxy);
    if (connection.forBind) {
        if (!sock1.bind(QHostAddress(connection.getHostIP()))) {
            message(sock1.errorString());
            return;
        }
    }
    sock1.connectToHost(QHostAddress(connection.getServerIP()), connection.getServerPORT().toUShort());
    if (!sock1.waitForConnected(5000)) {
        message(QString(tr("Не удалось установить соединение: ")) + sock1.errorString());
        return;
    }
    sock1.disconnectFromHost();
    if (!(sock1.state() == QAbstractSocket::UnconnectedState ||
              sock1.waitForDisconnected(5000))) {
        message(tr("Устройство найдено."));
        message(QString(tr("Ошибка: ")) + sock1.errorString());
        return;
    }
    message(tr("Устройство найдено и готово к работе."));
}
