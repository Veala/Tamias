#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    connect(ui->pushButtonCheckDevice, SIGNAL(clicked(bool)), this, SIGNAL(checkDevice(bool)));
    QShortcut* sc = new QShortcut(QKeySequence("Esc"), this);
    QShortcut* sc_host = new QShortcut(QKeySequence("Ctrl+H"), this);
    connect(sc,SIGNAL(activated()),this,SLOT(hide()));
    connect(sc_host,SIGNAL(activated()),this,SLOT(show_hide_host()));
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    show_hide_host();
    ui->labelParam->setVisible(false);
    ui->labelIPPCNet->setVisible(false);
    ui->lineEditIPPCNet->setVisible(false);
    resize(width(), 100);
    forBind = false;
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

Connection::~Connection()
{
    delete ui;
}

QString Connection::getServerIP() const
{
    return ui->lineEditIPDevice->text();
}

QString Connection::getServerPORT() const
{
    return ui->lineEditPortIODevice->text();
}

QString Connection::getHostIP() const
{
    return ui->lineEditIPPCNet->text();
}

void Connection::setServerIP(QString serverIp)
{
    ui->lineEditIPDevice->setText(serverIp);
}

void Connection::setServerPORT(QString PORT)
{
    ui->lineEditPortIODevice->setText(PORT);
}

void Connection::setHostIP(QString hostIp)
{
    ui->lineEditIPPCNet->setText(hostIp);
}

void Connection::setName(QString name)
{
    ui->labelTune->setText(tr("Настройки устройства %1").arg(name));
}

void Connection::show_hide_host()
{
    if (ui->labelParam->isVisible()) {
        ui->labelParam->setVisible(false);
        ui->labelIPPCNet->setVisible(false);
        ui->lineEditIPPCNet->setVisible(false);
        resize(width(), 100);
        forBind = false;
    } else {
        ui->labelParam->setVisible(true);
        ui->labelIPPCNet->setVisible(true);
        ui->lineEditIPPCNet->setVisible(true);
        forBind = true;
    }
}
