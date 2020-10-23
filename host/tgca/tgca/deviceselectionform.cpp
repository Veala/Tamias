#include "deviceselectionform.h"
#include "ui_deviceselectionform.h"

DeviceSelectionForm::DeviceSelectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceSelectionForm)
{
    ui->setupUi(this);

    QShortcut* sc = new QShortcut(QKeySequence("Esc"), this);
    connect(sc,SIGNAL(activated()),this,SLOT(hide()));
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    devsList << "Терасик (Обр. 30)" << "Свитч 411";
    ui->devType->insertItems(0, devsList);
    connect(ui->cancelButton, &QPushButton::clicked, [this] () {
        hide();
    });
    connect(ui->okButton, &QPushButton::clicked, [this] () {
        emit addDevice(ui->devName->text(), ui->devType->currentIndex());
    });
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

DeviceSelectionForm::~DeviceSelectionForm()
{
    delete ui;
}
