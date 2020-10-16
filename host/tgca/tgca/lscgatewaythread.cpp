#include "lscgatewaythread.h"
#include "tests/abstracttest.h"

void LSCGatewayThread::run()
{
//    try {
        dev->startLSCGateway();
//    } catch(const QString& exception) {
//        state = exception;
//        qDebug() << "LSCGatewayThread state: " << state;
//    }
}
