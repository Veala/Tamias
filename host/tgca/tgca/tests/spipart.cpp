#include "spipart.h"

void SpiPart::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    statsMap.insert("counter_iter", stats->findChild<QLabel*>("counter_iter"));
    statsMap.insert("counter_err", stats->findChild<QLabel*>("counter_err"));
    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));

    //disableStat();

    table = settings->findChild<QTableWidget*>("table");
    linesEdit = settings->findChild<QLineEdit*>("lines");
    cycleChBox = settings->findChild<QCheckBox*>("cycle");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    int rows = 32, columns = 2;
    for (int i=0; i<rows; i++)
    {
        table->insertRow(i);
        for (int j=0; j<columns; j++) {
            QLineEdit* line = new QLineEdit(out.readLine());
            line->setInputMask("HHHH;_");
            table->setCellWidget(i,j,line);
//            table->setItem(i,j,new QTableWidgetItem(out.readLine()));
        }
    }
    linesEdit->setText(out.readLine());
    cycleChBox->setChecked(out.readLine().toInt());
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new spiObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,double)), this, SLOT(statsTestOutput(QString,double)));
    connect(objToThread,SIGNAL(logOutputReady(QString)), this, SIGNAL(logOutputReady(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void SpiPart::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    int rows = 32, columns = 2;
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<columns; j++)
//            in << table->item(i,j)->data(Qt::DisplayRole).toString() << endl;
            in << ((QLineEdit*)table->cellWidget(i,j))->text() << endl;
    }
    in << linesEdit->text() << endl;
    in << cycleChBox->isChecked() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void SpiPart::startTest()
{
    spiObjToThread* curThread = (spiObjToThread*)objToThread;
    curThread->addr.clear();
    curThread->data.clear();
    for (int i=0; i<32; i++)
    {
//        curThread->addr.append(table->item(i,0)->data(Qt::DisplayRole).toString().toUInt(nullptr, 16));
//        curThread->data.append(table->item(i,1)->data(Qt::DisplayRole).toString().toUInt(nullptr, 16));
        curThread->addr.append(((QLineEdit*)table->cellWidget(i,0))->text().toUInt(nullptr, 16));
        curThread->data.append(((QLineEdit*)table->cellWidget(i,1))->text().toUInt(nullptr, 16));
    }

    curThread->dev = (TerasicDevice*)deviceList.at(0);
    curThread->subDev = subDeviceList.at(0);
    curThread->lines = linesEdit->text().toInt();
    curThread->cycle = cycleChBox->isChecked();

    emit startTestTh();
}

void spiObjToThread::doWork()
{
    try {
        emit resultReady((int)AbstractTest::Running);
        dev->tryToConnect();

        QVector<BaseReg*> regs;
        regs.append(&dev->reg_hsc_dr_spi_lsw);
        regs.append(&dev->reg_hsc_cr_spi);

        dev->reg_aux_bus_select.bus = subDev.toUInt();
        dev->writeReg(&dev->reg_aux_bus_select);

        do {
            thread()->msleep(500);
            for (int i=0; i<lines; i+=2)
            {
                dev->reg_hsc_dr_spi_lsw.data=data.at(i);
                *(((quint32*)&dev->reg_hsc_cr_spi)+1) = data.at(i+1);
                dev->writeRegs(regs);
//                int n = 0;
                QTimer timer;
                timer.setInterval(50);
                timer.start();
                while (1) {
                    dev->readReg(&dev->reg_hsc_cr_spi);
                    if (dev->reg_hsc_cr_spi.spif == 1) {
                        //emit outputReady(tr("reg_cr_spi.spif == %1").arg(dev->reg_hsc_cr_spi.spif,0,16));
                        break;
                    } else {
//                        n++;
                        //emit outputReady(tr("reg_cr_spi.spif != %1").arg(dev->reg_hsc_cr_spi.spif,0,16));
//                        if (n>1000) throw QString("unknownHardError");
                        if (timer.remainingTime() <= 0) throw QString("SPI Finalization Error");
                    }
                }

                dev->reg_hsc_cr_spi.spif = 0;
                if (dev->reg_hsc_cr_spi.getData() != data.at(i+1))  throw QString("SPI Comparison Error");
                //threadState = AbstractTest::Paused;
                if (pause_stop() == -1) throw QString("stopped");
            }
        } while (cycle);

        emit outputReady(tr("SPI загружено"));
        throw QString("finish");
    } catch (const QString& exception) {
        if (exception == "connection") {
            if (pause_stop() == -1) {
                emit resultReady(AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "socket") {
            if (pause_stop() == -1) {
                emit resultReady(AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "stopped") {
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::Stopped);
        } else if (exception == "finish") {
            emit statsOutputReady("counter_iter", 1);
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::Completed);
        } else if (exception == "unknownHardError") {
            emit statsOutputReady("counter_err", 1);
            dev->tryToDisconnect();
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else {
            emit statsOutputReady("counter_err", 1);
            dev->tryToDisconnect();
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        }
    }
}
