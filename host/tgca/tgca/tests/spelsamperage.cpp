#include "spelsamperage.h"

void SpelsAmperage::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    statsMap.insert("counter_iter", stats->findChild<QLabel *>("counter_iter"));
    statsMap.insert("counter_rt_bc_err", stats->findChild<QLabel *>("counter_rt_bc_err"));
    statsMap.insert("counter_rid_solomon", stats->findChild<QLabel *>("counter_rid_solomon"));
    statsMap.insert("counter_no_aw", stats->findChild<QLabel *>("counter_no_aw"));
    statsMap.insert("amper", stats->findChild<QLabel *>("amper"));

    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));

    ampEnabled = settings->findChild<QComboBox *>("ampEnabled");
    instrument = settings->findChild<QLineEdit *>("instrument");
    startDelay = settings->findChild<QLineEdit *>("startDelay");
    iterationDelay = settings->findChild<QLineEdit *>("iterationDelay");
    number = settings->findChild<QLineEdit *>("number");
    t_r = settings->findChild<QComboBox *>("t_r");
    impulseMode = settings->findChild<QCheckBox *>("impulseMode");
    endExchangeTime = settings->findChild<QSpinBox *>("endExchangeTime");
    iterPause = settings->findChild<QSpinBox *>("iterPause");
    iterNum = settings->findChild<QSpinBox *>("iterNum");
    outputBCstatus = settings->findChild<QComboBox *>("outputBCstatus");
    deviceEditBC = settings->findChild<QLineEdit *>("BC");
    deviceEditRT = settings->findChild<QLineEdit *>("RT");

    deviceLineEditList.append(deviceEditBC);
    deviceLineEditList.append(deviceEditRT);
