#include "basedevice.h"
#include "ui_basedevice.h"

BaseDevice::BaseDevice(QWidget *parent, QString name, int type, QTextBrowser *tB) :
    QFrame(parent),
    ui(new Ui::BaseDevice)

{
    ui->setupUi(this);

    setName(name);
    setType(type);
    projectBrowser = tB;

    QAction *act = menu.addAction(tr("Настроить устройство"));
    connect(act, &QAction::triggered, this, &BaseDevice::showSettings);
    act = menu.addAction(tr("Сохранить настройки"));
    connect(act, &QAction::triggered, this, &BaseDevice::saveSettings);
    act = menu.addAction(tr("Удалить устройство"));
    connect(act, &QAction::triggered, this, &BaseDevice::tryToDelete);

    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    QPalette palette;
    QBrush br(Qt::blue); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    //ui->picture->setPixmap(QPixmap(tr(":/pictogram/hardware_8356.png")));
    ui->monitor->setEnabled(false);
    ui->monitor->setVisible(false);

    message(tr("Устройство добавлено"));
}

BaseDevice::~BaseDevice()
{
    //---------------------------------когда тесты работают - добавить корректное удаление девайса
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~BaseDevice() start";
#endif
    emit sigDelete(getName());
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "BaseDevice " << getName() << " deleted.";
#endif
    message(tr("Устройство удалено"));
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~BaseDevice() end";
#endif
}

bool BaseDevice::isMonitor()
{
    return ui->monitor->isChecked();
}

void BaseDevice::setName(QString name)
{
    ui->name->setText(name);
}

QString BaseDevice::getName() const
{
    return ui->name->text();
}

void BaseDevice::setType(int type)
{
    m_type = type;
}

int BaseDevice::getType() const
{
    return m_type;
}

void BaseDevice::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        menu.exec(QCursor::pos());
    } else if (event->buttons() == Qt::LeftButton) {

    }
}

void BaseDevice::message(QString m)
{
    QDateTime local(QDateTime::currentDateTime());
    projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + ui->name->text() + ". " + m);
}

void BaseDevice::saveSettings()
{
    if (getFileSettingsName().isEmpty())
    {
        QString deviceName = getName();
        if (deviceName.size() == 0)
            deviceName = "device_default";
        setFileSettingsName(deviceName + QString(".dev"));
    }
}

bool BaseDevice::setSettingsFromFile(QString fileName)
{
    if (fileName.isEmpty())
    {
        setFileSettingsName("");
        return false;
    }
    return true;
}

QString BaseDevice::getFileSettingsName() const
{
    return m_fileSettingsName;
}

void BaseDevice::setFileSettingsName(QString fileSettingsName)
{
    m_fileSettingsName = fileSettingsName;
}

void BaseDevice::tryToDelete()
{
    if (getGlobalState() == FREE) {
        deleteLater();
    }
    else {
        message(tr("Ошибка: устройство не может быть удалено при работающих тестах."));
    }
}
