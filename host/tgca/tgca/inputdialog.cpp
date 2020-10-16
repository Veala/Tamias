#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}

InputDialog::~InputDialog()
{
    delete ui;
}

QString InputDialog::getStr()
{
    return ui->lineEdit->text();
}

void InputDialog::on_pushButton_clicked()
{
    accept();
}

//void InputDialog::closeEvent(QCloseEvent *event)
//{
//}
