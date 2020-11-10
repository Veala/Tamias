#include "switch411basetest.h"

void Switch411BaseTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    statsMap.insert("counter_iter", stats->findChild<QLabel*>("counter_iter"));
    statsMap.insert("counter_err", stats->findChild<QLabel*>("counter_err"));
    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));

    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new switch411BaseObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,double)), this, SLOT(statsTestOutput(QString,double)));
    connect(objToThread,SIGNAL(logOutputReady(QString)), this, SIGNAL(logOutputReady(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));

    testThread.start();
}

void Switch411BaseTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void Switch411BaseTest::startTest()
{
    switch411BaseObjToThread* curThread = (switch411BaseObjToThread*)objToThread;
    curThread->dev = (Switch411Device*)deviceList.at(0);

    emit startTestTh();
}

void switch411BaseObjToThread::doWork()
{
    try {
        emit resultReady((int)AbstractTest::Running);
        dev->tryToStart();

        dev->reg_Switch411_DELAY_START.delay_start = 144;
        dev->writeReg(dev->reg_Switch411_DELAY_START);

        throw QString("finish");
    } catch (const QString& exception) {
        if (exception == "connection") {
            if (pause_stop() == -1) {
                emit resultReady(AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "process") {
            if (pause_stop() == -1) {
                emit resultReady(AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "stopped") {
            dev->tryToStop();
            emit resultReady(AbstractTest::Stopped);
        } else if (exception == "finish") {
            dev->tryToStop();
            emit resultReady(AbstractTest::Completed);
        } else if (exception == "finishWithErrors") {
            dev->tryToStop();
            emit resultReady(AbstractTest::TestFault);
        } else {
            qDebug() << "Else from switch411basetest";
            emit resultReady(AbstractTest::Stopped);
        }
    }
}
