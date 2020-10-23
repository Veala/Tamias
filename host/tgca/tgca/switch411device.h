#ifndef SWITCH411DEVICE_H
#define SWITCH411DEVICE_H

#include <QSettings>
#include <QProcess>
#include "basedevice.h"
#include "switch411devicesettings.h"

class Switch411Device : public BaseDevice
{
    Q_OBJECT

public:
    explicit Switch411Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    virtual ~Switch411Device();

    Switch411DeviceSettings connection;
    bool setSettingsFromFile(QString fileName) override;

    void tryToConnect();
    void write(ushort addr, ushort data);
    ushort read(ushort addr);

protected:
    void showSettings() override;
    void saveSettings() override;

private:
    QProcess m_Mfectl;
    QStringList m_simpleWriteCmd;
    QStringList m_simpleReadCmd;
};

#endif // SWITCH411DEVICE_H
