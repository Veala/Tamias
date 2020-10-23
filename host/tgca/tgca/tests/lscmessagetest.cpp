#include "lscmessagetest.h"
#include "../regNSK.h"

void LscMessageTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    setStatSettings();

    defineFields();
    load(fName);

    // Скрываем расширенные настроойки.
    //labelNumMes->setEnabled(false);
    //spinBox->setEnabled(false);
    labelNumMes->setVisible(false);
    spinBox->setVisible(false);
    checkBoxCheckAll->setChecked(false);
    checkBoxCheckAll->setVisible(false);
    checkBoxCheckAll->setEnabled(false);

    deviceLineEditList.append(deviceEditBC);
    deviceLineEditList.append(deviceEditRT);
    checkDeviceAvailability(3);

    connect(comboBoxRTA, SIGNAL(activated(int)), this, SLOT(onRTA()));
    connect(checkBoxAutorepeat, SIGNAL(clicked()), this, SLOT(onAuto()));
    connect(radioButtonSet, SIGNAL(toggled(bool)), this, SLOT(onRadio()));
    connect(radioButtonUnlim, SIGNAL(toggled(bool)), this, SLOT(onRadio()));

    onAuto();

    objToThread = new lscMessageObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread, SIGNAL(resultReady(int)), objToThread, SLOT(terminate(int)));
    connect(objToThread,SIGNAL(setResultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,double)), this, SLOT(statsTestOutput(QString,double)));
    connect(objToThread,SIGNAL(logOutputReady(QString)), this, SIGNAL(logOutputReady(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void LscMessageTest::statsTestOutput(QString str, double n)
{
    QMap<QString, QLabel*>::iterator it = statsMap.find(str);
    if (it != statsMap.end())
    {
        QLabel* l = it.value();
        uint val = l->text().toUInt() + (uint)n;
        l->setText(QString::number(val));
        if (str != "totalIter")
        {
            QFont f = l->font();
            if (val == 0)
            {
                f.setBold(false);
                l->setStyleSheet(QLatin1String("color: rgb(0, 170, 0);\n"
                                               "background-color: rgb(255, 255, 255);"));
            }
            else
            {
                f.setBold(true);
                l->setStyleSheet(QLatin1String("color: rgb(255, 0, 0);\n"
                                               "background-color: rgb(255, 255, 255);"));
            }
            l->setFont(f);
        }
    }
}

void LscMessageTest::setStatSettings()
{
    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalErr", stats->findChild<QLabel*>("totalErr"));
    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));
}

void LscMessageTest::statsSave()
{
    QString saveStatsFileNameStr = QFileDialog::getSaveFileName(this, QObject::tr("Введите имя файла"), QObject::tr(""));
    if (!saveStatsFileNameStr.isEmpty()) {
        QFile f(saveStatsFileNameStr);
        f.open(QIODevice::WriteOnly);

        QByteArray str;
        bool b = (stats->findChild<QLabel*>("labelOnOff")->text() == "Да  ");
        str.append("Автоповтор    :  ");
        if (b)
            str.append("Да\nИтераций      :  N/A\nОшибок обмена :  ");
        else
        {
            str.append("Нет\nИтераций      :  ");
            str.append(stats->findChild<QLabel*>("totalIter")->text());
            str.append("\nОшибок обмена :  ");
        }
        str.append(stats->findChild<QLabel*>("totalErr")->text());
        str.append("\n");
        f.write(str);
        f.close();
    }
}

void LscMessageTest::load(QString fName)
{
    top_1
        deviceEditBC->setText(out.readLine());
        deviceEditRT->setText(out.readLine());

        checkBoxConfigBC->setChecked(!out.readLine().isEmpty());
        checkBoxConfigRT->setChecked(!out.readLine().isEmpty());
        checkBoxStartMessages->setChecked(!out.readLine().isEmpty());
        checkBoxAutorepeat->setChecked(!out.readLine().isEmpty());
        checkBoxCheckRW->setChecked(!out.readLine().isEmpty());
        checkBoxCheckAll->setChecked(!out.readLine().isEmpty());
        comboBoxRTA->setCurrentText(out.readLine());

        lineEditDW->setText(out.readLine());  // hex
        checkBoxPulse->setChecked(!out.readLine().isEmpty());
        comboBoxNW->setCurrentText(out.readLine());
        checkBoxOut->setChecked(!out.readLine().isEmpty());
        spinBox->setValue(out.readLine().toInt());
        comboBoxBus->setCurrentIndex(out.readLine().toInt());

        // Эта группа активна при выключенном автоповторе.
        lineEditPause->setText(out.readLine());
        lineEditNum->setText(out.readLine());
        radioButtonUnlim->setChecked(out.readLine()== "Unlim");
        radioButtonSet->setChecked(!radioButtonUnlim->isChecked());
        //int cycleNum = lineEditNum->text().isEmpty() ? 0 : lineEditNum->text().toInt();

        // Эта группа активна при включенном автоповторе.
        lineEditFT->setText(out.readLine());

        // Значения параметров не редактируются, но берутся из этих полей.
        lineMesRTA->setText(comboBoxRTA->currentText());
        //lineMesSA->setText("1");
        //lineMesType->setText("Приём данных");

        settingsFile.close();
}

void LscMessageTest::defineFields()
{
    deviceEditBC = settings->findChild<QLineEdit*>("deviceEditBC");
    deviceEditRT = settings->findChild<QLineEdit*>("deviceEditRT");

    checkBoxStartMessages = settings->findChild<QCheckBox*>("checkBoxStartMessages");
    checkBoxConfigRT = settings->findChild<QCheckBox*>("checkBoxConfigRT");
    checkBoxAutorepeat = settings->findChild<QCheckBox*>("checkBoxAutorepeat");
    checkBoxPulse = settings->findChild<QCheckBox*>("checkBoxPulse");
    checkBoxCheckRW = settings->findChild<QCheckBox*>("checkBoxCheckRW");
    checkBoxCheckAll = settings->findChild<QCheckBox*>("checkBoxCheckAll");
    checkBoxConfigBC = settings->findChild<QCheckBox*>("checkBoxConfigBC");
    checkBoxOut = settings->findChild<QCheckBox*>("checkBoxOut");
    comboBoxRTA = settings->findChild<QComboBox*>("comboBoxRTA");

    lineEditDW = settings->findChild<QLineEdit*>("lineEditDW");
    comboBoxNW = settings->findChild<QComboBox*>("comboBoxNW");
    comboBoxBus = settings->findChild<QComboBox*>("comboBoxBus");

    // Эта группа активна при выключенном автоповторе.
    labelHeaderRepeat = settings->findChild<QLabel*>("labelHeaderRepeat");
    labelPause = settings->findChild<QLabel*>("labelPause");
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");
    radioButtonSet = settings->findChild<QRadioButton*>("radioButtonSet");
    radioButtonUnlim = settings->findChild<QRadioButton*>("radioButtonUnlim");
    lineEditNum = settings->findChild<QLineEdit*>("lineEditNum");

    // Эта группа активна при включенном автоповторе.
    labelFT = settings->findChild<QLabel*>("labelFT");
    lineEditFT = settings->findChild<QLineEdit*>("lineEditFT");

    // Значения параметров не редактируются, но берутся из этих полей.
    lineMesRTA = settings->findChild<QLabel*>("lineMesRTA");
    lineMesSA = settings->findChild<QLabel*>("lineMesSA");
    lineMesType = settings->findChild<QLabel*>("lineMesType");

    // Пока скрыты
    labelNumMes = settings->findChild<QLabel*>("labelNumMes");
    spinBox = settings->findChild<QSpinBox*>("spinBox");
}

void LscMessageTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << deviceEditBC->text() << endl;
    in << deviceEditRT->text() << endl;

    in << (checkBoxConfigBC->isChecked() ? "1" : "") << endl;
    in << (checkBoxConfigRT->isChecked() ? "1" : "") << endl;
    in << (checkBoxStartMessages->isChecked() ? "1" : "") << endl;
    in << (checkBoxAutorepeat->isChecked() ? "1" : "") << endl;
    in << (checkBoxCheckRW->isChecked() ? "1" : "") << endl;
    in << (checkBoxCheckAll->isChecked() ? "1" : "") << endl;

    in << comboBoxRTA->currentText() << endl;

    in << lineEditDW->text() << endl;
    in << (checkBoxPulse->isChecked() ? "1" : "") << endl;
    in << comboBoxNW->currentText() << endl;
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;

    in << spinBox->value() << endl;
    in << comboBoxBus->currentIndex() << endl;

    // Эта группа активна при выключенном автоповторе.
    in << lineEditPause->text() << endl;
    in << lineEditNum->text() << endl;
    in << (radioButtonUnlim->isChecked() ? "Unlim" : "Lim") << endl;

    // Эта группа активна при включенном автоповторе.
    in << lineEditFT->text() << endl;

    settingsFile.close();
}

