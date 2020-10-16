// -*- mode:c; coding:utf-8;
///
/// \file		hscl.c
///
/// \brief 		Главный файл библиотеки t_hcsl
///             t_hscl - Библиотека времени исполнения для работы с логикой ВСК.
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

#include "hscl_i.h"
#include "hscl.h"

#include <string.h>
//#include <stddef.h>

struct S_HSCL     //  изменяется при изменении конфигурации
{
    int nbyte;    // число слов в одном символе
    int nbmax;    // максимальное число байт в пакете
};

static struct S_HSCL hscl_global;

int hscl_Configurate(int param)
{
    int iRet = hscl_Error_Configuration;

    switch (param)
    {
    case hscl_Config_Default:
        iRet = ConfigDefault();
        break;
    default:
        break;
    }

    return iRet;
}

///    mem_src  - указатель на исходные данные; может быть NULL при num_s == 0 или tr != 0;
///    size_src - число байтов для передачи; функция вычисляет нужное для этого количество символов OFDM и проверяет, чтобы оно не превысило
///               максимальное допустимое значение 254 (в режиме с 3 преамбулами 253 или 254 ??? !!!)
///    addr  - адрес ОУ (0 - 30) или адрес групповой команды 31;
///    tr    - направление передачи: hscl_tr_REC или hscl_tr_TRM;
///    code  - код команды.
/// Возвращаемое значение: hscl_OK или код ошибки; в случае ошибки содержимое mem_dst может оказаться произвольным.
int hscl_CreateCommandPack(void* mem_dst, size_t size_dst, void* mem_src, size_t size_src, addr_t addr, int tr, unsigned int code)
/// Создает командное слово и копирует его в mem_dst вместе с соответствующим количеством слов данных из mem_src.
{
    int iRet = hscl_OK;
    int index = 0;
    int sym_len = 0;
    word32_t command;
    char* ptr_dst = mem_dst;
    char* ptr_src = mem_src;
    int num_s;

    if (mem_src == NULL && tr == hscl_tr_REC && size_src > 0)
        return hscl_Error_DataAbsent;

    iRet = hscl_NumSymOFDM(size_src, &num_s, &sym_len);
    if (num_s < 0)
        return iRet;

    if (tr == hscl_tr_REC && ((num_s + 1) * NUMWORDINOFDMSYM > (int)size_dst))
        return hscl_Error_TooShortBuffer;

    /// Преамбулы будут созданы автоматически. Записываем в память только OFDM символы.

    if ((iRet = hscl_CreateCommandWord(&command, addr, num_s, tr, code)) != hscl_OK)
        return iRet;

    memcpy(ptr_dst, &command, sizeof(word32_t));
    ptr_dst += sizeof(word32_t);

    if (tr != hscl_tr_REC)
        size_src = num_s = 0;  // 

    if (size_src == 0)
    {
        memset((void*)ptr_dst, 0, NUMBYTEINOFDMSYM - sizeof(word32_t));
    }
    else
    {
        int sz_rest = NUMBYTEINOFDMSYM - sizeof(word32_t) > size_src ? NUMBYTEINOFDMSYM - sizeof(word32_t)-size_src : 0;
        int sz_copy = sz_rest == 0 ? NUMBYTEINOFDMSYM - sizeof(word32_t) : size_src;

        /// копируем первый символ
        memcpy((void*)ptr_dst, (void*)mem_src, sz_copy);
        memset((void*)(ptr_dst + sz_copy), 0, sz_rest);
        size_src -= sz_copy;
        ptr_dst += (NUMBYTEINOFDMSYM - sizeof(word32_t));
        ptr_src += sz_copy;
        /// копируем целые символы
        for(int ns = 1; ns < num_s; ns++)
        {
            memcpy((void*)ptr_dst, (void*)ptr_src, sz_copy);
            memset((void*)(ptr_dst + sz_copy), 0, sz_rest);
        }
        /// копируем последний символ, возможно неполный
    }
    return hscl_OK;
}

int hscl_CreateCommandWord(word32_t* command_word, addr_t addr, unsigned int num_s, int tr, unsigned int code)
{
    int iRet = hscl_OK;
    if (addr > MAX_RT_ADDR)
    {
        addr &= MAX_RT_ADDR;
        iRet = hscl_Error_CreateCommand;
    }
    if (num_s >= MAXNUMSYM)
    {
        num_s &= 0xFF;
        iRet = hscl_Error_CreateCommand;
    }
    if (code > MAX_COMMAND_CODE)
    {
        code &= MAX_COMMAND_CODE;
        iRet = hscl_Error_CreateCommand;
    }
    if (tr != hscl_tr_REC && tr != hscl_tr_TRM)
    {
        iRet = hscl_Error_CreateCommand;
        if (tr != 0)
            tr = 1;
    }
    *command_word = (addr << 16) | (num_s << 8) | (hscl_fl_CW_tr_bit * tr) | code;
    return iRet;
}

size_t hscl_NumWordInSymbol_0p()
{
    size_t len = 0;
    word16_t data;
    if (hscl_ReadConfigReg(&data) == hscl_OK)
        len = hscl_NumWordInSymbol_1p(data);
    return len;
}

size_t hscl_MaxNumByte()
{
    size_t len = 0, ret = 0;
    word16_t data;
    if (hscl_ReadConfigReg(&data) == hscl_OK)
    {
        len = hscl_NumWordInSymbol_1p(data);
        if (len > 0)
            ret = (len * MAXNUMSYMBINPACK - 1) * sizeof(word32_t);
        //if (hscl_CheckEnaAru(data) != 0)  ret -= len* sizeof(word32_t);  //??? !!!
    }
    return ret;
}


/// НЕРЕАЛИЗОВАННЫЕ ФУНКЦИИ


void hscl_Stop() {}

/// Установка пользовательской обработки прерывания
int hscl_SetProcIntBC(int(*pF)(void))
{
    return hscl_OK;  /// или hscl_Error_SetFunction
}

int hscl_SetProcIntRT(int(*pF)(void))
{
    return hscl_Error_SetFunction;
}

int hscl_CheckDevice()
{
    return hscl_OK;
}

void hscl_Reset()
{
}

int hscl_WriteReg(word16_t data, unsigned short reg_num)
{
    return hscl_OK;
}

int hscl_ReadReg(word16_t* data, unsigned short reg_num)
{
    return hscl_OK;
}

/// Переписать num_s символов OFDM с адреса mem_src в активный буфер передатчика ВСК.
int hscl_WriteMem(void* mem_src, unsigned int num_s)
{
    return hscl_OK;
}

/// Скопировать содержимое активного буфера приёмника ВСК в mem_dst.
int hscl_ReadMem(void* mem_dst)
{
    return hscl_OK;
}
