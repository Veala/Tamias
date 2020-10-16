#ifndef LSCMESSAGETEST_H
#define LSCMESSAGETEST_H

#include "abstracttest.h"
#include "../lscformat.h"
#include "../writedevicememory.h"
#include <QRadioButton>
#include <QSpinBox>

class LscMessageTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit LscMessageTest(QWidget *parent): AbstractTest(parent) {}
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    void defineFields();
    void load(QString fName);
    void setStatSettings();

    QLineEdit *deviceEditBC, *deviceEditRT;
    QCheckBox *checkBoxStartMessages;
    QCheckBox *checkBoxConfigRT;
    QCheckBox *checkBoxAutorepeat;
    QCheckBox *checkBoxPulse;
    QCheckBox *checkBoxCheckRW;
    QCheckBox *checkBoxCheckAll;
    QCheckBox *checkBoxConfigBC;
    QCheckBox *checkBoxOut;
    QComboBox *comboBoxRTA;

    QLineEdit *lineEditDW;
    QComboBox *comboBoxNW;
    QComboBox *comboBoxBus;

    // Эта группа активна при выключенном автоповторе.
    QLabel *labelHeaderRepeat;
    QLabel *labelPause;
    QLineEdit *lineEditPause;
    QRadioButton *radioButtonSet;
    QRadioButton *radioButtonUnlim;
    QLineEdit *lineEditNum;

    // Эта группа активна при включенном автоповторе.
    QLabel *labelFT;
    QLineEdit *lineEditFT;

    // Значения параметров не редактируются, но берутся из этих полей.
    QLabel *lineMesRTA;
    QLabel *lineMesSA;
    QLabel *lineMesType;

    // Пока скрыты
    QLabel *labelNumMes;
    QSpinBox *spinBox;

private slots:
    void onRadio();
    void onRTA();
    void onAuto();
    void statsTestOutput(QString str, double n);
    void statsToZero();
    void statsSave();
};

class lscMessageObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
    void terminate(int);
public:
    explicit lscMessageObjToThread();
    ~lscMessageObjToThread();

    //Device *devBC, *devRT;
    Device *dev;
    //QString subDevBC;
    //QString subDevRT;
    //QString m_curSubDev;
    uint busBC, busRT;
    //void setDeviceToBC();
    //void setDeviceToRT();

    bool initBC, initRT, startBC, autorepeat, checkRW, checkRTBC, setOutEnabled, started, impulse;
    int rtaddr, Count, nw, pause, ft, sa, torta;
    quint32 dataWord;
    QString mFormat;
    int num_messages;
    int bus;

private:
    // общие функции теста
//    AbstractTest::RunningState connectDevice();
    bool isTestRunning(int st);
    void pauseForSwitchDev() { thread()->msleep(1000); }
    void stdOutput(QString str) { if (setOutEnabled) emit outputReady(str); /*qDebug() << str;*/ }

    // запись в устройство
    void writeRegSingle(word32_t num, word32_t value);
    bool fillMemBlkLSC(int dst_addr, int num_words16, word16_t value);
    //bool fillMemBlkRT(int dst_addr, int num_words16, word16_t value) { return fillMemBlk(devRT, dst_addr, num_words16, value); }

    bool addMessage(C_LSC_Format* format);
    bool checkMessage(C_LSC_Format* format);
    bool configRT(word32_t initCommandStackRT);
    bool configBC();
    bool createMessages(std::vector<C_LSC_Format*>& messages);
    void startMessages(CWriteDeviceMemory &bufBC);
    void initStartMessages(CWriteDeviceMemory &bufBC);
    void switchToDev(uint num);

    //QByteArray regRT, regBC;
    const int max_num_messages = 64;
    int fixRT, fixBC;
    word32_t arrayAddrRW[64];
    int curCommandStackBC;
    int curCommandStackRT;
    int ptrMessagesBC;
    //bool enaAccessRT;

    word32_t stackRT, stackBC;

    const word32_t initCommandStackRT = 0;
    const word32_t initCommandStackBC = 0x200;  // 0
    const word32_t initMessageArea = 0; // 0x200
    const word32_t bufDataRT = 0x400;

    word32_t sa_buf_rec[32];
    word32_t sa_buf_trm[32];
    word32_t sa_buf_brd[32];

signals:
    void setResultReady(int);
};

#endif // LSCMESSAGETEST_H
