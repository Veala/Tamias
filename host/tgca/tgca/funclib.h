#ifndef FUNCLIB_H
#define FUNCLIB_H
#include "QByteArray"
//#define PRINT_START_END_DESTRUCTOR

enum ExchangeFormat {
    zero,
    write_f1,
    write_f2,
    read_f1,
    read_f2,
    write_echo,
    cpy_on_hard = 7
};

enum DeviceMode {
    LSC_BC,
    LSC_RT_st,
    LSC_RT_al,
    LSC_MC
};

struct Head {
    Head(quint32 cmd = 0, quint32 dsz = 0) : cmd(cmd), dsz(dsz) { }
    quint32 cmd;
    quint32 dsz;
};

struct f1_Head : Head {

};

struct f2_Head : Head {
    f2_Head(quint32 cmd = 0, quint32 dsz = 0, quint32 startAddr = 0) : Head(cmd, dsz),
        startAddr(startAddr) { }
    quint32 startAddr;
};

class FormatData
{
    explicit FormatData(ExchangeFormat f = write_f1, int size = 288);
    ~FormatData();
    void append(char*, int size);
    void clear();
    const char *getAllData();
private:
    ExchangeFormat exf;
    char *protocolFrame, *payload;
    unsigned int size, *frameSize, curPayloadPoint, offset;
};

struct CommandWord {
    CommandWord(quint32 addr = 0, quint32 cmd_code = 0, quint32 t_r = 0, quint32 num_of_symbols = 0, quint32 rtaddr = 0) :
        addr(addr), cmd_code(cmd_code), t_r(t_r), num_of_symbols(num_of_symbols), rtaddr(rtaddr) { }
    quint32 addr;
    quint32 cmd_code : 7;
    quint32 t_r : 1;
    quint32 num_of_symbols : 8;
    quint32 rtaddr : 5;
};

struct ResponseWord {
    ResponseWord(quint32 addr = 0, quint32 daterr = 0, quint32 rtaddr = 0) :
        addr(addr), daterr(daterr), rtaddr(rtaddr) { }
    quint32 addr;
    quint32  : 2;
    quint32 daterr : 1;
    quint32  : 13;
    quint32 rtaddr : 5;
};

QByteArray cmdHead(int cmd, int dsz);

#endif // FUNCLIB_H
