#include "switch411device.h"

Switch411Device::Switch411Device(QWidget *parent, QString name, QTextBrowser *tB) :
    BaseDevice(parent, name, switch411, tB)
{
    connection.setName(name);
    toMfectlDriver.setObjectName(name);
    //connect(&connection, SIGNAL(checkDevice(bool)), this, SLOT(checkDevice()));
    //connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(driverInit()), &mfectlDriver, SLOT(init()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(driverStopAll()), &mfectlDriver, SLOT(stop()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(driverStart(QString,QString)), &mfectlDriver, SLOT(tryToStart(QString,QString)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(driverStop()), &mfectlDriver, SLOT(tryToStop()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(driverExchange()), &mfectlDriver, SLOT(tryToExchange()), Qt::BlockingQueuedConnection);
    mfectlDriver.moveToThread(&toMfectlDriver);
    mfectlDriver.setObjectName(name);
    toMfectlDriver.start();
    emit driverInit();
    mfectlDriver.mutex->lock();
    message(tr("Устройство добавлено"));

}

Switch411Device::~Switch411Device()
{
    //---------------------------------когда тесты работают - добавить корректное удаление девайса
    //sock->abort();
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~Switch411Device() start";
#endif
    emit driverStopAll();
    mfectlDriver.mutex->unlock();
    toMfectlDriver.quit();
    //toSocketDriver.terminate();
    toMfectlDriver.wait();
    emit sigDelete(getName());
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "Device " << getName() << " deleted.";
#endif
    message(tr("Устройство удалено"));
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~Switch411Device() end";
#endif
}

bool Switch411Device::setSettingsFromFile(QString fileName)
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

    connection.setMfectlPath(settings.value("Device/Mfectl").toString());

    setFileSettingsName(fileName);
    return true;
}

void Switch411Device::tryToStart()
{
    mfectlDriver.mutex->unlock();
    mfectlDriver.allData.format = ExchangeFormatSwitch411::start_mfectl;
    emit driverStart(connection.getMfectlPath(), QString(" -r -p -"));
    mfectlDriver.mutex->lock();
    if (mfectlDriver.getMfectlState() != QProcess::Running)
        throw QString("connection");
}

void Switch411Device::tryToStop()
{
    mfectlDriver.mutex->unlock();
    mfectlDriver.allData.format = ExchangeFormatSwitch411::stop_mfectl;
    emit driverStop();
    mfectlDriver.mutex->lock();
    //qDebug() << "after mutex lock";
//    if (mfectlDriver.getMfectlState() != QProcess::Running)
//        throw QString("connection");
    //qDebug() << "mfectl state: " << mfectlDriver.getMfectlState();
}

void Switch411Device::writeReg(BaseSwitch411Reg &reg)
{
    mfectlDriver.mutex->unlock();
    mfectlDriver.allData.format = ExchangeFormatSwitch411::write_reg;
    mfectlDriver.allData.addr = reg.address;
    mfectlDriver.allData.data = (quint16*)&reg + 1;
    emit driverExchange();
    mfectlDriver.mutex->lock();
    if (mfectlDriver.getMfectlState() != QProcess::Running)
        throw QString("process");
}

void Switch411Device::readReg(BaseSwitch411Reg &reg)
{
    mfectlDriver.mutex->unlock();
    mfectlDriver.allData.format = ExchangeFormatSwitch411::read_reg;
    mfectlDriver.allData.addr = reg.address;
    mfectlDriver.allData.data = (quint16*)&reg + 1;
    emit driverExchange();
    mfectlDriver.mutex->lock();
    if (mfectlDriver.getMfectlState() != QProcess::Running)
        throw QString("process");
}

void Switch411Device::showSettings()
{
    connection.show();
}

void Switch411Device::saveSettings()
{
    BaseDevice::saveSettings();
    QSettings devini(getFileSettingsName(), QSettings::IniFormat);
    devini.clear();
    devini.setValue("Device/Mfectl", connection.getMfectlPath());
}

void Switch411Device::doError(QProcess::ProcessError err)
{

}
