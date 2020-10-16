#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>
#include "inputdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    prjLoaded(false),
    //tstLoaded(false),
    su(false),
    cff(0),
    progrMode(0),
    logFileDefault("log.txt"),
    curIndex(0)
{
    ui->setupUi(this);
    ui->actHelp->setVisible(false);

    connect(ui->actRun, SIGNAL(triggered(bool)), this, SLOT(prepare()));
    connect(ui->actRun, SIGNAL(triggered(bool)), this, SLOT(onRunTst()));
    connect(ui->actPause, SIGNAL(triggered(bool)), this, SLOT(onPause()));
    connect(ui->actStop, SIGNAL(triggered(bool)), this, SLOT(onStop()));
    connect(ui->actCancellation, SIGNAL(triggered(bool)), this, SLOT(onCancellation()));
    //connect(ui->actStop, SIGNAL(triggered(bool)), this, SLOT(onStopMonitor()));
    connect(ui->actSettings, SIGNAL(triggered(bool)), &settings, SLOT(show()));

    ui->outputMenu->addAction(ui->dockPro->toggleViewAction());
    ui->outputMenu->addAction(ui->dockTest->toggleViewAction());

    //setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint);

    act_devMode1 = new QAction(tr("Включить основной режим"), 0);
    act_devMode1->setObjectName(QStringLiteral("act_devMode1"));
    addAction(act_devMode1);
    act_devMode1->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0));
    connect(act_devMode1, SIGNAL(triggered()), this, SLOT(actDevMode()));
    act_devMode2 = new QAction(tr("Включить основной режим"), 0);
    act_devMode2->setObjectName(QStringLiteral("act_devMode2"));
    addAction(act_devMode2);
    act_devMode2->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+D", 0));
    connect(act_devMode2, SIGNAL(triggered()), this, SLOT(actDevMode()));
    act_programmerMode = new QAction(tr("Включить секретный режим"), 0);
    act_programmerMode->setObjectName(QStringLiteral("act_devMode2"));
    addAction(act_programmerMode);
    act_programmerMode->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+P", 0));
    connect(act_programmerMode, SIGNAL(triggered()), this, SLOT(actProgrMode()));

    ui->actConfiguration->setVisible(false);
    ui->actConfiguration->setEnabled(false);

    QSettings project("../tgca/project.ini", QSettings::IniFormat);
    if (project.status() == QSettings::NoError && project.allKeys().size() >= 2 &&
            project.value("Common/autoload").toString() == "1")
        prjLoaded = loadProject(project);

    if (prjLoaded)
        ui->actSavePrj->setEnabled(true);
    else
        actDevMode();

    connect(ui->actAddDevice, &QAction::triggered, &deviceSelection, &QWidget::show);
    connect(&deviceSelection, &DeviceSelectionForm::addDevice, this, &MainWindow::addNewDevice);

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, [=] () {
        setTimerState(0);
        if (settings.getPauseChecked())
            ui->actRun->triggered();
    });

    treeState = stopped;
}

MainWindow::~MainWindow()
{
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~MainWindow() start";
#endif

    clearProject();

    delete ui;
#ifdef PRINT_START_END_DESTRUCTOR
    qDebug() << "~MainWindow() end";
#endif
}

QString MainWindow::version()
{
#ifdef QT_DEBUG
    QString configuration("Debug");
#else
    QString configuration("Release");
#endif
    QString myText("---");
    QFile file(":/version");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        //QTextStream in(&file);
        myText = QTextStream(&file).readAll();
        file.close();
    }
    return tr("%1 %2").arg(configuration).arg(myText);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("Справка"), tr("Шифр: \"Обработка 30\"\nВерсия: %1").arg(version()));
}

