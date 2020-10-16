// -*- mode:c; coding:utf-8;
///
/// \file		test_bc.c
///
/// \brief 	Тестирование библиотеки t_hcsl
///         в режиме контроллера шины
/// \details
///
/// \copyright  ЗАО НТЦ "Модуль"
/// \author     Мисюрёва Е.В.
///
/// \date 		24.01.2018
/// \version 	0.1
///  
///  История изменений:
///  
///

#include "hscl.h"

#include <stdlib.h>    // malloc
//#include <iostream>  // cout

#define MAIN_TEST_
#include "testing.h"
#undef MAIN_TEST_

#include "test_parameters_bc.h"
#include <string.h>

#define isEnabled(strparam) (_stricmp(strparam, "YES") == 0)

/// Варианты заполнения массива данных.
#define data_default  0x2114F001

/// Формирование очередного элемента данных
word32_t data_value(int ns, int nw, int* val, int res, int nb, int type)
/// ns  - номер символа - очередного сегмента массива длины 124*4 байт
/// nw  - номер элемента в символе
/// val - указатель на числовой параметр (например, счетчик), используемый при генерации значения
/// res - ограничитель или другой числовой параметр, используемый при генерации значения
/// nb  - размер элемента в байтах (1, 2 или 4)
/// type - тип данных: 0 - фиксированное значение, 1 - возрастание со сбросом по порогу, 2 - ...
{
    if (type == 1)      // инкремент
    {
        if ((*val) >= res)
            *val = 0;
        if (nb == 1 && *val > 0xFF)
            *val = 0xFF;
        return (*val)++;
    }
    else if (type == 2)  // чередование
    {
        if (nb == 1)
        {
            switch (nw % 4)
            {
            case 0:
                return (*val) & 0xFF;
            case 1:
                return ns & 0xFF;
            case 2:
                return 0;
            case 3:
                return ((nw + 3) / 4) & 0xFF;
            }
        }
        else if (nb == 2)
        {
            if (nw%2)
                return (*val) & 0xFF00 | ns;
            else
                return nw/2 + 1;
        }
        else
            return ((*val) & 0xFF000000) | (ns << 12) | (nw + 1);
    }
    else   // фиксированное значение
        return *val;
}

int  data_alignment;         /// количество 32-разрядных слов, которые будут заполнены данными в каждом сегменте из 128 слов,
/// а остальные слова до конца сегмента будут дописаны нулями; при значении 0 число слов
/// в каждом сегменте будет совпадать с числом слов в ofdm символе
int  data_unit_size;         /// размер одной ячейки данных - 1, 2 или 4 байта
void FillData(void* mem, int max_size, int nw, int nb, int value, char* type)
{
    int fill_type = 0;  // 0 - fix, 1 - inc, 2 - alt, 3 = num
    int res = 0;
    int mask = 0xFFFFFFFF;
    int maxword;

    if (nb == 1)
        mask = 0xFF;
    else if (nb == 2)
        mask = 0xFFFF;

    if (strcmp(type, "000") == 0)
    {
        value = 0;
    }
    else if (strcmp(type, "FFF") == 0)
    {
        value = 0xFFFFFFFF;
    }
    else if (strcmp(type, "111") == 0)
    {
        value = 0x11111111;
    }
    else if (strcmp(type, "Alt") == 0)
    {
        fill_type = 2;
    }
    else if (strcmp(type, "Inc") == 0)
    {
        fill_type = 1;
        res = mask;
    }
    else if (strcmp(type, "INC") == 0)
    {
        fill_type = 1;
        res = mask;
    }
    else if (strcmp(type, "ABC") == 0)
    {
        fill_type = 1;
        res = 26;
        value = 'A';
    }
    else if (strcmp(type, "AAA") == 0)
    {
        value = 0xAAAAAAAA;
    }
    else if (strcmp(type, "abc") == 0)
    {
        fill_type = 1;
        res = 26;
        value = 'a';
    }
    else if (strcmp(type, "555") == 0)
    {
        value = 0x55555555;
    }
    else if (strcmp(type, "Num") == 0)
    {
        fill_type = 1;
        res = nw;
        value = 0;
    }
    else if (strcmp(type, "FIX") == 0)
    {
    }
    else if (strcmp(type, "Fix") == 0)
    {
    }
    else if (strcmp(type, "Def") == 0)
    {
        value = data_default;
    }
    else
    {
        value = data_default;
    }

    if (fill_type == 0)
        value &= mask;

    maxword = NUMWORDINOFDMSYM * sizeof(word32_t) / nb;
    nw *= (sizeof(word32_t) / nb);

    if (nb == 1)
    {
        char* addr = (char*)mem;
        int cnt = 0;
        for (int i = 0; i < cnt; i++)
        {
            for (int j = 0; j < nw && cnt < max_size; j++, addr++, cnt++)
                *addr = (char)(data_value(i, j, &value, res, 1, fill_type));
            for (int j = nw; j < maxword && cnt < max_size; j++, addr++, cnt++)
                *addr = 0;
        }
    }
    else if (nb == 2)
    {
        word16_t* addr = (word16_t*)mem;
    }
    else // nb == 4
    {
        word32_t* addr = (word32_t*)mem;
        for (int i = 0; i < MAXNUMSYM; i++)
        {
            for (int j = 0; j < nw; j++, addr++)
                *addr = data_value(i, j, &value, res, nb, fill_type);
            for (int j = nw; j < maxword; j++, addr++)
                *addr = 0;
        }
    }
}

