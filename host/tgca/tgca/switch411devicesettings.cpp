#include "switch411devicesettings.h"
#include "ui_switch411devicesettings.h"

Switch411DeviceSettings::Switch411DeviceSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Switch411DeviceSettings)
{
    ui->setupUi(this);

    QShortcut* sc = new QShortcut(QKeySequence("Esc"), this);
    connect(sc,SIGNAL(activated()),this,SLOT(hide()));
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

Switch411DeviceSettings::~Switch411DeviceSettings()
{
    delete ui;
}

QString Switch411DeviceSettings::getMfectlPath() const
{
    return ui->lineMfectlPath->text();
}

void Switch411DeviceSettings::setMfectlPath(QString mfectlPath)
{
    ui->lineMfectlPath->setText(mfectlPath);
}

void Switch411DeviceSettings::setName(QString name)
{
    ui->labelTune->setText(tr("Настройки устройства %1").arg(name));
}
