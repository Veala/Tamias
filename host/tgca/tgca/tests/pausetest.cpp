#include "pausetest.h"

void PauseTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    statsMap.insert("time", stats->findChild<QLabel*>("time"));
    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));

    pause = settings->findChild<QLineEdit*>("pause");
top_1
    pause->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new pauseObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,double)), this, SLOT(statsTestOutput(QString,double)));
    connect(objToThread,SIGNAL(logOutputReady(QString)), this, SIGNAL(logOutputReady(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));

    testThread.start();
}

void PauseTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << pause->text() << endl;
    settingsFile.close();
}

void PauseTest::startTest()
{
    pauseObjToThread* curThread = (pauseObjToThread*)objToThread;
    curThread->pause = pause->text();

    emit startTestTh();
}

void pauseObjToThread::doWork()
{
    try {
        emit resultReady((int)AbstractTest::Running);

        QStringList MMSSmmm = pause.split(":");
        double time = MMSSmmm.at(0).toUInt()*60000 + MMSSmmm.at(1).toUInt()*1000 + MMSSmmm.at(2).toUInt();
        qDebug() << "time: " << time;
        while (1) {
            if (pause_stop() == -1) throw QString("stopped");
            if (time > 2000) {
                thread()->msleep(2000);
                time -= 2000;
            } else {
                thread()->msleep(time);
                time = 0;
            }
            statsOutputReady(QString("time"), time);
            if (time == 0) break;
        }

        throw QString("finish");
    } catch (const QString& exception) {
        if (exception == "stopped") {
            emit resultReady(AbstractTest::Stopped);
        } else if (exception == "finish") {
            emit resultReady(AbstractTest::Completed);
        } else {
            qDebug() << "Else from pausetest";
            emit resultReady(AbstractTest::Stopped);
        }
    }
}
