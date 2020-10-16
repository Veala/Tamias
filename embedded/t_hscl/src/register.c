// -*- mode:c; coding:utf-8;
///
/// \file		register.c
///
/// \brief 		Чтение/запись регистров библиотеки t_hcsl
/// \details
///
/// \copyright  ЗАО НТЦ "Модуль"
/// \author     Мисюрёва Е.В.
///
/// \date 		29.01.2018
/// \version 	0.1
///  
///  История изменений:
///  
///

#include "hscl.h"
#include "hscl_reg.h"
#include <string.h>

#define AddrToCfg(addr)    (((addr) << 7) & hscl_FL_REG_CFG_rtavsk)
#define AddrFromCfg(cfg)   (((cfg) & hscl_FL_REG_CFG_rtavsk) >> 7)

#define ManTypeFromCfg(cfg)  ((cfg) & hscl_FL_REG_CFG_type_man)

word16_t hscl_CreateConfigReg(struct S_hscl_Config cfg)
{
    word16_t flags = AddrToCfg(cfg.rtavsk) | cfg.type_man;
    if (cfg.ena_codec)
        flags |= hscl_fl_REG_CFG_ena_codec;
    if (cfg.ena_aru)
        flags |= hscl_fl_REG_CFG_ena_aru;
    if (cfg.ena_mem_vsk)
        flags |= hscl_fl_REG_CFG_ena_mem_vsk;
    if (cfg.rtavsk_ena)
        flags |= hscl_fl_REG_CFG_rtavsk_ena;
    if (cfg.rt_bc == hscl_mode_BC)
        flags |= hscl_val_REG_CFG_mode_BC;
    if (cfg.rt_bc == hscl_mode_RT)
        flags |= hscl_val_REG_CFG_mode_RT;
    if (cfg.en_rt_bc_int)
        flags |= hscl_fl_REG_CFG_en_rt_bc_int;

    return flags;
}

size_t hscl_NumWordInSymbol_1p(word16_t cfg)
{
    return hscl_NumWordInSymbol_2p(ManTypeFromCfg(cfg), cfg & hscl_fl_REG_CFG_ena_codec);
}

size_t hscl_NumWordInSymbol_2p(int mt, int ena_codec)
{
    switch (mt)
    {
    case hscl_val_REG_CFG_type_man_QPSK:
        return ena_codec ? 28 : 24;
    case hscl_val_REG_CFG_type_man_QAM16:
        return ena_codec ? 56 : 44;
    case hscl_val_REG_CFG_type_man_QAM64:
        return ena_codec ? 84 : 56;
    default:
        return 0;
    }
}

int hscl_NumSymOFDM(int num_byte, int* num_sym, int* sym_len)
{
    int iRet = hscl_Error_NegativeLength;
    int aru = 0;
    *num_sym = -1;
    size_t len;
    word16_t cfg;
    
    if (num_byte < 0)
        return iRet;

    if ((iRet = hscl_ReadConfigReg(&cfg)) != hscl_OK)
        return iRet;

    len = hscl_NumWordInSymbol_1p(cfg);
    if (len == 0)
        return hscl_Error_IllegalManType;

    *sym_len = len * sizeof(word32_t);

    *num_sym = (num_byte + sizeof(word32_t)) / len;  // учтено командное слово
    if ((*num_sym) >= MAXNUMSYMBINPACK)   // или  MAXNUMSYMBINPACK-aru ??? !!!
        return hscl_Error_DataLong;
    
    return hscl_OK;
}

int CalcNumSymOFDM(unsigned int num_byte, int mt, int ena_codec)
{
    size_t len = hscl_NumWordInSymbol_2p(mt, ena_codec);
    if (len == 0)
        return -1;
    return (num_byte + NUMWORDINOFDMSYM + sizeof(word32_t)) / len;
}

word16_t CatchStatusChange()
{
    word16_t stat;
    if (hscl_ReadReg(&stat, hscl_NUMREG_status) == hscl_OK && (stat & hscl_FL_REG_STATUS_flags) != 0)
        return stat;
    return 0;
}

/// Получить номер прошивки.
int hscl_GetID()
{
    word16_t id;
    if (!hscl_ReadReg(&id, hscl_NUMREG_id))
        id = 0;
    return (int)id;
}

/// Запись типа манипуляции в регистр конфигурации.
/// Остальные признаки регистра конфигурации не меняются.
/// Возвращает hscl_OK или код ошибки (ошибка чтения/записи регистра или недопустимый тип манипуляции).
/// В случае ошибки значение регистра конфигурации не меняется.
int hscl_SetManType(int man_type)
{
    int iRet = hscl_Error_IllegalManType;
    if (man_type >= 0 && man_type < 3)
    {
        word16_t cfg;
        if (hscl_ReadReg(&cfg, hscl_NUMREG_cfg) == hscl_OK)
        {
            cfg = (cfg & 0xFFFC) | man_type;
            iRet = hscl_WriteReg(cfg, hscl_NUMREG_cfg) ? hscl_OK : hscl_Error_WriteReg;
        }
    }
    return iRet;
}