void LscMessageTest::onRadio()
{
    lineEditNum->setEnabled(radioButtonSet->isChecked());
}

void LscMessageTest::onRTA()
{
    lineMesRTA->setText(comboBoxRTA->currentText());
}

void LscMessageTest::onAuto()
{
    bool b = checkBoxAutorepeat->isChecked();

    labelHeaderRepeat->setEnabled(!b);
    labelPause->setEnabled(!b);
    lineEditPause->setEnabled(!b);
    radioButtonSet->setEnabled(!b);
    radioButtonUnlim->setEnabled(!b);
    lineEditNum->setEnabled(!b && radioButtonSet->isChecked());
    checkBoxPulse->setEnabled(!b);

    labelFT->setEnabled(b);
    lineEditFT->setEnabled(b);

    stats->findChild<QLabel*>("labelOnOff")->setText(b? "Да  " : "Нет ");
    QLabel * l1 = stats->findChild<QLabel*>("labelIter");
    QLabel * l2 = stats->findChild<QLabel*>("totalIter");

    l1->setEnabled(!b);
    l2->setEnabled(!b);
    QFont f = l1->font();
    f.setBold(!b);
    l1->setFont(f);
    l2->setFont(f);
}

void LscMessageTest::statsToZero()
{
    foreach (QLabel* l, statsMap) {
        l->setText("0");
        if (l->objectName() != "totalIter")
        {
            l->setStyleSheet(QLatin1String("color: rgb(0, 170, 0);\n"
                                           "background-color: rgb(255, 255, 255);"));
            QFont f = l->font();
            f.setBold(false);
            l->setFont(f);
        }
    }
}

