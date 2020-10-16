#include "lscformat.h"

void C_LSC_Format::addMessage(int &addrStackRT, int &addrStackBC, int &addrMesBC)
{
    QByteArray arr;
    arr.append((char*)&ctrlwrd, 4);
    arr.append((char*)&command, 4);
    addDataBC(arr, addrMesBC);
    devMemW(addrMesBC, arr.data(), arr.size());
    pStackRT = addrStackRT;
    pStackBC = addrStackBC;
    addCommandBlock(addrMesBC);
    addrStackBC+=4;
    addDataRT(addrStackRT);
    addrMesBC += length();
}

void C_LSC_Format::switchDev(uint devNum)
{
    pDev->write_F2(REG_AUX_T_bus_select, (char*)(&devNum), 4);  //
}

void C_LSC_Format::devMemW(int startAddr, char *writeArray, int size)
{
    pDev->write_F2(addrIntLSC(startAddr), writeArray, size );
}

void C_LSC_Format::spoilDataRT(uint rt, uint bc)
{
    if (rt!=bc)
        switchDev(rt);
    spoilRT();
    if (rt!=bc)
        switchDev(bc);
}

bool C_LSC_Format::checkDataRT(uint rt, uint bc)
{
    if (rt!=bc)
        switchDev(rt);
    bool b = checkRT();
    if (b)
        spoilRT();
    if (rt!=bc)
        switchDev(bc);
    return b;
}

void C_LSC_Format::addCommandBlock(int addrMesBC)
{
    word32_t block[4];
    block[0] = initValSW;  // там будет слово состояния
    block[2] = 0;
    block[3] = addrMesBC;
    devMemW(pStackBC, (char*)block,  4*sizeof(word32_t));
}

void C_LSC_Format::setBus(int bus)
{
    bus_flag = (bus == 0 ? lsc_CW_BUS_A : lsc_CW_BUS_B);  // шина 0 - А, 1 - Б
    expectedSWBC = expectedSWRT = 0x8000 + bus*0x2000;
}

word32_t C_LSC_Format::readRW()
{
    word32_t answer;
    readMemBlkLSC(addrRW, &answer, 1);
    return answer;
}

word32_t C_LSC_Format::readSWBC()
{
    word32_t answer;
    readMemBlkLSC(pStackBC, &answer, 1);
    return answer;
}

void C_LSC_Format::writeRam1Word(word32_t addr, word32_t val)
{
    devMemW(addr, (char*)(&val), sizeof(word32_t));
}

void C_LSC_Format::readMemBlkLSC(int raw_addr, word32_t* buf, int num_words)
{
    pDev->read_F2(addrIntLSC(raw_addr), num_words*4, (char*)(buf));
}

/////////////////////////////////////////////////////////////////////////

C_LSC_Format1::C_LSC_Format1(int rtaddr, int saddr, int num, word32_t val, bool my, int bus):
    C_LSC_Format(bus),
    rta(rtaddr),
    sa(saddr),
    nw(num),
    dataVal(val),
    own(my)
{
    command = (rta << 11) | (sa << 5);
    if (nw<32)
        command |= nw;
    expectedRW = (own ? (rta<<11) : initValRW);
    ctrlwrd = bus_flag;
    if (!own)
        expectedSWBC |= 0x1000;   // признак ошибки
    /*
    body[0] = ctrlwrd;
    body[1] = command;
    for (int i=0; i<nw; i++)
        body[i+2] = val;
    body[nw+3] = 0;
    body_len = nw+4;
    */
}