bool MainWindow::clearProject()
{
    bool bRet = true;
  /*  if (su)
    {
        su = false;
        ui->actConfiguration->setVisible(false);
        ui->actConfiguration->setEnabled(false);
        if (devConf)
            delete devConf;
        devConf = 0;
    }*/    //LLL!!!
    cff = 0;

    int sz = ui->tests->count();
    for (int i=sz-1; i>=0; i--)
    {
        AbstractTest *test = (AbstractTest*)ui->tests->itemAt(i)->widget();
        /// закрыть тест, выгрузить из tests
        delete test;
    }

    sz = ui->devices->count();
    for (int i=sz-1; i>=0; i--)
    {
        BaseDevice *dev = (BaseDevice*)ui->devices->itemAt(i)->widget();
        /// Здесь надо разорвать все соединения и удалить устройство
        delete dev;
    }
    return bRet;
}
/*
static bool setupConnection(Device *device, QString ip, QString port, QString hostIp, QString hostPort)
{
    device->connection.setServerIP(ip);
    device->connection.setServerPORT(port);
    device->connection.setHostIP(hostIp);
    device->connection.setHostPORT(hostPort);
    return !ip.isEmpty() && !port.isEmpty() && !hostIp.isEmpty();
}
*/
bool MainWindow::loadProject(QSettings& settings)
{
    su = false;
    if (settings.value("Common/user").toString() == "su")
        actDevMode();
    this->settings.setLogFileName(settings.value("Common/output").toString());
    cff = settings.value("Common/cff").toInt();
    if (progrMode == 3)
        cff = 1;
    this->settings.setPauseTime(settings.value("Common/pauseTime").toString());
    this->settings.setPauseChecked(settings.value("Common/pauseChecked").toBool());

    int sze = settings.beginReadArray("Devices");
    for (int i = 0; i < sze; ++i) {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        //TODO
        if (!name.isEmpty())
        {
            Device *device = new Device(this, name, ui->projectBrowser);
            ui->devices->addWidget(device);
            LSCTable::addDevice(name);
            connect(device, SIGNAL(sigDelete(QString)), this, SLOT(delDevice(QString)), Qt::DirectConnection);
            emit newDev(name);
            if (!device->connection.setFromFile(settings.value("connections").toString()))
            {
                device->connection.setServerIP(settings.value("IP").toString());
                device->connection.setServerPORT(settings.value("port").toString());
                device->connection.setHostIP(settings.value("hostIP").toString());
            }
        }
    }
    settings.endArray();

    sze = settings.beginReadArray("Tests");
    for (int i = 0; i < sze; ++i) {
        settings.setArrayIndex(i);

        QString name = settings.value("test").toString();
        if (!name.isEmpty())
        {
            AbstractTest *at = testLib::loadTest(name, ui->devices, ui->projectBrowser, ui->testBrowser);
            if (at)
            {
                loadTest(at);
                QString ena = settings.value("enabled").toString();
                if (!ena.isEmpty())
                {
                    at->setEnable(ena != "0");
                    //tstLoaded = true;
                }
            }
        }
    }
    settings.endArray();

    message(QString("Проект %1 загружен").arg(settings.fileName()), QString("tests"));
    return true;
}

void MainWindow::onLoadPrj()
{
    if (getGlobalState() == BUSY) {
        message("Тесты запущены. Для загрузки проекта остановите тесты.");
        return;
    }
    if (timer.isActive()) {
        message(tr("Предупреждение: работает таймер цикла, для загрузки проекта остановите таймер кнопкой \"Стоп\"."));
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Загрузить проект из файла"), "/home/", tr("*.ini"));

    if (!fileName.isEmpty())
    {
        if (clearProject())
        {
            QSettings project(fileName, QSettings::IniFormat);
            prjLoaded = (project.status() == QSettings::NoError && project.allKeys().size() >= 2 && loadProject(project));
        }
    }
}

bool MainWindow::onSavePrj()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить в файл"), "");

    if (fileName == "")
        return false;

    QSettings ini(fileName, QSettings::IniFormat);

    ini.clear();

    ini.setValue("Common/output", this->settings.getLogFileName());
    ini.setValue("Common/autoload", "0");
    ini.setValue("Common/user", su ? "su" : "");
    if (cff>1)
        ini.setValue("Common/cff", tr("%1").arg(cff));
    ini.setValue("Common/pauseTime", this->settings.getPauseTime());
    ini.setValue("Common/pauseChecked", this->settings.getPauseChecked());

    ini.beginWriteArray("Devices");
    //TODO
    for (int j=0; j<ui->devices->count(); j++)
    {
        Device *dev = (Device*)ui->devices->itemAt(j)->widget();

        ini.setArrayIndex(j);
        ini.setValue("name", dev->getName());
        QString fname = dev->connection.getFileName();
        if (fname.isEmpty())
        {
            ini.setValue("IP", dev->connection.getServerIP());
            ini.setValue("port", dev->connection.getServerPORT());
            ini.setValue("hostIP", dev->connection.getHostIP());
        }
        else
            ini.setValue("connections", fname);
    }
    ini.endArray();


    ini.beginWriteArray("Tests");
    for (int i=0; i<ui->tests->count(); i++)
    {
        AbstractTest * test = (AbstractTest*)ui->tests->itemAt(i)->widget();
        ini.setArrayIndex(i);
        ini.setValue("test", test->getName());
        ini.setValue("enabled", test->getEnable() ? "1" : "0");
    }
    ini.endArray();

    message(QString("Проект %1 сохранен").arg(fileName));
    return true;
}

