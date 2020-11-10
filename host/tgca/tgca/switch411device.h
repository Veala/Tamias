#ifndef SWITCH411DEVICE_H
#define SWITCH411DEVICE_H

#include <QSettings>
#include <QThread>
#include "basedevice.h"
#include "mfectldriver.h"
#include "switch411devicesettings.h"
#include "data_structs_switch411.h"

class Switch411Device : public BaseDevice
{
    Q_OBJECT

signals:
    void error(QProcess::ProcessError err);

    void driverInit();
    void driverStopAll();
    void driverStart(QString, QString);
    void driverStop();
    void driverExchange();

public:
    explicit Switch411Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    virtual ~Switch411Device();

    Switch411DeviceSettings connection;

    bool setSettingsFromFile(QString fileName) override;
    void tryToStart();
    void tryToStop();

    void writeReg(BaseSwitch411Reg &reg);
    void readReg(BaseSwitch411Reg &reg);

    REG_Switch411_CTRL reg_Switch411_CTRL; REG_Switch411_RESET reg_Switch411_RESET; REG_Switch411_STATUS reg_Switch411_STATUS;
    REG_Switch411_OLD_TKPA reg_Switch411_OLD_TKPA; REG_Switch411_CFG_NRSHM reg_Switch411_CFG_NRSHM; REG_Switch411_START_STOP reg_Switch411_START_STOP;
    REG_Switch411_CFG_SGVM502 reg_Switch411_CFG_SGVM502; REG_Switch411_DELAY_START reg_Switch411_DELAY_START; REG_Switch411_COUNTER_TEST reg_Switch411_COUNTER_TEST;

protected:
    void showSettings() override;
    void saveSettings() override;

private:
    MfectlDriver mfectlDriver;
    QThread toMfectlDriver;

private slots:
    void doError(QProcess::ProcessError err);
};

#endif // SWITCH411DEVICE_H
