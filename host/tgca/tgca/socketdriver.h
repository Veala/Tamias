#ifndef SOCKETDRIVER_H
#define SOCKETDRIVER_H

#include <QObject>
#include <QMutex>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QTimer>
#include "funclib.h"

class SocketDriver : public QObject
{
    Q_OBJECT

public:
    explicit SocketDriver(QObject *parent = 0) : QObject(parent) { } //, timer(NULL), socket(NULL), mutex(NULL) { }
    ~SocketDriver();
    QAbstractSocket::SocketState getSocketState() {
        return socket->state();
    }
    //state по read write придумать или переменные
    //QMutex mutex;

    QByteArray head_and_Data;

    struct AllData {
        ExchangeFormat format;
        char* writeArray = NULL;
        char* readArray = NULL;
        int littleAnswer = 0;
        qint64 curPointer = 0;
        char* tempArray = NULL;
        int size = 0;
        int startAddr = 0;
        int count = 0;
        QString text;
        int fromAddr = 0;
        int toAddr = 0;
        int allDataToWrite = 0;
        int allDataToRead = 0;
    } allData;
    QMutex* mutex;

private:
    QTcpSocket* socket;
    QTimer* timer;

private slots:
    void init();
    void stop();
    void connectedSlot();
    void disconnectedSlot();
    void readyReadSlot();
    void bytesWrittenSlot(qint64);
    void errorSlot(QAbstractSocket::SocketError);
    void timerTOSlot();

public slots:
    void tryToConnect(QString ip, ushort port);
    void tryToDisconnect();
    void tryToExchange();
};

#endif // SOCKETDRIVER_H
