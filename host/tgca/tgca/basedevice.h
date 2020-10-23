#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QFrame>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextBrowser>
#include <QDateTime>
#include "globalstate.h"

namespace Ui {
class BaseDevice;
}

class BaseDevice : public QFrame, public GlobalState
{
    Q_OBJECT

signals:
    //signals to tests
    void sigDelete(QString);
    void sigConnectedDevice();
    void sigDisconnectedDevice();

public:
    explicit BaseDevice(QWidget *parent = 0, QString name = "default", int type = 0, QTextBrowser *tB = NULL);
    virtual ~BaseDevice();

    bool isMonitor();
    void setName(QString);
    QString getName() const;
    virtual void setType(int);
    int getType() const;
    QString getFileSettingsName() const;
    void setFileSettingsName(QString fileSettingsName);
    virtual bool setSettingsFromFile(QString fileName);

    enum DeviceTypes {
        terasic,
        switch411
    };

protected:
    void mousePressEvent(QMouseEvent *event);
    void message(QString);
    virtual void showSettings() = 0;
    virtual void saveSettings();

    QMenu menu;
    QTextBrowser* projectBrowser;

private:
    Ui::BaseDevice *ui;
    int m_type;
    QString m_fileSettingsName;

private slots:
    void tryToDelete();
};

#endif // BASEDEVICE_H
