#include "deviceselectionform.h"
#include "ui_deviceselectionform.h"

DeviceSelectionForm::DeviceSelectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceSelectionForm)
{
    ui->setupUi(this);

    devsList << "Терасик (Обр. 30)" << "Свитч 411";
    ui->devType->insertItems(0, devsList);
    connect(ui->cancelButton, &QPushButton::clicked, [this] () {
        hide();
    });
    connect(ui->okButton, &QPushButton::clicked, [this] () {
        emit addDevice(ui->devName->text(), ui->devType->currentIndex());
    });
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    //setWindowFlags(Qt::WindowStaysOnTopHint);
}

DeviceSelectionForm::~DeviceSelectionForm()
{
    delete ui;
}