/// Получить тип манипуляции (из конфигурационного регистра)
int hscl_GetManType()
{
    int mt = hscl_val_REG_CFG_type_man_ERROR;
    word16_t cfg;
    if (hscl_ReadReg(&cfg, hscl_NUMREG_cfg))
        mt = cfg & hscl_FL_REG_CFG_type_man;

    return mt;
}

int ConfigDefault()
{
    if (
        hscl_WriteReg(hscl_NUMREG_ram_tx_rx_INIT_DEFAULT, hscl_NUMREG_ram_tx_rx) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_rx_cntr_INIT_DEFAULT, hscl_NUMREG_rx_cntr) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_creg_INIT_RESET, hscl_NUMREG_creg) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_cr_spi_INIT_DEFAULT, hscl_NUMREG_cr_spi) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_time_rsp_INIT_DEFAULT, hscl_NUMREG_time_rsp) == hscl_OK &&
        
        hscl_WriteReg(hscl_NUMREG_cnt_pct_tx_msw_INIT_0, hscl_NUMREG_cnt_pct_tx_msw) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_cnt_pct_tx_lsw_INIT_0, hscl_NUMREG_cnt_pct_tx_lsw) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_cnt_pct_rx_msw_INIT_0, hscl_NUMREG_cnt_pct_rx_msw) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_cnt_pct_rx_lsw_INIT_0, hscl_NUMREG_cnt_pct_rx_lsw) == hscl_OK &&

        hscl_WriteReg(hscl_NUMREG_lvl_sync_kf_rx_msw_INIT_DEFAULT, hscl_NUMREG_lvl_sync_kf_rx_msw) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_lvl_sync_kf_rx_lsw_INIT_DEFAULT, hscl_NUMREG_lvl_sync_kf_rx_lsw) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_lvl_sync_pre_rx_msw_INIT_DEFAULT, hscl_NUMREG_lvl_sync_pre_rx_msw) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_lvl_sync_pre_rx_lsw_INIT_DEFAULT, hscl_NUMREG_lvl_sync_pre_rx_lsw) == hscl_OK &&

        hscl_WriteReg(hscl_NUMREG_lvl_qam16_INIT_DEFAULT, hscl_NUMREG_lvl_qam16) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_lvl_qam64_low_INIT_DEFAULT, hscl_NUMREG_lvl_qam64_low) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_lvl_qam64_midle_INIT_DEFAULT, hscl_NUMREG_lvl_qam64_midle) == hscl_OK &&
        hscl_WriteReg(hscl_NUMREG_lvl_qam64_high_INIT_DEFAULT, hscl_NUMREG_lvl_qam64_high) == hscl_OK
        )
        return hscl_OK;

    return hscl_Error_Configuration;

}

int hscl_ReadConfigReg(word16_t* data)
{
    return hscl_ReadReg(data, hscl_NUMREG_cfg);
}

int hscl_CheckEnaAru(word16_t cfg)
{
    return (cfg & hscl_fl_REG_CFG_ena_aru);
}

int hscl_TypeMan(char* type_man, char* default_on_error)
{
    if (strcmp(type_man, "QPSK") == 0)
        return hscl_val_REG_CFG_type_man_QPSK;
    else if (strcmp(type_man, "QAM16") == 0)
        return hscl_val_REG_CFG_type_man_QAM16;
    else if (strcmp(type_man, "QAM64") == 0)
        return hscl_val_REG_CFG_type_man_QAM64;
    else if (strcmp(default_on_error, "QPSK") == 0)
        return hscl_val_REG_CFG_type_man_QPSK;
    else if (strcmp(default_on_error, "QAM16") == 0)
        return hscl_val_REG_CFG_type_man_QAM16;
    else if (strcmp(default_on_error , "QAM64") == 0)
        return hscl_val_REG_CFG_type_man_QAM64;
    else return hscl_val_REG_CFG_type_man_ERROR;
}

int hscl_WriteConfigReg(struct S_hscl_Config cfg)
{
    word16_t conf = hscl_CreateConfigReg(cfg);

    return hscl_WriteReg(conf, hscl_NUMREG_cfg);
}

/// Старт обмена
int hscl_Start()
{
    return hscl_WriteReg(hscl_fl_REG_CREG_start_bc, hscl_NUMREG_creg);
    /// или hscl_WriteReg(hscl_fl_REG_CREG_start_bc | hscl_NUMREG_creg_INIT_RESET, hscl_NUMREG_creg);  ???
}