top_1
    ampEnabled->setCurrentText(out.readLine());
    instrument->setText(out.readLine());
    startDelay->setText(out.readLine());
    iterationDelay->setText(out.readLine());
    number->setText(out.readLine());
    t_r->setCurrentText(out.readLine());
    impulseMode->setChecked(out.readLine().toInt());
    endExchangeTime->setValue(out.readLine().toInt());
    iterPause->setValue(out.readLine().toInt());
    iterNum->setValue(out.readLine().toInt());
    outputBCstatus->setCurrentText(out.readLine());
    deviceEditBC->setText(out.readLine());
    deviceEditRT->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new spelsAmperageObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,double)), this, SLOT(statsTestOutput(QString,double)));
    connect(objToThread,SIGNAL(logOutputReady(QString)), this, SIGNAL(logOutputReady(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    connect(((spelsAmperageObjToThread*)objToThread)->dcThread, SIGNAL(output(QString)), this, SLOT(testOutout(QString)));
    connect(((spelsAmperageObjToThread*)objToThread)->dcThread, SIGNAL(outputStat(QString, double)), this, SLOT(statsTestOutput(QString, double)));
    //connect(((spelsAmperageObjToThread*)objToThread)->dcThread, SIGNAL(outputLog(QString)), this, SIGNAL(logOutputReady(QString)));
    testThread.start();

}

void SpelsAmperage::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << ampEnabled->currentText() << endl;
    in << instrument->text() << endl;
    in << startDelay->text() << endl;
    in << iterationDelay->text() << endl;
    in << number->text() << endl;
    in << t_r->currentText() << endl;
    in << (int)impulseMode->isChecked() << endl;
    in << endExchangeTime->value() << endl;
    in << iterPause->value() << endl;
    in << iterNum->value() << endl;
    in << outputBCstatus->currentText() << endl;
    in << deviceEditBC->text() << endl;
    in << deviceEditRT->text() << endl;
    settingsFile.close();
}

void SpelsAmperage::startTest()
{
    spelsAmperageObjToThread* curThread = (spelsAmperageObjToThread*)objToThread;
#ifdef Q_OS_WIN32
    strcpy(curThread->dcThread->resrc, instrument->text().toLatin1().data());
#endif
    curThread->dcThread->startDelay = startDelay->text().toInt();
    curThread->dcThread->iterationDelay = iterationDelay->text().toInt();
    curThread->dcThread->n = number->text().toInt();

    curThread->ampEnabled = ampEnabled->currentText();
    if (t_r->currentText() == "Команда приема (КШ - ОУ)")
        curThread->t_r = 0;
    else if (t_r->currentText() == "Команда передачи (ОУ - КШ)")
        curThread->t_r = 1;
    curThread->impulseMode = (int)impulseMode->isChecked();
    curThread->endExchangeTime = endExchangeTime->value();
    curThread->iterPause = iterPause->value();
    curThread->iterNum = iterNum->value();
    curThread->outputBCstatus = outputBCstatus->currentText();
    curThread->BC = (TerasicDevice*)deviceList.at(0);
    curThread->RT = (TerasicDevice*)deviceList.at(1);
    curThread->subDevBC = subDeviceList.at(0);
    curThread->subDevRT = subDeviceList.at(1);

    emit startTestTh();
}

spelsAmperageObjToThread::spelsAmperageObjToThread(QObject *parent) : absObjToThread(parent)
{
    dcThread = new DCThread();
}

void spelsAmperageObjToThread::setCmdWord(quint32 t_r)
{
    dev->reg_aux_winmode_reset.reset = 0;
    //dev->reg_aux_winmode_reset.winmode = 0; //последнее что было записано тут сохраняется, менять не надо (если в ПЛИС оно как то не меняется)
    dev->reg_aux_winmode_reset.t_r = t_r;
    dev->writeReg(&dev->reg_aux_winmode_reset);
}

void spelsAmperageObjToThread::doWork()
{
    try {
        emit resultReady((int)AbstractTest::Running);
        if (BC == RT) {
            dev = BC;
        } else {
            throw QString("stopped");
        }

        dev->tryToConnect();

        if (ampEnabled == "Вкл.") {
            dcThread->open();
        }

        dev->reg_aux_winmode_reset.reset = 1;
        dev->reg_aux_winmode_reset.winmode = 0;
        dev->writeReg(&dev->reg_aux_winmode_reset);
        thread()->msleep(1000);
        dev->reg_aux_winmode_reset.reset = 0;
        dev->writeReg(&dev->reg_aux_winmode_reset);

        if (subDevRT == "0") {
            configure(1);
            thread()->msleep(1000);
            configure();
        } else {
            configure();
            thread()->msleep(1000);
            configure(1);
            thread()->msleep(200);
            setDeviceToBC();
        }

        setCmdWord(t_r);

        qint64 i;
        int errorFlag = 0;
        int impErr = 0;
        quint64 bufImpIter = -2;
        if (iterNum == 0) i=-1; else i=0;
        QString logStr;
        if (impulseMode) {
            logStr = "%1\t%2\t%3\n%4";
        }
        else {
            logStr = "%1\t%2\t%3";
        }
        dcThread->amper = "-1000";
        QString status;
        for (quint64 counterIt=0; i<iterNum; i++, counterIt++) {
            if (iterNum == 0) i--;
            if (pause_stop() == -1) {
                throw QString("stopped");
            }

            dev->reg_aux_winmode_reset.winmode = 1;
            dev->writeReg(&dev->reg_aux_winmode_reset);
            thread()->msleep(5);

            quint32 var = 0;
            dev->reg_hsc_creg.setData(var);
            dev->reg_hsc_creg.start_bc = 1;

            emit outputReady(QString("Номер итерации: %1").arg(counterIt));

            if (ampEnabled == "Вкл.") {
                dcThread->isFinished = false;
                dcThread->start();
            }
            dev->writeReg(&dev->reg_hsc_creg);

            thread()->msleep(endExchangeTime);
            dev->reg_aux_winmode_reset.winmode = 0;
            dev->writeReg(&dev->reg_aux_winmode_reset);

            dev->readReg(&dev->reg_hsc_status);
            dev->readReg(&dev->reg_hsc_cnt_pct_rx_lsw);
            dev->readReg(&dev->reg_hsc_cnt_pct_tx_lsw);

            thread()->msleep(iterPause);
            if (ampEnabled == "Вкл.") {
                while (!dcThread->isFinished) {
                    thread()->msleep(100);
                    //emit outputReady(QString("Waiting of DC process"));
                    if (pause_stop() == -1) {
                        throw QString("stopped");
                    }
                }
            }

            QString output;
            emit statsOutputReady("counter_iter", 1);
            if (dev->reg_hsc_status.rt_bc_int && !dev->reg_hsc_status.no_aw_err && !dev->reg_hsc_status.rs_err) {       //&& counterIt != 2 && counterIt != 10 && counterIt != 15 && counterIt != 16
                output.append("Статус: норма");
                bufImpIter=-2;
                status = "Норма";
                if (impulseMode) {
                    errorFlag=0;
                }
            } else {
                errorFlag=1;
                if (counterIt - 1 != bufImpIter) {
                    bufImpIter = counterIt;
                } else {
                    impErr = 1;
                }
                status = "Сбой";
//                if (impulseMode)
//                    emit logOutputReady("Сбой");
                if (!dev->reg_hsc_status.rt_bc_int) {
                    output.append("Ошибка: нет флага прерывания");
                    emit statsOutputReady("counter_rt_bc_err", 1);
                }
                if (dev->reg_hsc_status.no_aw_err) {
                    if (!output.isEmpty()) output.append('\n');
                    output.append("Ошибка: нет ответного слова");
                    emit statsOutputReady("counter_no_aw", 1);
                }
                if (dev->reg_hsc_status.rs_err) {
                    if (!output.isEmpty()) output.append('\n');
                    output.append("Ошибка: декодер Рида-Соломона\n");
                    emit statsOutputReady("counter_rid_solomon", 1);
                }
            }

            if (impulseMode)
                emit logOutputReady(logStr.arg(status).arg(QString::number(counterIt)).arg(dcThread->amper).arg(QDateTime::currentDateTime().toString(tr("dd.MM.yyyy hh:mm:ss"))));
            else
                emit logOutputReady(logStr.arg(status).arg(QString::number(counterIt)).arg(dcThread->amper));

//            if (i < 0)
//                emit logOutputReady(QDateTime::currentDateTime().toString(tr("\ndd.MM.yyyy hh:mm:ss")));


            if (outputBCstatus == "Вкл.") {
                emit outputReady(output);
//                emit outputReady(QString("no_aw_err: %1; rt_bc_int: %2; tx_num_buf: %3").arg(dev->reg_hsc_status.no_aw_err).arg(dev->reg_hsc_status.rt_bc_int).arg(dev->reg_hsc_status.tx_num_buf));
//                emit outputReady(QString("reg_hsc_cnt_pct_rx_lsw.counter: %1").arg(dev->reg_hsc_cnt_pct_rx_lsw.counter));
//                emit outputReady(QString("reg_hsc_cnt_pct_tx_lsw.counter: %1").arg(dev->reg_hsc_cnt_pct_tx_lsw.counter));
            }

            if (ampEnabled == "Вкл.") {
#ifdef Q_OS_WIN32
                if (dcThread->error != VI_SUCCESS) {
                    emit outputReady("Неисправность: блок питания стал недоступен");
                    if (pause_stop() == -1)
                        throw QString("stopped");
                    throw QString("DC error");
                }
#endif
            }
            if (impulseMode && impErr) {
                throw QString("finishWithErrors");
            }
            //emit outputReady(QString("\n"));
        }


        if (!errorFlag || impulseMode == 1)
            throw QString("finish");
        else
            throw QString("finishWithErrors");
    } catch (const QString& exception) {
        if (ampEnabled == "Вкл.") {
            dcThread->isFinished = true;
            dcThread->close();
        }
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
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::Completed);
        } else if (exception == "finishWithErrors") {
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::TestFault);
        } else if (exception == "Open DC error") {
            dev->tryToDisconnect();
            if (pause_stop() == -1) {
                emit resultReady(AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "DC error") {
            dev->tryToDisconnect();
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else {
            qDebug() << "else from spelsAmperage";
            emit resultReady(AbstractTest::Stopped);
        }
    }
}

void spelsAmperageObjToThread::configure(quint32 rt_bc, quint32 type_man, quint32 ena_codec, quint32 addr)
{
    if (rt_bc == 0) {
        setDeviceToBC();
    } else if (rt_bc == 1) {
        setDeviceToRT();
    }

    QVector<BaseReg *> regs;
    regs.append(&dev->reg_hsc_creg);
    regs.append(&dev->reg_hsc_lvl_sync_kf_rx_msw);
    regs.append(&dev->reg_hsc_lvl_sync_kf_rx_lsw);
    regs.append(&dev->reg_hsc_lvl_sync_pre_rx_msw);
    regs.append(&dev->reg_hsc_lvl_sync_pre_rx_lsw);
    regs.append(&dev->reg_hsc_lvl_qam16);
    regs.append(&dev->reg_hsc_lvl_qam64_low);
    regs.append(&dev->reg_hsc_lvl_qam64_midle);
    regs.append(&dev->reg_hsc_lvl_qam64_high);
    regs.append(&dev->reg_hsc_cfg);
    regs.append(&dev->reg_hsc_time_rsp);

    quint32 var;
    var = 0x0007; dev->reg_hsc_creg.setData(var);
    var = 0x0000; dev->reg_hsc_lvl_sync_kf_rx_msw.setData(var);
    var = 0x0200; dev->reg_hsc_lvl_sync_kf_rx_lsw.setData(var);
    var = 0x0040; dev->reg_hsc_lvl_sync_pre_rx_msw.setData(var);
    var = 0x0000; dev->reg_hsc_lvl_sync_pre_rx_lsw.setData(var);
    var = 0x1BBE; dev->reg_hsc_lvl_qam16.setData(var);
    var = 0x0DF7; dev->reg_hsc_lvl_qam64_low.setData(var);
    var = 0x1BBE; dev->reg_hsc_lvl_qam64_midle.setData(var);
    var = 0x29E5; dev->reg_hsc_lvl_qam64_high.setData(var);
    var = 0x0000; dev->reg_hsc_cfg.setData(var);
    dev->reg_hsc_cfg.en_rt_bc_int = 1;
    dev->reg_hsc_cfg.ena_mem_vsk = 0;
    dev->reg_hsc_cfg.rt_bc = rt_bc;
    dev->reg_hsc_cfg.rtavsk_ena = 1;
    dev->reg_hsc_cfg.rtavsk = 0;
    dev->reg_hsc_cfg.type_man = type_man;
    dev->reg_hsc_cfg.ena_codec = ena_codec;
    var = 0xC350; dev->reg_hsc_time_rsp.setData(var);

    dev->writeRegs(regs);

    if (rt_bc == 1) {
        dev->reg_aux_rtaddr.addr = addr;
        dev->writeReg(&dev->reg_aux_rtaddr);
    }

    var = 0x3048;
    dev->reg_hsc_pll_reg.setData(var);
    dev->writeReg(&dev->reg_hsc_pll_reg);
}

void spelsAmperageObjToThread::setDeviceToBC()
{
    dev->reg_aux_bus_select.bus = subDevBC.toUInt();
    dev->writeReg(&dev->reg_aux_bus_select);
}

void spelsAmperageObjToThread::setDeviceToRT()
{
    dev->reg_aux_bus_select.bus = subDevRT.toUInt();
    dev->writeReg(&dev->reg_aux_bus_select);
}
