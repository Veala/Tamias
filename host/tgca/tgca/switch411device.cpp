#include "switch411device.h"

Switch411Device::Switch411Device(QWidget *parent, QString name, QTextBrowser *tB) :
    BaseDevice(parent, name, switch411, tB)
{
    connection.setName(name);
    m_simpleWriteCmd << "--phyad" << " " << "-r" << " ";
    m_simpleWriteCmd << "--phyad" << " " << "-r" << " ";
    message(tr("Устройство добавлено"));
}

Switch411Device::~Switch411Device()
{

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

void Switch411Device::tryToConnect()
{
    if (!QFile::exists(connection.getMfectlPath()))
        throw QString("connection");
    m_Mfectl.setProgram(connection.getMfectlPath());
}

void Switch411Device::write(ushort addr, ushort data)
{
    m_simpleWriteCmd.replace(1, QString::number(addr >> 5));
    m_simpleWriteCmd.replace(1, QString::number(addr & 0x1F, 16) + QString("=") + QString::number(data, 16));
    m_Mfectl.setArguments(m_simpleWriteCmd);
    m_Mfectl.start();
    if(!m_Mfectl.waitForFinished(5000)) {
        m_Mfectl.kill();
        throw QString("Error: waitForFinished. Process was killed");
    }
}

ushort Switch411Device::read(ushort addr)
{
    m_simpleReadCmd.replace(1, QString::number(addr >> 5));
    m_simpleReadCmd.replace(1, QString::number(addr & 0x1F, 16));
    m_Mfectl.setArguments(m_simpleReadCmd);
    m_Mfectl.start();
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