void MainWindow::prepare()
{
    if (getGlobalState() == BUSY) return;
    if (timer.isActive()) {
        message(tr("Предупреждение: работает таймер цикла, тесты не могут быть запущены"));
        return;
    }
    if (getTreeState() == running) {
        return;
    } else {
        if(AbstractTest::getBeginTest() != NULL)
            curIndex = ui->tests->indexOf(AbstractTest::getBeginTest());
        prepareFirstStart=1;
        logFile.setFileName(settings.getLogFileName());
        if (!logFile.open(QIODevice::Append)) {
            logFile.setFileName(logFileDefault);
            logFile.open(QIODevice::Append);
        }
        if (logFile.isOpen()) {
            QDateTime local(QDateTime::currentDateTime());
            logStream.setDevice(&logFile);
            logStream << local.toString(tr("dd.MM.yyyy hh:mm:ss"));
        }
    }
}

void MainWindow::onCancellation()
{
    if (getGlobalState() == BUSY) {
        message(tr("Предупреждение: для сброса необходима остановка тестов"));
        return;
    }
    if (timer.isActive()) {
        message(tr("Предупреждение: работает таймер цикла, для сброса остановите таймер кнопкой \"Стоп\"."));
        return;
    }
    if (getTreeState() == running) {
        message(tr("Предупреждение: для сброса необходима остановка тестов"));
        return;
    } else {
        int n = ui->tests->count();
        for ( int i = 0; i<n; i++)
            ((AbstractTest*)ui->tests->itemAt(i)->widget())->cancellation();
        message(tr("Результаты и статистики тестов сброшены"));
    }
}

void MainWindow::addNewDevice(QString name, int type)
{
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Добавить устройство"), tr("Не задано имя устройства"));
        return;
    }
    if (name.at(name.size()-1) == QChar('_')) {
        QMessageBox::information(this, tr("Добавить устройство"), tr("Запрещено использовать символ \'_\' в конце имени устройства"));
        return;
    }
    BaseDevice* dev;
    for (int i=0; i<ui->devices->count(); i++) {
        dev = (BaseDevice*)ui->devices->itemAt(i)->widget();
        if (dev->getName() == name) {
            QMessageBox::information(this, tr("Добавить устройство"), tr("Ошибка: устройство %1 уже существует").arg(name));
            return;
        }
    }

    switch (type) {
    case DeviceTypes::terasic:
        dev = new Device(this, name, ui->projectBrowser);   //TODO
        break;
    case DeviceTypes::switch411:
        dev = new Device(this, name, ui->projectBrowser);   //TODO
        break;
    }
    ui->devices->addWidget(dev);
    //ui->projectBrowser->append(tr("Устройство %1 добавлено").arg(name));
    LSCTable::addDevice(name);
    connect(dev, SIGNAL(sigDelete(QString)), this, SLOT(delDevice(QString)), Qt::DirectConnection);
    emit newDev(name);
}

void MainWindow::delDevice(QString name)
{
    LSCTable::delDevice(name);
}