/// Загрузка параметров теста из входного файла file_name. Пока не реализована.
/// Функция будет переопределять параметры, заданные в файле, и не менять остальные параметры.
bool LoadFromFile(struct S_TestSettings* params, char* file_name)
{
    if (file_name && *file_name)
        return false;
    
    return true;
}

bool LoadConfigSettings(struct S_hscl_Config* cfg)
{
    if (strcmp(RT_BC_MODE, "BC") == 0)
        cfg->rt_bc = hscl_mode_BC;
    if (strcmp(RT_BC_MODE, "RT") == 0)
        cfg->rt_bc = hscl_mode_RT;
    else return false;

    cfg->ena_codec = CODEC;
    cfg->ena_aru = ENA_ARU;
    cfg->ena_mem_vsk = EVA_MEM_VSK;
    cfg->rtavsk_ena = EVA_ADDR_HSC;
    cfg->rtavsk = ADDR_HSC;
    cfg->en_rt_bc_int = INTERRUPTION;

    cfg->type_man = hscl_TypeMan(MANIPULATION, "QPSK");
    return true;
}

void LoadCommandSetting(enum command_type* type, char* strtype, int* cnt, int repeat, bool* group, char* broadcast)
{
    *type = ct_NONE;
    if (strcmp(strtype, "TRM") == 0)
        *type = ct_TRM;
    else if (strcmp(strtype, "REC") == 0)
        *type = ct_REC;
    else if (strcmp(strtype, "CODE") == 0)
        *type = ct_CODE;

    *cnt = 1;
    if (*type == ct_CODE)
        *cnt = repeat;
    *group = isEnabled(broadcast);
}