void LscMessageTest::startTest()
{
    lscMessageObjToThread* curThread = (lscMessageObjToThread*)objToThread;
    curThread->dev = (TerasicDevice*)deviceList.at(0);
    //curThread->devRT = deviceList.at(1);
    curThread->busBC = subDeviceList.at(0).toUInt();
    curThread->busRT = subDeviceList.at(1).toUInt();

    curThread->initBC = checkBoxConfigBC->isChecked();
    curThread->initRT = checkBoxConfigRT->isChecked();
    curThread->startBC = checkBoxStartMessages->isChecked();
    curThread->autorepeat = checkBoxAutorepeat->isChecked();
    curThread->impulse = !curThread->autorepeat && checkBoxPulse->isChecked();
    curThread->checkRW = checkBoxCheckRW->isChecked();
    curThread->checkRTBC = checkBoxCheckAll->isChecked();
    curThread->setOutEnabled = checkBoxOut->isChecked();
    curThread->rtaddr = comboBoxRTA->currentIndex();  // 0-30
    curThread->nw = comboBoxNW->currentIndex()+1;     // 1-32
    curThread->sa = lineMesSA->text().toInt();
    curThread->torta = lineMesRTA->text().toInt();
    curThread->dataWord = lineEditDW->text().toInt(0, 16);
    curThread->mFormat = lineMesType->text();
    curThread->num_messages = spinBox->value();
    curThread->bus = comboBoxBus->currentIndex();

    if (checkBoxAutorepeat->isChecked())
    {
        curThread->ft = lineEditFT->text().toInt();
        curThread->pause = 100; //lineEditPause->text().toInt();
    }
    else
    {
        curThread->Count = radioButtonUnlim->isChecked() ? -1 : (lineEditNum->text().isEmpty() ? 0 : lineEditNum->text().toInt());
        curThread->pause = lineEditPause->text().toInt();
    }

    emit startTestTh();
}

/////////////////////////////////////////////////////////////////////////////////