void MainWindow::loadTest(AbstractTest* test)
{
    test->setParent(this);
    test->setUserLevel(su, progrMode);
    connect(act_devMode1, SIGNAL(triggered()), test, SLOT(actDevMode()));
    connect(act_devMode2, SIGNAL(triggered()), test, SLOT(actDevMode()));
    connect(this, SIGNAL(set_programmerMode(int)), test, SLOT(actProgrMode(int)));
    connect(this, SIGNAL(newDev(QString)), test, SLOT(newDev(QString)));
    connect(this, SIGNAL(setTestStateIcon(int)), test, SLOT(setRunningState(int)));
    connect(test, SIGNAL(setEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(setSlot(QPushButton*,QPushButton*,QPushButton*)));
    connect(test, SIGNAL(unsetEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(unsetSlot(QPushButton*,QPushButton*,QPushButton*)));
    connect(test, SIGNAL(applyToAll(QString&,QString&,QString&,QString&)), this, SLOT(applyToAllTests(QString&,QString&,QString&,QString&)));
    connect(test, SIGNAL(dragged()), this, SLOT(onDragged()));
    connect(test, SIGNAL(dropped()), this, SLOT(onDropped()));
    connect(test, SIGNAL(logOutputReady(QString)), this, SLOT(writeLog(QString)));
    ui->tests->addWidget(test);
    test->addAction(act_devMode1);
    test->addAction(act_devMode2);
    test->connectActions();
}

void MainWindow::applyToAllTests(QString& , QString& classType, QString& fieldName, QString& value)
{
    for (int i=0; i<ui->tests->count(); i++)
    {
        AbstractTest *test = (AbstractTest*)ui->tests->itemAt(i)->widget();
        //if (test->testType() == testType)
        {
            QWidget* wid;
            if (classType == "QCheckBox")
            {
               if (wid = test->findField<QCheckBox>(fieldName)) ((QCheckBox*)wid)->setChecked(!value.isEmpty());
            }
            else if (classType == "QLineEdit")
            {
                if (wid = test->findField<QLineEdit>(fieldName)) ((QLineEdit*)wid)->setText(value.isEmpty() ? tr("0") : value);
            }
            /*else if (classType == "ComboBoxNoScroll")
            {
                if (wid = test->findField<ComboBoxNoScroll>(fieldName)) ((ComboBoxNoScroll*)wid)->setCurrentText(value);
            }*/
            else if (classType == "QComboBox")
            {
                if (wid = test->findField<QComboBox>(fieldName)) ((QComboBox*)wid)->setCurrentText(value);
            }
        }
    }
}

void MainWindow::openTest()
{
    QString txtFile = QFileDialog::getOpenFileName(0, tr("Открыть файл параметров теста"), tr(""));
    if (txtFile.isEmpty()) return;
    AbstractTest* test = testLib::loadTest(txtFile, ui->devices, ui->projectBrowser, ui->testBrowser);
    if(test != NULL)
        loadTest(test);
}

void MainWindow::writeLog(QString m)
{
    if (logFile.isOpen()) {
        logStream << '\t' << m;
    }
}

void MainWindow::createTest()
{
    AbstractTest* test = testLib::createTest(ui->devices, ui->projectBrowser, ui->testBrowser, su);
    if(test != NULL)
        loadTest(test);
}

void MainWindow::actDevMode()
{
    if (!su)
    {
        su = true;
        //ui->actConfiguration->setVisible(true);
        //ui->actConfiguration->setEnabled(true);
        //devConf = new Configuration();
    }
}

void MainWindow::actProgrMode()
{
    progrMode = (progrMode+1)%4;  // 0 - standard mode, 1 - debug uotput, 2 - debug mode + special features, 3 - correct package
    qDebug() << "mode changed: " << progrMode;
    emit set_programmerMode(progrMode);
}

void MainWindow::onDragged()
{
    dragIndex = ui->tests->indexOf((AbstractTest*)sender());
}

void MainWindow::onDropped()
{
    dropIndex = ui->tests->indexOf((AbstractTest*)sender());
    if (dragIndex == dropIndex) return;
    QLayoutItem* tempTest = ui->tests->takeAt(dragIndex);
    ui->tests->insertItem(dropIndex, tempTest);
}

void MainWindow::onMenuDevices(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Добавить устройство"));
    connect(act, SIGNAL(triggered()), this, SLOT(addNewDevice()));

    menu.exec(ui->labeDevicesTitle->mapToGlobal(point));
}

void MainWindow::onMenuTests(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Добавить новый тест"));
    connect(act, SIGNAL(triggered()), this, SLOT(createTest()));
    act = menu.addAction(tr("Добавить существующий тест"));
    connect(act, SIGNAL(triggered()), this, SLOT(openTest()));

    menu.exec(ui->labeDevicesTitle->mapToGlobal(point));
}

void MainWindow::onMenuOutput(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Очистить"));
    connect(act, SIGNAL(triggered()), this, SLOT(outputClear()));
    if (sender()->objectName() == "projectBrowser") {
        act->setObjectName("pB");
        menu.exec(ui->projectBrowser->mapToGlobal(point));
    } else if (sender()->objectName() == "testBrowser") {
        act->setObjectName("tB");
        menu.exec(ui->testBrowser->mapToGlobal(point));
    }
}

void MainWindow::outputClear()
{
    if (sender()->objectName() == "pB") {
        ui->projectBrowser->clear();
    } else if (sender()->objectName() == "tB") {
        ui->testBrowser->clear();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Escape)
        QMainWindow::keyPressEvent(e);
}

void MainWindow::setTreeState(MainWindow::TreeState s)
{
    if (s == running) {
        if (prepareFirstStart == 1) message(tr("Тесты запущены"));
    } else if (s == stopped) {
        curIndex = 0;
        message(tr("Тесты остановлены"));
    } else if (s == finished) {
        curIndex = 0;
        message(tr("Тесты завершены"));
    } else if (s == next) {
        curIndex++;
        prepareFirstStart++;
    } else if (s == bigStop) {
        if (treeState!=running) {
            setTreeState(smallStop);
            return;
        }
    } else if (s == smallStop) {

    }
    treeState = s;
}

MainWindow::TreeState MainWindow::getTreeState() const
{
    return treeState;
}

void MainWindow::message(QString m, QString browser)
{
    QDateTime local(QDateTime::currentDateTime());
    if (browser == "project")
        ui->projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + m);
    else if (browser == "tests")
        ui->testBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + m);
}