void C_LSC_Format1::addDataBC(QByteArray &arr, int addrBC)
{
    for (int i=0; i<nw; i++)
        arr.append((char*)&dataVal, 4);
    arr.append((char*)&spoilData, 4);
    arr.append((char*)&initValRW, 4);
    addrRW = addrBC+nw+3;



    /*
        bufBC.addRamLSC(curCommandStackBC, spoiler);
        bufBC.addRamLSC(curCommandStackBC+2, 0x0);
        bufBC.addRamLSC(curCommandStackBC+3, ptrMessagesBC);  // 0, 400
        curCommandStackBC += 4;
        int datalen = 5;
        arrayAddrRW[num_messages] = ptrMessagesBC+datalen+3;
        num_messages++;
        ptrMessagesBC += (datalen+4);
        datalen = 3;
        arrayAddrRW[num_messages] = ptrMessagesBC+datalen+3;
        num_messages++;
    */
    #if 1
    /*
        bufBC.addRamLSC(0x000, 0x0);  // 0 - шина Б, 0x80 - шина А
        bufBC.addRamLSC(0x001, 0x828);
        bufBC.addRamLSC(0x026, 0x0);  // 0 - шина Б, 0x80 - шина А
        bufBC.addRamLSC(0x027, 0x848);
        bufBC.wtiteDev(devBC, true);
        fillMemBlk(devBC, 0x428, 8, 0xBB);
        fillMemBlk(devBC, 0x402, 8, 0xAA);
*/
    #else

        //bufBC1.wtiteDev(devBC, true);
        word32_t contW = 0;
        word32_t commW = 0x845;  // расшифровка: 8-rta, 4-sa, 5-nw
        word32_t data[36];
        data[0] = contW;
        data[1] = commW;
        for (int i=0; i<nw; i++)
            data[i+2] = dataVal;
        data[nw+2] = spoilData;
        data[nw+3] = initValRW; //{0, 0x1845, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0, 0, 0, 0x1883, 0xBB, 0xBB, 0xBB, 0, 0, 0, 0};
 //       devMemW(addrBC, (char*)data, 18 * 4);
        //writeMemBlkBC(0, data, 18);
    #endif


}

void C_LSC_Format1::spoilRT()
{
        word32_t arr_spoiler[32];
        for (int i=0; i<64; i++)
            arr_spoiler[i] = spoilData;
        devMemW(getSABufRec(sa), (char*)(arr_spoiler), 32*sizeof(word32_t));
        devMemW(pStackRT, (char*)(arr_spoiler), 4*sizeof(word32_t));
}
/*
void C_LSC_Format1::spoilAll()
{
    // Инициализируем память, которая может измениться после сообщения
    spoilRT();
    writeRam1Word(pStackBC, initValSW);
    writeRam1Word(addrRW, initValRW);
}
*/
bool C_LSC_Format1::checkRT()
{
    bool bRet = true;
    // проверка стека команд ОУ и принятых данных
        if (own)
        {
            word32_t stack[4];
            readMemBlkLSC(pStackRT, stack, 4);  // слово состояния, таймер, адрес данных, команда
            if (stack[0]!=expectedSWRT || stack[2] != sa_buf_rec[sa] || stack[3]!=command)
                bRet = false;

            // сравнить данные
            word32_t pData = stack[2];
            word32_t data[32];
            readMemBlkLSC(pData, data, nw);
            for (int i=0; i<nw; i++)
                if (data[i] != dataVal)
                {
                    bRet = false;
                    break;
                }
        }
        else
        {
            word32_t stack[4];
            readMemBlkLSC(pStackRT, stack, 4);  // слово состояния, таймер, адрес данных, команда
            if (stack[0]!=spoilData || stack[1] != spoilData || stack[2]!=spoilData || stack[3] != spoilData)
                bRet = false;

            // сравнить данные
            word32_t pData = sa_buf_rec[sa];
            word32_t data[32];
            readMemBlkLSC(pData, data, 32);
            for (int i=0; i<32; i++)
                if (data[i] != spoilData)
                {
                    bRet = false;
                    break;
                }
        }
    return bRet;
}

// проверка слова состояния КШ и ответного слова
bool C_LSC_Format::checkSWBC()
{
    if (readSWBC() !=  expectedSWBC)
        return false;
    spoilSWBC();
    return true;
}

void C_LSC_Format1::addDataRT(int &addrStackRT)
{
    if (own)
        addrStackRT+=4;
}