lscMessageObjToThread::lscMessageObjToThread():
    absObjToThread(),
    dev(0),
    started(false)

{
}

lscMessageObjToThread::~lscMessageObjToThread()
{
    if (started)
    {
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}

bool lscMessageObjToThread::checkMessage(C_LSC_Format* format)
{
    if (checkRTBC)
    {
        return format->checkRW() && format->checkSWBC() && format->checkDataRT(busRT, busBC);
    }
    else if (checkRW)
    {
        return format->checkRW();
    }
    return true;
}

void lscMessageObjToThread::doWork()
{
    //bool checkEna = false, spoil_fast = false;
    //enaAccessRT = true; //initRT;
    bool check_time = false, stopped = false;
    stackRT = 0;  // 0 - А, 1 - Б
    stackBC = 0;  // 0 - А, 1 - Б

    int errors = 0;
    bool fatal = false;
    fixRT = 0x100+stackRT*4;
    fixBC = 0x100+stackBC*4;
    curCommandStackBC = initCommandStackBC;
    curCommandStackRT = initCommandStackRT;
    ptrMessagesBC = initMessageArea;

    //qDebug() << "lscMessageObjToThread::doWork() started";
    try
    {
        emit resultReady((int)AbstractTest::Running);

        dev->tryToConnect();

        // Настройка ОУ.
        if (initRT)
        {
            configRT(initCommandStackRT);
        }

        if (initRT && (initBC || startBC))
        {
            pauseForSwitchDev();
        }

        // Настройка КШ.
        if (initBC)
        {
            configBC(); // конфигурация регистров НСК КШ
        }

        if (startBC)
        {
            if (!initBC)
                switchToDev(busBC);
             // создание сообщений и их запись в память НСК КШ
            std::vector<C_LSC_Format*> messages;
            createMessages(messages);

            CWriteDeviceMemory bufStartBC;
            initStartMessages(bufStartBC);

            if (autorepeat)
            {
                // Старт КШ.
                startMessages(bufStartBC);
                started = true;

                do
                {
                    thread()->msleep(pause);
                    for (unsigned i=0; i< messages.size(); i++)
                    {
                        if (!checkMessage(messages[i]))
                        {
//                            //if (errors < 30)
                                stdOutput(tr("Ошибка передачи сообщения. Найдено %1 ошибок.").arg(errors));
                            emit statsOutputReady("totalErr",1);
                            errors ++;
                        }
                    }
                }
                while (pause_stop() != -1);

                writeRegSingle(mr_NUMREG_COMMAND, 0x20);
                started = false;
            }
            else if (Count != 0)
            {
                int counter = 0;
                bool prevErr = false, curErr = false;
                if (check_time)
                {
                    QDateTime local(QDateTime::currentDateTime());
                    stdOutput(QString("Время начала цикла: ") + local.toString(tr("dd.MM.yyyy hh:mm:ss")));
                }
                for ( ; counter!=Count; counter++)
                {
                    //if (counter%100 == 0) qDebug() << "count=" << counter;
                    if (pause_stop() == -1)
                    {
                        //if (Count > 0)
                            stopped = true;
                        break;
                    }

                    // Старт сообщения
                    startMessages(bufStartBC);
                    emit statsOutputReady("totalIter",1);

                    // Другой старт:

/*                        word32_t buf1[32];
                        readMemBlkRT(0x140, buf1, 32);
                        word32_t buf2[32];
                        readMemBlkRT(buf1[2], buf2, 32);
                        stdOutput(tr("BufRT addr=%1: %2 %3 %4 %5 %6 %7 %8 %9").arg(buf1[2], 4, 16, QLatin1Char('0')).arg(buf2[0], 4, 16, QLatin1Char('0')).
                                arg(buf2[1], 4, 16, QLatin1Char('0')).arg(buf2[2], 4, 16, QLatin1Char('0')).arg(buf2[3], 4, 16, QLatin1Char('0')).arg(buf2[4], 4, 16, QLatin1Char('0'))
                                .arg(buf2[5], 4, 16, QLatin1Char('0')).arg(buf2[6], 4, 16, QLatin1Char('0')).arg(buf2[7], 4, 16, QLatin1Char('0')));
                        readMemBlkRT(buf1[4], buf2, 32);
                        stdOutput(tr("BufRT addr=%1: %2 %3 %4 %5 %6 %7 %8 %9").arg(buf1[4], 4, 16, QLatin1Char('0')).arg(buf2[0], 4, 16, QLatin1Char('0')).
                                arg(buf2[1], 4, 16, QLatin1Char('0')).arg(buf2[2], 4, 16, QLatin1Char('0')).arg(buf2[3], 4, 16, QLatin1Char('0')).arg(buf2[4], 4, 16, QLatin1Char('0'))
                                .arg(buf2[5], 4, 16, QLatin1Char('0')).arg(buf2[6], 4, 16, QLatin1Char('0')).arg(buf2[7], 4, 16, QLatin1Char('0')));
*/

                    // Проверки
                    if (checkRW || checkRTBC)
                    {
                        curErr = false;
                        thread()->msleep(num_messages);
                        for (int i=0; i<num_messages; i++)
                        {
                            if (!checkMessage(messages[i]))
                            {
                                errors ++;
                                curErr = true;
                               //if (errors < 30)
                                stdOutput(tr("Ошибка передачи сообщения. Шаг %1, %2 ошибок.").arg(counter).arg(errors));
                                emit statsOutputReady("totalErr",1);
                            }
                        }
                        if (impulse)
                        {
                            if (curErr)
                            {
                                QString str1 = QString(tr("Сбой")) +'\t' + QString::number(counter) + '\n' +
                                        QDateTime::currentDateTime().toString(tr("dd.MM.yyyy hh:mm:ss"));
                                emit logOutputReady(str1);
                                if (prevErr)
                                {
                                    //stdOutput(tr("Фатальная ошибка. Тест прерван."));
                                    fatal = true;
                                    break;
                                    /*
                                    stdOutput(tr("Тест завершён с ошибками."));
                                    emit resultReady((int)AbstractTest::TestFault);
                                    return;
                                    */
                                }
                            }
                            prevErr = curErr;
                        }
                        if (pause > num_messages)
                            thread()->msleep(pause - num_messages);
                    }
                    else if (pause > 0)
                        thread()->msleep(pause);
                } // конец цикла по сообщениям
                if (check_time)
                {
                    /*
                    time_t timer = time(NULL);
                    char buftime[128];
                    ctime_s(buftime, 128, &timer);
                    stdOutput(QString("Время завершения теста: ")+QString(buftime));
                    */
                    QDateTime local(QDateTime::currentDateTime());
                    stdOutput(QString("Время завершения цикла: ") + local.toString(tr("dd.MM.yyyy hh:mm:ss")));
                    stdOutput(QString("Пройдено %1 итераций").arg(counter));
                }
            }  // if (Count)
            else
            {
                stdOutput("Пустой цикл.");
            }

            if (stopped)
            {
                QString resume("Тест остановлен. ");
                if (errors)
                {
                    if (impulse && !fatal)
                         resume += tr("Обнаружены устранимые ошибки.");
                    else
                         resume += tr("Обнаружены ошибки.");
                }
                else
                    resume += tr("Ошибок не обнаружено.");
                stdOutput(resume);
                emit resultReady((int)AbstractTest::Stopped);
            }
            else if (errors)
            {
                if (impulse && !fatal)
                {
                    stdOutput(tr("Тест завершён. Обнаружены устранимые ошибки."));
                    emit resultReady((int)AbstractTest::Completed);
                }
                else
                {
                    stdOutput(tr("Тест завершён с ошибками."));
                    emit resultReady((int)AbstractTest::TestFault);
                }
            }
            else
            {
                stdOutput("Тест завершён успешно.");
                emit resultReady((int)AbstractTest::Completed);
            }
        } // startBC
        else
        {
            stdOutput("Тест завершён успешно.");
            emit resultReady((int)AbstractTest::Completed);
        }
    }
    catch(const QString& exception)
    {
        stdOutput(tr("Error: ") + exception);
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}

bool lscMessageObjToThread::configRT(word32_t initCommandStackRT)
{
    // конфигурация ОУ НСК

    switchToDev(busRT);  // переключение на ОУ
    CWriteDeviceMemory bufRT;
    bufRT.addReg(mr_NUMREG_COMMAND, 1);      // сброс
    bufRT.writeDev(dev, true);

    fillMemBlkLSC(0x100, 0x300, 0);      // 0x100 - 0x400
    fillMemBlkLSC(0x140, 0x60, bufDataRT);   // буферы приёма и передачи для всех подадресов инициализированы одинаковым значением
    fillMemBlkLSC(0x1C0, 0x60, bufDataRT);
    fillMemBlkLSC(bufDataRT, 0x800-bufDataRT, 0xCC);

    //writeRegInitRT();
    // Инициализация буферов приёма и передачи.
    // Часть буферов объединяются, потому что памяти всего 2К
    word32_t pBufRec = (stackRT == 0 ? 0x140 : 0x1C0);
    for (int i=1; i<31; i++)
    {
        bufRT.addRamLSC(pBufRec+i, bufDataRT+i*32);
        bufRT.addRamLSC(pBufRec+32+i, bufDataRT+i*32);

        sa_buf_rec[i] = bufDataRT+i*32;
        sa_buf_trm[i] = bufDataRT+i*32;
        sa_buf_brd[i] = bufDataRT+i*32;

    }
    bufRT.addRamLSC(fixRT, initCommandStackRT);

    bufRT.addReg(mr_NUMREG_CONFIG_5, rtaddr<<1);
    bufRT.addReg(mr_NUMREG_CONFIG_3, 0x8000);
    bufRT.addReg(mr_NUMREG_CONFIG_3, 0x8080| (mr_REG_CFG1_CURR_AREA_FLAG*stackRT));
    bufRT.addReg(mr_NUMREG_CONFIG_2, 0x8010); // двойная буферизация приёма и отделение группового приёма выключены из-за нехватки памяти
    bufRT.addReg(mr_NUMREG_EXT_RTA, rtaddr);
    bufRT.addReg(mr_NUMREG_CONFIG_1, 0x8F80 );

    bufRT.writeDev(dev, true);

    return true;
}

bool lscMessageObjToThread::configBC()
{
    // конфигурация регистров
    switchToDev(busBC);    // переключение на КШ

    CWriteDeviceMemory bufBC;
    bufBC.addReg(mr_NUMREG_COMMAND, 1);       // сброс
    bufBC.addReg(mr_NUMREG_CONFIG_3, 0x8000); // расширенный режим конфигурирования
    bufBC.addReg(mr_NUMREG_CONFIG_5, 0x600);  // максимальный response timeout
    bufBC.addReg(mr_NUMREG_CONFIG_2, 0x400);  // запрещение 256-словных ограничений
    if (autorepeat)
    {
            bufBC.addReg(mr_NUMREG_CONFIG_1, 0x0160 | (mr_REG_CFG1_CURR_AREA_FLAG*stackBC));
            bufBC.addReg(mr_NUMREG_BCFTIME_RTCW_WWMT, ft);
    }
    else
            bufBC.addReg(mr_NUMREG_CONFIG_1, 0x0060 | (mr_REG_CFG1_CURR_AREA_FLAG*stackBC));

    bufBC.writeDev(dev, true);

    return true;
}

void lscMessageObjToThread::switchToDev(uint num)
{
    writeRegSingle(mr_NUMREG_EXT_BUS, num);
}

bool lscMessageObjToThread::createMessages(std::vector<C_LSC_Format*>& messages)
{
    for (int n=0; n<num_messages; n++)
    {
        if (mFormat == "Приём данных")
        {
            messages.push_back(new C_LSC_Format1(torta, sa, nw, dataWord, rtaddr==torta, bus));
        }
        else
        {
            //messages.push_back(new C_LSC_Format2());
            //messages.push_back(new C_LSC_Format3());
            //messages.push_back(new C_LSC_Format4());
            //messages.push_back(new C_LSC_Format5());
            //messages.push_back(new C_LSC_Format6());
            //messages.push_back(new C_LSC_Format7());
            //messages.push_back(new C_LSC_Format8());
            //messages.push_back(new C_LSC_Format9());
            //messages.push_back(new C_LSC_Format10());
        }
    }

    num_messages = 0;

    for (unsigned i=0; i<messages.size(); i++)
        if (!addMessage(messages[i]))
            return false;

    return true;
}

bool lscMessageObjToThread::isTestRunning(int st)
{
    return st == AbstractTest::Running || st == AbstractTest::Paused;
}

void lscMessageObjToThread::terminate(int st)
{
    if (!isTestRunning(st))
    {
        if (started)
        {
            writeRegSingle(mr_NUMREG_COMMAND, 0x20);
            started = false;
        }
        //stdOutput(tr("Terminate: before tryToDisconnect()  (DEBUGGING)"));
        dev->tryToDisconnect();
    }
    emit setResultReady(st);
}
/*
void lscMessageObjToThread::writeRegInitBC()
{
    regBC.clear();
}

void lscMessageObjToThread::writeRegAddBC(word32_t num, word32_t val)
{
    int addr = num*4;
    regBC.append((char*)&addr, 4);
    regBC.append((char*)&val, 4);
}

void lscMessageObjToThread::writeRegistersBC()
{
    devBC->write_F1(regBC.data(), regBC.size());
}

void lscMessageObjToThread::writeRegInitRT()
{
    regRT.clear();
}

void lscMessageObjToThread::writeRegAddRT(word32_t num, word32_t val)
{
    word32_t addr = num*4;
    regRT.append((char*)&addr, 4);
    regRT.append((char*)&val, 4);
    //char* data = regRT.data();
}

void lscMessageObjToThread::writeRegistersRT()
{
    devRT->write_F1(regRT.data(), regRT.size());
}
*/
bool lscMessageObjToThread::fillMemBlkLSC(int dst_addr, int num_words16, word16_t value)
{
    char* buf = (char*)malloc(num_words16 * 4);
    if (buf == 0)
        return false;
    word32_t* tmp = (word32_t*)buf;
    for (int i=0; i<num_words16; i++)
        tmp[i] = value;
    dev->write_F2(0x4000 + dst_addr*4, buf, num_words16 * 4);
    free (buf);
    return true;
}

void lscMessageObjToThread::writeRegSingle(word32_t num, word32_t value)
{
    dev->write_F2(num*4, (char*)(&value), 4);
}

bool lscMessageObjToThread::addMessage(C_LSC_Format* format)
{
    if (num_messages >= max_num_messages || format == 0)
        return false;

    format->setDevice(dev);
    format->setSABufs(sa_buf_rec, sa_buf_trm, sa_buf_brd);
    //format->setBus(bus);
    format->addMessage(curCommandStackRT, curCommandStackBC, ptrMessagesBC);
    if (checkRTBC)
        format->spoilDataRT(busRT, busBC);

    num_messages++;

    return true;
}
/*
void lscMessageObjToThread::startMessages()
{
    CWriteDeviceMemory bufBC;
    bufBC.addRamLSC(fixBC, initCommandStackBC);
    bufBC.addRamLSC(fixBC+1, 0xFFFF-num_messages);
    bufBC.addReg(mr_NUMREG_COMMAND, 2);
    bufBC.writeDev(devBC, true);
}
*/
void lscMessageObjToThread::initStartMessages(CWriteDeviceMemory &bufBC)
{
    bufBC.addRamLSC(fixBC, initCommandStackBC);
    bufBC.addRamLSC(fixBC+1, 0xFFFF-num_messages);
    if (autorepeat)
    {
        bufBC.addRamLSC(fixBC+2, initCommandStackBC);
        bufBC.addRamLSC(fixBC+3, 0xFFFF-num_messages);
    }
    bufBC.addReg(mr_NUMREG_COMMAND, 2);
}

void lscMessageObjToThread::startMessages(CWriteDeviceMemory &bufBC)
{
//    setDeviceToBC();
    bufBC.writeDev(dev, false);
}
/*
void lscMessageObjToThread::setDeviceToBC()
{
    if (subDevBC == m_curSubDev)
        return;
    dev->reg_aux_bus_select.bus = subDevBC.toUInt();
    dev->writeReg(&dev->reg_aux_bus_select);
    m_curSubDev = subDevBC;
}

void lscMessageObjToThread::setDeviceToRT()
{
    if (subDevRT == m_curSubDev)
        return;
    dev->reg_aux_bus_select.bus = subDevRT.toUInt();
    dev->writeReg(&dev->reg_aux_bus_select);
    m_curSubDev = subDevRT;
}
*/
