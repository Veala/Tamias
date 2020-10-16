#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QComboBox>
#include <QSpinBox>
#include <QtUiTools>
//#include <QFormBuilder>
#include <QtNetwork>
#include <QPushButton>
#include <QMessageBox>
#include <QDrag>
#include "../funclib.h"
#include "../device.h"
#include "../data_structs.h"
#include "../animatedspinnericon.h"
#include "../globalstate.h"
#include "../comboboxnoscroll.h"

//#define debug_AT

class NewUiLoader : public QUiLoader {
public:
    NewUiLoader(QObject * parent = 0) : QUiLoader(parent)
    {

    }
    virtual QWidget * createWidget(const QString &className, QWidget *parent = 0, const QString &name = "")
    {
        if (className == "ComboBoxNoScroll") {
            ComboBoxNoScroll * w = new ComboBoxNoScroll(parent);
            w->setObjectName(name);
            return w;
        }
        return QUiLoader::createWidget(className, parent, name);
    }
    virtual ~NewUiLoader()
    {

    }
};

class WheelFilter : public QObject
{
    Q_OBJECT
public:
    explicit WheelFilter(QObject *parent = 0) : QObject(parent)
    {

    }
    virtual ~WheelFilter()
    {

    }
protected:
    virtual bool eventFilter(QObject *o, QEvent *e) {
        if (e->type() == QEvent::Wheel) {
            e->ignore();
            return true;
        }
        return QObject::eventFilter(o,e);
    }
};

class absObjToThread;

class AbstractTest : public QFrame, public GlobalState
{
    Q_OBJECT

#define top_1       QFile settingsFile(fName); \
                    if (!settingsFile.open(QFile::ReadOnly)) \
                        return; \
                    QTextStream out(&settingsFile); \
                    out.readLine(); out.readLine(); out.readLine();

#define top_2(str)          if (str.isEmpty()) \
                                return; \
                            QFile settingsFile(str); \
                            if(!settingsFile.open(QFile::WriteOnly)) \
                                return; \
                            QTextStream in(&settingsFile); \
                            in << name_enabled->text() << endl; \
                            in << name_enabled->isChecked() << endl; \
                            in << mark->text() << endl; \
                            message(tr("Настройки теста сохранены (файл: %1, тест: %2)").arg(fileName->text()).arg(mark->text()));

public:
    explicit AbstractTest(QWidget *parent = 0);
    virtual ~AbstractTest();

    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
    void connectActions();
    QString getName() const;
    bool isReady() const;
    void setUserLevel(bool b, int p);
    void setEnable(bool b) { name_enabled->setChecked(b); }
    bool getEnable() { return name_enabled->isChecked(); }

    void setBeginTest();
    static AbstractTest* getBeginTest();
    void setEndTest();
    static AbstractTest* getEndTest();
    void resetBeginEnd();
    void cancellation();
    QString testType() const { return name_enabled->text(); }
    template<typename t>
    t* findField(QString& fieldName) { return (t*)(settings->findChild<t*>(fieldName)); }

