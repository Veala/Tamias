#ifndef LSCFORMAT_H
#define LSCFORMAT_H

#include "extern_hsc.h"
#include "terasicdevice.h"
#include <QByteArray>

/// Шина
//#define lsc_BUS_A 0
//#define lsc_BUS_B 1
#define lsc_CW_BUS_A 0x80
#define lsc_CW_BUS_B 0

class C_LSC_Format
{
public:
    explicit C_LSC_Format(int bus) /*: pDev(0), pDevRT(0), pDevBC(0), pDevRT2(0)*/ { setBus(bus); }
    virtual ~C_LSC_Format() {}

    void addMessage(int &addrStackRT, int &addrStackBC, int &addrMesBC);
    void setDevice(TerasicDevice* dev) { pDev = dev; }
    void setSABufs(word32_t* rec, word32_t* trm, word32_t* brd_rec) { sa_buf_rec = rec, sa_buf_trm=trm, sa_buf_brd = brd_rec; }
    bool checkRW() { if (readRW() == expectedRW) { spoilRW(); return true;} return false; }
    bool checkSWBC();
    void spoilDataRT(uint rt, uint bc);
    bool checkDataRT(uint rt, uint bc);
    word32_t getCommand() {return command;}
    //bool checkExtended() { bool b=checkRT(); spoilAll(); return b; }

protected:
    TerasicDevice* pDev;
    //word32_t body[38];
    //int body_len;
    word32_t bus_flag;
    word32_t command;
    word32_t ctrlwrd;
    word32_t expectedSWRT, expectedSWBC;
    word32_t pStackBC, pStackRT;
    word32_t addrRW, expectedRW;
    //bool RTAvailable;

    word32_t* sa_buf_rec;
    word32_t* sa_buf_trm;
    word32_t* sa_buf_brd;

    word32_t addrIntLSC(word32_t raw_addr) { return 0x4000 + raw_addr*4; }


    void writeRam1Word(word32_t addr, word32_t val);
    word32_t getSABufRec(int sa) { return sa_buf_rec[sa]; }
    void readMemBlkLSC(int raw_addr, word32_t* buf, int num_words);

    //virtual void addControlWord(QByteArray &arr) = 0;
    //virtual void addCommandWord(QByteArray &arr) = 0;
    virtual void addDataBC(QByteArray &arr, int addrBC) = 0;
    virtual void addDataRT(int &addrStackRT) = 0;
    void devMemW(int startAddr, char *writeArray, int size);
    virtual bool broad() = 0;
    virtual int length() = 0;
    void spoilRW() { writeRam1Word(addrRW, initValRW); }
    void spoilSWBC() { writeRam1Word(pStackBC, initValRW); }
//    virtual void spoilAll() = 0;
    void addCommandBlock(int addrMesBC);
    word32_t readSWBC();

// признаки слова управления сообщением
//static const word32_t bc_a_b_flag = 0x80; //	выбор канала: 0x80 - А, 0 - Б
const word32_t modecode_flag = 4;  // формат КУ
const word32_t broad_flag    = 2;  // групповое сообщение
const word32_t rt_rt_flag    = 1;  // сообщение «ОУ-ОУ»

const word32_t initValRW = 0xFF00;   // значение, которым инициализируется память: оно должно быть затёрто ответным словом
const word32_t initValSW = 0xFF00;   // значение, которым инициализируется слово состояния
const word32_t spoilData = 0xFFFF;     // значение, которым инициализируется память, в которую будут переданы данные

private:
    //Device* pDev, pDevRT, pDevBC, pDevRT2;
    void setBus(int bus);
    word32_t readRW();
    virtual void spoilRT() = 0;
    virtual bool checkRT() = 0;
    void switchDev(uint devNum);
};

class C_LSC_Format1: public C_LSC_Format  // КШ-ОУ
{
public:
    explicit C_LSC_Format1(int rtaddr, int saddr, int num, word32_t val, bool my, int bus);
    bool broad() { return false; }
    int length() {return nw + 4; }
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag; arr.append((char*)CW, 4); }
    //void addCommandWord(QByteArray &arr) { command = (rta<<11)|(sa<<5)|nw; arr.append((char*)command, 4); }
    void addDataBC(QByteArray &arr, int addrBC);
    void addDataRT(int &addrStackRT);
 //   void spoilAll();
    virtual void spoilRT();
    virtual bool checkRT();
private:
    bool own;
    int rta, sa, nw;
    word32_t dataVal;
};

class C_LSC_Format2: public C_LSC_Format  // ОУ-КШ
{
public:
    bool broad() { return false; }
    int length() {return nw + 4; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag; arr.append((char*)CW, 4); }
    //void addCommandWord(QByteArray &arr) { command = (rta<<11)|(1<<10)|(sa<<5)|nw; arr.append((char*)command, 4); }
    //void addDataBC(QByteArray &arr, int addrBC);
    //void addDataRT(int &addrStackRT);
    //void spoilAll();
private:
    bool own;
    int rta, sa, nw;
    word32_t dataVal;
};

class C_LSC_Format3: public C_LSC_Format  // ОУ-ОУ
{
public:
    bool broad() { return false; }
    int length() {return nw + 6; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + rt_rt_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int type; // 0 - передача от rta другому, 1 - передача от другого к rta, 2 - оба адреса отличны от rta
    int rta, sa1, sa2, nw;
    word32_t addrRW1, addrRW2, dataVal;
};

class C_LSC_Format4: public C_LSC_Format  // КУ без СД
{
public:
    bool broad() { return false; }
    int length() {return 3; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + modecode_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, mc;
};

class C_LSC_Format5: public C_LSC_Format  // КУ и приём СД от ОУ
{
public:
    bool broad() { return false; }
    int length() {return 4; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + modecode_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, mc;
};

class C_LSC_Format6: public C_LSC_Format  // КУ со СД
{
public:
    bool broad() { return false; }
    int length() {return 4; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + modecode_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, mc, cd;
};

class C_LSC_Format7: public C_LSC_Format  // групповая КШ-всем ОУ
{
public:
    bool broad() { return true; }
    int length() {return nw + 3; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int sa, nw;
    word32_t dataVal;
};

class C_LSC_Format8: public C_LSC_Format  // групповая передача от ОУ другим ОУ
{
public:
    bool broad() { return true; }
    int length() {return nw + 5; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag + rt_rt_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, sa1, sa2, nw;
    word32_t dataVal;
};

class C_LSC_Format9: public C_LSC_Format  // групповая КУ
{
public:
    bool broad() { return true; }
    int length() {return 3; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag + modecode_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int mc;
};

class C_LSC_Format10: public C_LSC_Format  // групповая КУ со СД (синхронизация с СД)
{
public:
    bool broad() { return true; }
    int length() {return 4; }
    bool checkRT() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    //void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag + modecode_flag; arr.append((char*)CW, 4); }
    //void spoilAll() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int mc, cd;
};

#endif // LSCFORMAT_H
