#ifndef LSCGATEWAY_H
#define LSCGATEWAY_H

#include <QThread>
#include <QDebug>

class TerasicDevice;

class LSCGatewayThread : public QThread
{
    Q_OBJECT
    TerasicDevice* dev;
public:
    explicit LSCGatewayThread(QObject *parent = 0) : QThread(parent) { }
    ~LSCGatewayThread() {
#ifdef PRINT_START_END_DESTRUCTOR
        qDebug() << "~LSCGatewayThread()";
#endif
    }
    void setDevice(TerasicDevice* d)  { dev = d; }
    QString state;
protected:
    virtual void run() Q_DECL_OVERRIDE;
};

#endif // LSCGATEWAY_H
