#include "memtest.h"

void MemTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    statsMap.insert("counter_iter", stats->findChild<QLabel*>("counter_iter"));
    statsMap.insert("counter_err", stats->findChild<QLabel*>("counter_err"));
    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));

    mode = settings->findChild<QComboBox*>("mode");
    checkReadData = settings->findChild<QCheckBox*>("checkReadData");
    memType = settings->findChild<QComboBox *>("memType");
    startData = settings->findChild<QLineEdit *>("startData");
    incData = settings->findChild<QSpinBox *>("incData");
    inversion = settings->findChild<QComboBox*>("inverse");
    iteration = settings->findChild<QSpinBox *>("iteration");
    oput = settings->findChild<QComboBox*>("output");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    mode->setCurrentText(out.readLine());
    checkReadData->setChecked(out.readLine().toInt());
    memType->setCurrentText(out.readLine());
    startData->setText(out.readLine());
    incData->setValue(out.readLine().toInt());
    inversion->setCurrentText(out.readLine());
    iteration->setValue(out.readLine().toInt());
    oput->setCurrentText(out.readLine());
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new memObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,double)), this, SLOT(statsTestOutput(QString,double)));
    connect(objToThread,SIGNAL(logOutputReady(QString)), this, SIGNAL(logOutputReady(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));

    connect(memType, SIGNAL(activated(QString)), this, SLOT(activatedMemType(QString)));
    activatedMemType(memType->currentText());
    testThread.start();
}

void MemTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << mode->currentText() << endl;
    in << (int)checkReadData->isChecked() << endl;
    in << memType->currentText() << endl;
    in << startData->text() << endl;
    in << incData->text() << endl;
    in << inversion->currentText() << endl;
    in << iteration->text() << endl;
    in << oput->currentText() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void MemTest::startTest()
{
    memObjToThread* curThread = (memObjToThread*)objToThread;
    curThread->mode = mode->currentText();
    curThread->checkReadData = (int)checkReadData->isChecked();
    curThread->memType = memType->currentText();
    curThread->data = startData->text().toUInt(NULL, 16);
    curThread->datainc = incData->value();
    curThread->inverse = inversion->currentText().toUInt();
    curThread->inCycle = iteration->value();
    curThread->output = oput->currentText().toUInt();
    curThread->dev = deviceList.at(0);
    curThread->subDev = subDeviceList.at(0);

    emit startTestTh();
}

void MemTest::activatedMemType(QString type)
{
    if (type=="Память НСК" || type=="Регистры НСК" || type=="Регистры ВСК") {
        startData->setInputMask("HHHH;_");
    } else if (type=="Память ВСК: буфер 1" || type=="Память ВСК: буфер 2") {
        startData->setInputMask("HHHHHHHH;_");
    }

}

