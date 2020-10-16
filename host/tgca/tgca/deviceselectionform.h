#ifndef DEVICESELECTIONFORM_H
#define DEVICESELECTIONFORM_H

#include <QWidget>

namespace Ui {
class DeviceSelectionForm;
}

class DeviceSelectionForm : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceSelectionForm(QWidget *parent = nullptr);
    ~DeviceSelectionForm();

private:
    Ui::DeviceSelectionForm *ui;
    QStringList devsList;

signals:
    void addDevice(QString name, int type);
};

#endif // DEVICESELECTIONFORM_H
