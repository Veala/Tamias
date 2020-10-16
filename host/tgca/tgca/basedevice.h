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
    explicit BaseDevice(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    virtual ~BaseDevice();

    bool isMonitor();
    virtual void setName(QString);
    QString getName() const;
    void setType(int);
    int getType() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void message(QString);

    QMenu menu;
    QTextBrowser* projectBrowser;

private:
    Ui::BaseDevice *ui;
    int m_type;

private slots:
    void tryToDelete();
};

#endif // BASEDEVICE_H