/// Загрузка параметров по умолчанию, заданнных в файле "test_parameters_bc.h".
bool LoadDefault(struct S_TestSettings* params)
{
    params->check_version = false;
#ifdef ID_NUMBER
    params->version = ID_NUMBER;
    params->check_version = true;
#endif

    params->use_interruption = isEnabled(USE_INTERRUPTION);
    params->wait_time = WAIT_TIME;

    LoadCommandSetting(&(params->command1), COMMAND_1, &(params->repeat1), REPEAT_1, &(params->broadcast1), BROADCAST_1);
    LoadCommandSetting(&(params->command2), COMMAND_2, &(params->repeat2), REPEAT_2, &(params->broadcast2), BROADCAST_2);

#ifdef NUM_SYM
    params->num_sym = NUM_SYM;
#else
    params->num_sym = 0;
#endif
#ifdef DATA_SIZE
    params->size_data = DATA_SIZE;
#else
    params->size_data = 0;
#endif
    params->code = COMMAND_CODE;
    params->addr_rt = ADDR_RT;
//    std::cout << "sizeof(params->data_gen_type) = " << sizeof(params->data_gen_type) << std::endl;
    strcpy_s(params->data_gen_type, sizeof(params->data_gen_type) - 1, "Fix");
    params->data_init_value = data_default;
#ifdef DATA_UNIT_SIZE
    params->data_unit_size = DATA_UNIT_SIZE;
#else
    params->data_unit_size = 0;
#endif
#ifdef DATA_GEN_TYPE
    strncpy_s(params->data_gen_type, sizeof(params->data_gen_type) - 1, DATA_GEN_TYPE, 3);
#endif
#ifdef DATA_VALUE
    params->data_init_value = DATA_VALUE;
#else
    if (strcmp(params->data_gen_type, "Inc") == 0 || strcmp(params->data_gen_type, "INC") == 0)
        params->data_init_value = 0;
#endif

    params->init_scenario = INIT_VSK;

    return LoadConfigSettings(&(params->cfg_reg));
}

/// Проверка и переопределение зависимых параметров.
bool CheckSettings(struct S_TestSettings* params)
{
    if (params->wait_time < 1)
        return false;

    if (params->cfg_reg.en_rt_bc_int == 0)
        params->use_interruption = 0;

    if (params->addr_rt > MAX_RT_ADDR)
        params->addr_rt = MAX_RT_ADDR;

    if (params->num_sym >= MAXNUMSYM)
        params->num_sym = MAXNUMSYM-1;

    if (params->data_unit_size != 1 && params->data_unit_size != 2 && params->data_unit_size != 4)
        params->data_unit_size = 0;

    return true;
}

bool LoadSettings(struct S_TestSettings* params, char* file_name)
{
    return LoadDefault(params) && LoadFromFile(params, file_name) && CheckSettings(params);
}

int hscl_SingleExchange(char* data, size_t size, size_t len, addr_t addr_rt, int trbit, unsigned int code, int wait, bool use_int)
{
    int iRet;
    char buff[MAXPACKAGESIZE];
    char* ptr = buff;
    unsigned int cur_len = 0;
    int num_s = trbit == hscl_tr_TRM ? (size + sizeof(word32_t)) / len : 0;
    if (trbit == hscl_tr_TRM)
    {
        cur_len = len - sizeof(word32_t);
        if (size < cur_len)
            cur_len = size;
        size -= cur_len;
        memcpy(buff + sizeof(word32_t), data, cur_len);
    }
    memset(buff + sizeof(word32_t)+cur_len, 0, NUMBYTEINOFDMSYM - sizeof(word32_t) - cur_len);
    /// Создаём в буфере buff командную часть пакета.
    hscl_CreateCommandWord((word32_t*)buff, addr_rt, num_s, trbit, code);
    /// Копируем в буфер остальные данные, разбивая их на OFDM символы.
    if (trbit == hscl_tr_TRM)
    {
        while (size > 0)
        {
            cur_len = size > len ? len : size;
            ptr += NUMBYTEINOFDMSYM;
            memcpy(ptr, data, cur_len);
            memset(ptr + cur_len, 0, NUMBYTEINOFDMSYM - cur_len);
            data += len;
            size -= cur_len;
        }
    }
    /// Копируем buff в буфер передачи ВСК.
    iRet = hscl_WriteMem(buff, num_s);
    if (iRet != hscl_OK)
        return iRet;
    /// Старт обмена.

    iRet = hscl_CreateCommandPack(data, size, buff, len, addr_rt, trbit, code);
    
    //int num_s
    // iRet = hscl_NumSymOFDM(size, &num_s, &len);

    return iRet;
}