void memObjToThread::doWork()
{
    try {
        emit resultReady((int)AbstractTest::Running);
        dev->tryToConnect();

        dev->reg_aux_winmode_reset.reset = 1;
        setWinmode(0);
        thread()->msleep(1000);
        dev->reg_aux_winmode_reset.reset = 0;
        dev->writeReg(&dev->reg_aux_winmode_reset);

        dev->reg_aux_bus_select.bus = subDev.toUInt();
        dev->writeReg(&dev->reg_aux_bus_select);

        quint32 var = 0x0000;
        dev->reg_hsc_ram_tx_rx.setData(var);
        dev->reg_hsc_cfg.setData(var);

        quint32 addr, range, addrinc = 4;
        if (memType == "Память ВСК: буфер 1" || memType == "Память ВСК: буфер 2") {
            addr = 0x8000;
            range = 0x8fff;
            dev->reg_hsc_cfg.ena_mem_vsk = 1;
            dev->writeReg(&dev->reg_hsc_cfg);
            if (memType == "Память ВСК: буфер 1")
                dev->reg_hsc_ram_tx_rx.buf_1 = 1;
            else if (memType == "Память ВСК: буфер 2")
                dev->reg_hsc_ram_tx_rx.buf_2 = 1;
            dev->writeReg(&dev->reg_hsc_ram_tx_rx);
        } else if (memType == "Память НСК") {
            addr = 0x4000;
            range = 0x7fff;
            dev->writeReg(&dev->reg_hsc_cfg);
            dev->writeReg(&dev->reg_hsc_ram_tx_rx);
        } else if (memType == "Регистры НСК") {
            addr = 0x0000;
            range = 0x002c;
            addrinc = 40;
            dev->writeReg(&dev->reg_hsc_cfg);
            dev->writeReg(&dev->reg_hsc_ram_tx_rx);
        } else if (memType == "Регистры ВСК") {
            addr = 0x00c8;
            range = 0x00e8;
            dev->writeReg(&dev->reg_hsc_cfg);
            dev->writeReg(&dev->reg_hsc_ram_tx_rx);
        }

        long it = 0, decrement = 0;
        if (inCycle == 0) { it=-1;  decrement=-1;   }

        int dsz;

        uint temp = (((range - addr + 1) / addrinc) * addrinc) + addr - 1 + 4;
        if (temp <= range)   dsz = ((range - addr + 1) / addrinc + 1)*4;
        else    dsz = ((range - addr + 1) / addrinc)*4;

        QByteArray writeArray, answer;
        QByteArray readArray;

        if (mode == "w") {
            uint final;
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;
                writeArray.append((char*)&i, 4);
                writeArray.append((char*)&final, 4);
            }

            for (uint i1=addr; it<inCycle; it=it+1+decrement) {
                setWinmode(1);
                dev->write_F1(writeArray.data(), dsz*2);
                setWinmode(0);

                for (int i=0; i<writeArray.size(); i+=8) {
                    if (output)
                        emit outputReady(QString("Write: (%1)  %2").arg((uint)*(int*)(writeArray.data()+i), 8, 16, QLatin1Char('0')).arg((uint)*(int*)(writeArray.data()+i+4), 8, 16, QLatin1Char('0')));
                }

                if (pause_stop() == -1) {
                    writeArray.clear();
                    throw QString("stopped");
                }
                emit statsOutputReady("counter_iter", 1);
            }

            writeArray.clear();
            throw QString("finish");
        } else if (mode == "r") {
            for (uint i=addr; i+3<=range; i+=addrinc)
                readArray.append((char*)&i, 4);
            answer.resize(readArray.size());

            uint final;
            ulong same=0, diff=0;
            for (uint i1=addr; it<inCycle; it=it+1+decrement) {
                setWinmode(1);
                dev->read_F1(readArray.data(), answer.data(), readArray.size());
                setWinmode(0);

                if (!checkReadData) {
                    for (int i=0; i<answer.size(); i+=4, i1+=addrinc) {
                        if (output)
                            emit outputReady(QString("Read: (%1)  %2").arg(i1, 8, 16, QLatin1Char('0')).arg((uint)*(int*)(answer.data()+i), 8, 16, QLatin1Char('0')));
                    }
                    emit statsOutputReady("counter_iter", 1);
                }

                if (checkReadData) {
                    for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                        if (inverse) final = ~j;    else    final = j;
                        writeArray.append((char*)&i, 4);
                        writeArray.append((char*)&final, 4);
                    }

                    same=0; diff=0;
                    uint w,r,a;
                    for (int i=0, j=4; i<answer.size(); i+=4, j+=8) {
                        w = *(int*)(writeArray.data()+j);
                        r = *(int*)(answer.data()+i);
                        a = *(int*)(writeArray.data()+j-4);
                        if (output) emit outputReady(tr("Read: (%1) %2").arg(a, 8, 16, QLatin1Char('0')).arg(r, 8, 16, QLatin1Char('0')));
                        if (w != r) {
                            //if (output) emit outputReady(tr("<b>Read: (%1) %2</b>").arg(a, 8, 16, QLatin1Char('0')).arg(r, 8, 16, QLatin1Char('0')));
                            diff++;
                        } else {
                            //if (output) emit outputReady(tr("Read: (%1) %2").arg(a, 8, 16, QLatin1Char('0')).arg(r, 8, 16, QLatin1Char('0')));
                            same++;
                        }
                    }
                    emit outputReady(tr("Write!=Read: %1;    Write==Read: %2").arg(QString::number(diff)).arg(QString::number(same)));
                    emit statsOutputReady("counter_iter", 1);
                    emit statsOutputReady("counter_err", diff);

                    writeArray.clear();
                    readArray.clear();
                    answer.clear();
                    if (!diff)
                        throw QString("finish");
                    else
                        throw QString("finishWithErrors");
                }
                if (pause_stop() == -1) {
                    writeArray.clear();
                    readArray.clear();
                    answer.clear();
                    throw QString("stopped");
                }
            }

            writeArray.clear();
            readArray.clear();
            answer.clear();
            throw QString("finish");
        } else if (mode == "wr") {
            uint final;
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;
                writeArray.append((char*)&i, 4);
                writeArray.append((char*)&final, 4);
                readArray.append((char*)&i, 4);
            }
            answer.resize(readArray.size());

            ulong same=0, diff=0;
            for (; it<inCycle; it=it+1+decrement) {
                setWinmode(1);
                dev->write_F1(writeArray.data(), writeArray.size());
                dev->read_F1(readArray.data(), answer.data(), readArray.size());
                setWinmode(0);

                same=0; diff=0;
                uint w,r,a;
                for (int i=0, j=4; i<answer.size(); i+=4, j+=8) {
                    w = *(int*)(writeArray.data()+j);
                    r = *(int*)(answer.data()+i);
                    a = *(int*)(writeArray.data()+j-4);
                    if (output) emit outputReady(tr("(%1) Write: %2; Read: %3").arg(a, 8, 16, QLatin1Char('0')).arg(w, 8, 16, QLatin1Char('0')).arg(r, 8, 16, QLatin1Char('0')));
                    if (w != r) {
                        //if (output) emit outputReady(tr("<b>(%1) Write: %2; Read: %3</b>").arg(a, 8, 16, QLatin1Char('0')).arg(w, 8, 16, QLatin1Char('0')).arg(r, 8, 16, QLatin1Char('0')));
                        diff++;
                    } else {
                        //if (output) emit outputReady(tr("(%1) Write: %2; Read: %3").arg(a, 8, 16, QLatin1Char('0')).arg(w, 8, 16, QLatin1Char('0')).arg(r, 8, 16, QLatin1Char('0')));
                        same++;
                    }
                }
                emit outputReady(tr("Write!=Read: %1;    Write==Read: %2").arg(QString::number(diff)).arg(QString::number(same)));
                emit statsOutputReady("counter_iter", 1);
                emit statsOutputReady("counter_err", diff);
                if (pause_stop() == -1) {
                    writeArray.clear();
                    readArray.clear();
                    answer.clear();
                    throw QString("stopped");
                }
            }

            writeArray.clear();
            readArray.clear();
            answer.clear();
            if (!diff)
                throw QString("finish");
            else
                throw QString("finishWithErrors");
        }
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
            setWinmode(0);
            if (memType == "Память ВСК: буфер 1" || memType == "Память ВСК: буфер 2") {
                dev->reg_hsc_cfg.ena_mem_vsk = 0;
                dev->writeReg(&dev->reg_hsc_cfg);
            }
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::Stopped);
        } else if (exception == "finish") {
            if (memType == "Память ВСК: буфер 1" || memType == "Память ВСК: буфер 2") {
                if (memType == "Память ВСК: буфер 1")
                    dev->reg_hsc_ram_tx_rx.buf_1 = 0;
                else if (memType == "Память ВСК: буфер 2")
                    dev->reg_hsc_ram_tx_rx.buf_2 = 0;
                dev->writeReg(&dev->reg_hsc_ram_tx_rx);

                dev->reg_hsc_cfg.ena_mem_vsk = 0;
                dev->writeReg(&dev->reg_hsc_cfg);
            }
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::Completed);
        } else if (exception == "finishWithErrors") {
            if (memType == "Память ВСК: буфер 1" || memType == "Память ВСК: буфер 2") {
                if (memType == "Память ВСК: буфер 1")
                    dev->reg_hsc_ram_tx_rx.buf_1 = 0;
                else if (memType == "Память ВСК: буфер 2")
                    dev->reg_hsc_ram_tx_rx.buf_2 = 0;
                dev->writeReg(&dev->reg_hsc_ram_tx_rx);

                dev->reg_hsc_cfg.ena_mem_vsk = 0;
                dev->writeReg(&dev->reg_hsc_cfg);
            }
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::TestFault);
        } else {
            qDebug() << "Else from memtest";
            emit resultReady(AbstractTest::Stopped);
        }
    }
}

void memObjToThread::setWinmode(int mode)
{
    if (memType == "Память ВСК: буфер 1" || memType == "Память ВСК: буфер 2") {
        if (mode == 1) {
            dev->reg_aux_winmode_reset.winmode = 1;
            dev->writeReg(&dev->reg_aux_winmode_reset);
            thread()->msleep(5);
        } else if (mode == 0) {
            thread()->msleep(50);
            dev->reg_aux_winmode_reset.winmode = 0;
            dev->writeReg(&dev->reg_aux_winmode_reset);
        }
    }
}
