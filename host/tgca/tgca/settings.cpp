#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QShortcut* sc = new QShortcut(QKeySequence("Esc"), this);
    connect(sc,SIGNAL(activated()),this,SLOT(hide()));
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::WindowStaysOnTopHint);

//    QRegExp rx("[0-9]+");
//    validator.setRegExp(rx);
//    ui->timeLineEdit->setText("00:00.000");
//    ui->timeLineEdit->setValidator(&validator);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setPauseChecked(bool ch)
{
    ui->pauseCheckbox->setChecked(ch);
}

bool Settings::getPauseChecked()
{
    return ui->pauseCheckbox->isChecked();
}

void Settings::setLogFileName(QString name)
{
    ui->logFileName->setText(name);
}

QString Settings::getLogFileName()
{
    return ui->logFileName->text();
}

void Settings::setPauseTime(QString time)
{
    ui->timeLineEdit->setText(time);
}

QString Settings::getPauseTime()
{
    return ui->timeLineEdit->text();
}