    enum ValidState {
        DeviceIsNotAvailable,
        ConnectionIsNotAvailable,
        ItIsOk
    };
    enum RunningState {
        Running,
        Paused,
        Stopped,
        Completed,
        ErrorIsOccured,
        TestFault,
        Deleting
    };
private:
    void setValidState(ValidState);
    ValidState getValidState() const;
    RunningState getRunningState() const;
    CAnimatedSpinnerIcon* spinner;
    bool su;

signals:
    void globalStart();
    void startTestTh();
    void setEmit(QPushButton*, QPushButton*, QPushButton*);
    void unsetEmit(QPushButton*, QPushButton*, QPushButton*);
    void dragged();
    void dropped();
    void applyToAll(QString&,QString&,QString&,QString&);
    void chProgrMode(int);
    void logOutputReady(QString);

protected:
    void mousePressEvent(QMouseEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    bool event(QEvent *e);
    virtual void startTest() = 0;
    QThread testThread;
    absObjToThread *objToThread;
    QList<QLineEdit*> deviceLineEditList;
    QList<Device*> deviceList;
    QList<QString> subDeviceList;
    QMap<QString, QLabel*> statsMap;
    QString saveFileNameStr;
    QCheckBox *name_enabled;
    QLabel *fileName;
    QPushButton *startButton, *pauseButton, *stopButton;
    QDialog *settings;
    QWidget *stats;
    QVBoxLayout *devices;
    QTextBrowser *projectBrowser, *testsBrowser;
    QLineEdit* mark;
    static AbstractTest* yellowTest, *beginTest, *endTest;

    bool userMode() const { return !su; }
    int progrMode;
    void message(QString m = "", QString browser = "project");
    void setConnections(Device*);
    void setDisconnections(Device*);
    virtual void setEnabledSpecial(bool b);
    void disableStat();

protected slots:
    virtual void save();
    void markChanged(QString);
    void firstStartTest();
    void pauseTest();
    void stopTest();
    void newDev(QString);
    void checkDeviceAvailability(int);
    void deletingDevice(QString);
    void deletingDevice_part();
//    void connectingSockDevice();
//    void disconnectingSockDevice();
//    void errorDevice(QAbstractSocket::SocketError);
    void testOutout(QString);
    virtual void statsTestOutput(QString, double);
    virtual void statsToZero();
    virtual void statsSave();
    void setRunningState(int);

    void deleteObject();
private slots:
    void actDevMode();
    void actProgrMode(int p);

private:
    QMenu menu;
    QHBoxLayout *layout;
    ValidState validState;
    QLabel *statusIcon, *statusTxt, *begin_end_Icon;
    RunningState runningState;
    QSize forIcons;
    int forIconSize;

    bool isRunning();
};

class absObjToThread : public QObject//, public DeviceDriver
{
    Q_OBJECT

public:
    explicit absObjToThread(QObject* parent = 0);
    virtual ~absObjToThread() {
#ifdef PRINT_START_END_DESTRUCTOR
        qDebug() << "~absObjToThread()";
#endif
    }
    AbstractTest::RunningState threadState;

public slots:
    virtual void doWork() = 0;
    void template_doWork();
    void setState(int);
signals:
    void resultReady(int);
    void outputReady(QString);
    void statsOutputReady(QString,double);
    void logOutputReady(QString);
    //void statsOutputStringReady(QString, QString);
protected:
    int pause_stop();
};

const QString testTypeMemory(QObject::tr("Тест памяти/регистров"));
const QString testTypeRegisters(QObject::tr("Тест регистров"));
const QString testTypeEcho(QObject::tr("Тест \"Эхо\""));
const QString testTypeBulbs(QObject::tr("Тест \"лампочек\""));
const QString testTypeMonitor(QObject::tr("Тест \"монитор\""));
const QString testTypeTrmSingle(QObject::tr("Передача одного пакета"));
const QString testTypeVariation(QObject::tr("Перебор параметров команды"));
const QString testTypeGroupVar(QObject::tr("Перебор параметров групповой команды"));
const QString testTypeLoadSPI(QObject::tr("Загрузка по SPI"));
const QString testTypePause(QObject::tr("Пауза"));
const QString testTypeBuffers(QObject::tr("Проверка буферов"));
const QString testTypeNull(QObject::tr("Тест отладочный"));
const QString testTypeTrash(QObject::tr("Тест trash"));
const QString testTypeNoise(QObject::tr("Помехоустойчивость"));
const QString testTypeAlien(QObject::tr("Команды с чужими адресами"));
const QString testTypeInvalid(QObject::tr("Неопределённые команды"));
const QString testTypeLSCMes(QObject::tr("Тест передачи по НСК"));
const QString testTypeSpelsAmerage(QObject::tr("Тест на функционирование"));


namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB, bool su);
AbstractTest *loadTest(QString file, QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB);
}
#endif // ABSTRACTTEST_H
