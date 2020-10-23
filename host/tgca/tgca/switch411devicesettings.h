#ifndef SWITCH411DEVICESETTINGS_H
#define SWITCH411DEVICESETTINGS_H

#include <QWidget>
#include <QShortcut>

namespace Ui {
class Switch411DeviceSettings;
}

class Switch411DeviceSettings : public QWidget
{
    Q_OBJECT

public:
    explicit Switch411DeviceSettings(QWidget *parent = nullptr);
    ~Switch411DeviceSettings();

    QString getMfectlPath() const;
    void setMfectlPath(QString);
    void setName(QString);

private:
    Ui::Switch411DeviceSettings *ui;
};

#endif // SWITCH411DEVICESETTINGS_H