int Test_BC_TRM(char* data, unsigned int size, addr_t addr_rt, unsigned int code, int wait, bool use_int)
{
    int iRet = test_Unknown_Error;
    int iPackStarted = 0;

    if (size == 0)
    {
    }
    else
    {
        word16_t cfg;
        size_t len, max_size;
        int iRet = hscl_ReadConfigReg(&cfg);
        if (iRet != hscl_OK)
            return iRet;
        len = hscl_NumWordInSymbol_1p(cfg) * sizeof(word32_t);
        if (len == 0)
            return iRet;
        max_size = len * MAXNUMSYMBINPACK - sizeof(word32_t);
        if (hscl_CheckEnaAru(cfg) != 0)
            max_size -= len;
        do
        {
            int cur_size = size > max_size ? max_size : size;
            iRet = hscl_SingleExchange(data, cur_size, len, addr_rt, hscl_tr_TRM, code, wait, use_int/*, &iPackStarted*/);
            size -= cur_size;
            if (iRet != hscl_OK)
                return iRet;
        }
        while (size > 0);
    }

    word32_t command;
    word32_t buffer[MAXPACKAGESIZE];
    unsigned int  num_symbols;  /// число передаваемых символов



    hscl_Reset();

    iRet = hscl_CreateCommandPack(buffer, MAXPACKAGESIZE, data, num_symbols, addr_rt, hscl_tr_TRM, code);
    if (iRet != hscl_OK)
        return iRet;

    hscl_Start();

    return hscl_OK;
}

int main(int argc, char* argv[])
{
    int iRet = test_Unknown_Error;
    addr_t addr_rt;
    char* file_name = NULL;
    struct S_TestSettings params;
    char buffer[MAXPACKAGESIZE];
    char *data = NULL;

    if (argc > 1)
        file_name = argv[1];

    if (!LoadSettings(&params, file_name))
        return test_Error_LoadSettings;

    if ((iRet = hscl_CheckDevice()) != hscl_OK)
        return iRet;

    if (params.check_version && hscl_GetID() != params.version)
        return test_Error_IdNumber;

    if (params.use_interruption && (iRet = hscl_SetProcIntBC(Test_IntFlagSetOn)) != hscl_OK)
        return iRet;

    if ((iRet = hscl_Configurate(params.init_scenario)) != hscl_OK)
        return iRet;

    if ((iRet = hscl_WriteConfigReg(params.cfg_reg)) != hscl_OK)
        return iRet;

    if (params.command1 == ct_TRM || params.command1 == ct_REC || params.command2 == ct_TRM || params.command2 == ct_REC)
    {
        if (params.size_data > 0)
        {
            int len;
            int num_s;
            iRet = hscl_NumSymOFDM(params.size_data, &num_s, &len);
            data = malloc((num_s + 1) * (2 * NUMBYTEINOFDMSYM));
            FillData(data, params.size_data, NUMWORDINOFDMSYM,
                     params.data_unit_size == 0 ? 1 : params.data_unit_size, params.data_init_value, params.data_gen_type);
        }
        else
            data = malloc(NUMBYTEINOFDMSYM);
    }
    
    if (params.command1 == ct_CODE || params.command2 == ct_CODE)
    {
        FillData(buffer, MAXPACKAGESIZE, hscl_NumWordInSymbol_2p(params.cfg_reg.type_man, params.cfg_reg.ena_codec),
              params.data_unit_size == 0 ? 4 : params.data_unit_size, params.data_init_value, params.data_gen_type);
    }

    addr_rt = params.broadcast1 ? params.addr_rt : BRD_RT_ADDR;
    if (params.command1 == ct_REC)
    {
    }
    else if (params.command1 == ct_TRM)
    {
        iRet = Test_BC_TRM(data, params.size_data, addr_rt, params.code, params.wait_time, params.use_interruption);
        if (iRet != hscl_OK)
            return iRet;
    }
    else if (params.command1 == ct_CODE)
    {
        for (int i = 0; i < params.repeat1; i++)
        {
        }
    }

    addr_rt = params.broadcast1 ? params.addr_rt : BRD_RT_ADDR;
    if (params.command2 == ct_REC)
    {
    }
    else if (params.command2 == ct_TRM)
    {
    }
    else if (params.command2 == ct_CODE)
    {
        for (int i = 0; i < params.repeat2; i++)
        {
        }
    }

    if (data)
        free (data);
    return iRet;
}