void MainWindow::run(int index)
{
    if (AbstractTest::getEndTest() == NULL && index >= ui->tests->count()) {
        setTreeState(finished);
        if (settings.getPauseChecked()) {
            QStringList MMSSmmm = settings.getPauseTime().split(":");
            timer.setInterval(MMSSmmm.at(0).toInt()*60000 + MMSSmmm.at(1).toInt()*1000. + MMSSmmm.at(2).toInt());
            timer.start();
            setTimerState(1);
            message(tr("Таймер цикла запущен"));
        }
        if (logFile.isOpen()) {
            logStream << endl;
            logFile.close();
        }
        return;
    } else if (AbstractTest::getEndTest() != NULL
               && index-1 == ui->tests->indexOf(AbstractTest::getEndTest())
               && prepareFirstStart!=1) {
        setTreeState(finished);
        if (settings.getPauseChecked()) {
            QStringList MMSSmmm = settings.getPauseTime().split(":");
            timer.setInterval(MMSSmmm.at(0).toInt()*60000 + MMSSmmm.at(1).toInt()*1000. + MMSSmmm.at(2).toInt());
            timer.start();
            setTimerState(1);
            message(tr("Таймер цикла запущен"));
        }
        if (logFile.isOpen()) {
            logStream << endl;
            logFile.close();
        }
        return;
    } else if (AbstractTest::getEndTest() != NULL && index >= ui->tests->count()) {
        curIndex=0; index=0;
    }

//    if (index >= ui->tests->count()) {
//        setTreeState(finished);
//        return;
//    }

    AbstractTest* test = (AbstractTest* )ui->tests->itemAt(index)->widget();
    if (test->isReady()) {
        emit test->globalStart();
    } else {
        if (logFile.isOpen()) {
            logStream << '\t';
        }
        setTreeState(next);
        onRunTst();
    }
}

void MainWindow::onRunTst()
{
    if (getGlobalState() == BUSY) return;
    if (timer.isActive())   return;
    if (getTreeState() == running) {
        return;
    } else {
        if (prepareFirstStart == 1)
            emit setTestStateIcon((int)AbstractTest::Stopped);
        setTreeState(running);
        run(curIndex);
    }
}

void MainWindow::onPause()
{
    if (getGlobalState() == BUSY) return;
    if (getTreeState() != running)
        message(tr("Тесты не запущены"));
}

void MainWindow::onStop()
{
    if (getGlobalState() == BUSY) {
        setTreeState(bigStop);
        if (logFile.isOpen()) {
            logStream << tr("\tОстан");
            logStream << endl;
            logFile.close();
        }
        return;
    }
    if (getTreeState() != running)
        message(tr("Тесты не запущены"));
    if (timer.isActive()) {
        message(tr("Таймер цикла остановлен"));
        timer.stop();
        setTimerState(0);
    }
    if (logFile.isOpen()) {
        logStream << endl;
        logFile.close();
    }
}

void MainWindow::setSlot(QPushButton *start, QPushButton *pause, QPushButton *stop)
{
    //qDebug() << "setSlots";
    connect(ui->actRun, SIGNAL(triggered(bool)), start, SIGNAL(clicked(bool)));
    connect(ui->actPause, SIGNAL(triggered(bool)), pause, SIGNAL(clicked(bool)));
    connect(ui->actStop, SIGNAL(triggered(bool)), stop, SIGNAL(clicked(bool)));
}

void MainWindow::unsetSlot(QPushButton *start, QPushButton *pause, QPushButton *stop)
{
    //qDebug() << "unsetSlots";
    disconnect(ui->actRun, SIGNAL(triggered(bool)), start, SIGNAL(clicked(bool)));
    disconnect(ui->actPause, SIGNAL(triggered(bool)), pause, SIGNAL(clicked(bool)));
    disconnect(ui->actStop, SIGNAL(triggered(bool)), stop, SIGNAL(clicked(bool)));
    if (getTreeState() == running) {
        curIndex = ui->tests->indexOf((AbstractTest*)sender());
        //qDebug() << "curIndex: " << curIndex;
        setTreeState(next);
        onRunTst();
    } else if (getTreeState() == bigStop) {
        setTreeState(stopped);
    } else if (getTreeState() == smallStop) {
        treeState = stopped;
    }
}

void MainWindow::setReport(QTextDocument& doc)
{
    InputDialog inputDialog;
    inputDialog.setWindowFlags(inputDialog.windowFlags() & (~Qt::WindowContextHelpButtonHint));
    inputDialog.exec();
    QString str = inputDialog.getStr();
    QDateTime local(QDateTime::currentDateTime());
/*
    QString str2=QInputDialog::getText( 0,
                                         "Отчёт",
                                         "Введите номер или название образца:",
                                         QLineEdit::Normal,
                                         "",
                                         &bOk,
                                       inputDialog.windowFlags()
                                        );
*/
    QString title = QString("<br><br><br><big><b><center>\
                            <br>ПРИЛОЖЕНИЕ К ПРОТОКОЛУ ИСПЫТАНИЙ<br>МИКРОСХЕМЫ ИНТЕГРАЛЬНОЙ 1888BC034<br><br>\
                            <br>( ФУНКЦИОНАЛЬНЫЙ КОНТРОЛЬ ФК2 )<br><br>\
                            <br>Номер образца:   %1</b></big><br>\
                                                         <br>\
                                                         <br>\
                                                         <br>\
                                                         <br><center>Дата выпуска отчёта: <b>%2</b><br></center>\
                                                         <center><br>Версия тестового ПО: <b>TGCA %3<br></center>")
            .arg(str).arg(local.toString(QString("dd.MM.yyyy"))).arg(version());


    QString header2 = QString("</p>\n<br style = 'page-break-after: always;'><html>\n<body>\n<p><br><br><br><br><big><b><center>ОКНО ТЕСТОВ<br></center></b><br><br><br>");
    QString page2 = ui->testBrowser->toHtml();
    if (progrMode == 2)
    {
        QString header1 = QString("</p>\n<br style = 'page-break-after: always;'><html>\n<p><br><br><br><br><big><b><center>ОКНО ПРОЕКТА<br></center></b><br><br><br>");
        QString page1 = ui->projectBrowser->toHtml();
        doc.setHtml(title+header1+page1+header2+page2);
    }
    else
        doc.setHtml(title+header2+page2);

    // QString("<font size=\"1\">")
}

void MainWindow::printReport(QPrinter* prnt)
{
    QTextDocument document;
    setReport(document);
    document.print(prnt);
}

void MainWindow::onPrintRep()
{
    QPrinter prnt;
    QPrintDialog* dialog = new QPrintDialog(&prnt, this);
    connect(dialog, SIGNAL(accepted(QPrinter*)), this, SLOT(printReport(QPrinter*)));
    dialog->exec();
    disconnect(dialog, SIGNAL(accepted(QPrinter*)), this, SLOT(printReport(QPrinter*)));
}

bool MainWindow::onCreateRep()
{
    QTextDocument document;
    setReport(document);

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить в файл"), "", tr("Файлы pdf (*.pdf)\nВсе файлы (*.*)"));

    if (fileName == "")
        return false;

    QPrinter prnt;
    prnt.setPageSize(QPrinter::A4);
    prnt.setOutputFormat(QPrinter::PdfFormat);
    prnt.setOutputFileName(fileName);
    document.print(&prnt);

    return true;
}

void MainWindow::onHelp()
{
    qDebug() << QObject::tr("Slot onHelp() is not implemented !!!");
}

