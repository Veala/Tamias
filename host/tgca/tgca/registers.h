// -*- mode:c; coding:utf-8;
///
/// \file		registers.h
///
/// \brief 		Чтение/запись регистров
/// \details
///
/// \copyright  ЗАО НТЦ "Модуль"
/// \author     Мисюрёва Е.В.
///
/// \date 		28.07.2018
/// \version 	0.1
///  
///  История изменений:
///  
///

#ifndef REGISTERS_H
#define REGISTERS_H

#define NUMOFREGVSK    32
#define REGVSKADDRFROM 0x80

#define NUMOFREGNSK    32
#define REGNSKADDRFROM 0x00

typedef unsigned short  reg16_t;


///  АДРЕСА ВСПОМОГАТЕЛЬНЫХ РЕГИСТРОВ

#define REG_AUX_T_rtaddr                 0x180   // вспомогательный регистр: адрес ОУ
#define REG_AUX_T_interruption           0x184   // вспомогательный регистр: прерывание
#define REG_AUX_T_bus_select             0x188   // вспомогательный регистр: шина
#define REG_AUX_T_win_mode_reset         0x190   // вспомогательный регистр: оконный режим
#define REG_AUX_T_bulb                   0x194   // регистр "лампочек"


///  АДРЕСА РЕГИСТРОВ ВСК

#define   REG_VSK_ram_tx_rx             0x80
//                                         0x84
#define   REG_VSK_id                    0x88   // только чтение
#define   REG_VSK_status                0x8C   // только чтение
#define   REG_VSK_cfg                   0x90
#define   REG_VSK_tx_cntr               0x94
//                                         0x98
#define   REG_VSK_rx_cntr               0x9C
#define   REG_VSK_creg                  0xA0
#define   REG_VSK_cr_spi                0xA4
#define   REG_VSK_dr_spi_msw            0xA8
#define   REG_VSK_dr_spi_lsw            0xAC
//                                         0xB0
#define   REG_VSK_time_rsp              0xB4
#define   REG_VSK_cnt_pct_tx_msw        0xB8
#define   REG_VSK_cnt_pct_tx_lsw        0xBC
#define   REG_VSK_cnt_pct_rx_msw        0xC0
#define   REG_VSK_cnt_pct_rx_lsw        0xC4

#define   REG_VSK_lvl_sync_kf_rx_msw    0xC8   // чтение/запись
#define   REG_VSK_prcs_max_sync_msw     0xC8   // только чтение

#define   REG_VSK_lvl_sync_kf_rx_lsw    0xCC   // чтение/запись
#define   REG_VSK_prcs_max_sync_lsw     0xCC   // только чтение

#define   REG_VSK_lvl_sync_pre_rx_msw   0xD0   // чтение/запись
#define   REG_VSK_prs_level_max_rn_msw  0xD0   // только чтение

#define   REG_VSK_lvl_sync_pre_rx_lsw   0xD4   // чтение/запись
#define   REG_VSK_prs_level_max_rn_lsw  0xD4   // только чтение

#define   REG_VSK_lvl_qam16             0xD8
#define   REG_VSK_lvl_qam64_low         0xDC
#define   REG_VSK_lvl_qam64_middle      0xE0
#define   REG_VSK_lvl_qam64_high        0xE4
#define   REG_VSK_amplification_factor  0xE8
#define   REG_VSK_amplitude_signal      0xEC   // только чтение
#define   REG_VSK_g_sp                  0xF0
#define   REG_VSK_g_1_sp_high           0xF4
#define   REG_VSK_g_1_sp_low            0xF8
#define   REG_VSK_pll_reg               0xFC

#define   REG_VSK_BEGIN_ADDR            0x80
#define   REG_VSK_END_ADDR              0xFC

#define VSK_ReadStatusReg(data)   VSK_ReadReg(data, REG_VSK_status)
#define VSK_WriteConfigReg_(data) VSK_WriteReg(data, REG_VSK_cfg)
//reg16_t VSK_CreateConfigReg(struct S_hscl_Config cfg);       /// возвращает образ конфигурационного регистра

/// Разряды регистра статуса.
#define   fl_REG_STATUS_rt_bc_int      1         // флаг прерывания
#define   fl_REG_STATUS_rs_err        (1 << 4)   // флаг ошибки кодека Рида - Соломона
#define   fl_REG_STATUS_no_aw_err     (1 << 5)   // флаг ошибки отсутствия ответного пакета на негрупповую команду
#define   fl_REG_STATUS_yes_aw_gr_err (1 << 6)   // флаг ошибки получения ответного пакета на групповую команду
#define   fl_REG_STATUS_tx_num_buf    (1 << 10)  // номер активного буфера передатчика во внешней памяти
#define   fl_REG_STATUS_rx_num_buf    (1 << 11)  // номер активного буфера приёмника во внешней памяти
#define   FL_REG_STATUS_flags          0xFF      // маска, содержащая все разряды флагов, но исключающая номера активных буферов
#define   FL_REG_STATUS_ERR_flags      0xFE      // маска, содержащая все разряды флагов ошибок


/// Поля конфигурационного регистра.
#define   FL_REG_CFG_type_man          3         // тип манипуляции, занимает 2 бита (0-1)
#define   fl_REG_CFG_ena_codec        (1 << 2)   // разрешение работы кодека Рида-Соломона
#define   fl_REG_CFG_ena_aru          (1 << 3)   // разрешение работы автоматической регулировки усиления
#define   fl_REG_CFG_ena_mem_vsk      (1 << 4)   // разрешение доступа к внутренней памяти функциональной логики канала ВСК
#define   fl_REG_CFG_rtavsk_ena       (1 << 6)   // разрешение использования адреса ОУ ВСК
#define   fl_REG_CFG_mode_rt_bc       (1 << 12)  // режим работы ВСК КШ/ОУ - 0, если КШ
#define   fl_REG_CFG_en_rt_bc_int     (1 << 13)  // разрешение выработки прерывания
#define   FL_REG_CFG_rtavsk           (0xF80)    // поле адреса оконечного устройства канала ВСК: биты 7-11

/// Значения полей конфигурационного регистра:
/// типы манипуляции
#define   val_REG_CFG_type_man_QPSK      0
#define   val_REG_CFG_type_man_QAM16     1
#define   val_REG_CFG_type_man_QAM64     2
#define   val_REG_CFG_type_man_ERROR     3
/// режим работы ВСК КШ/ОУ
#define   CFG_MODE_RT    1
#define   CFG_MODE_BC    0

#define AddrToCfg(addr)    (((addr) << 7) & FL_REG_CFG_rtavsk)


// Поля других регистров
#define   fl_REG_RX_CNTR_max_Rn_sync_pre   1
#define   fl_REG_RX_CNTR_prcs_max_sync     2

#define   fl_REG_CREG_com_res              1
#define   fl_REG_CREG_tx_res         (1 << 1)
#define   fl_REG_CREG_rx_res         (1 << 2)
#define   fl_REG_CREG_start_bc       (1 << 4)

#define   FL_REG_CR_SPI_spi_en             3
#define   val_REG_CR_SPI_spi_en_SPI_EN0    0
#define   val_REG_CR_SPI_spi_en_SPI_EN1    1
#define   val_REG_CR_SPI_spi_en_SPI_EN2    2
#define   val_REG_CR_SPI_spi_en_SPI_EN3    3
#define   SH_REG_CR_SPI_dr8_16_32          8
#define   FL_REG_CR_SPI_dr8_16_32    (3 << SH_REG_CR_SPI_dr8_16_32)
#define   val_REG_CR_SPI_dr_DEP8           0
#define   val_REG_CR_SPI_dr_DEP16    (1 << SH_REG_CR_SPI_dr8_16_32)
#define   val_REG_CR_SPI_dr_DEP32    (2 << SH_REG_CR_SPI_dr8_16_32)
#define   val_REG_CR_SPI_dr_UNDEF    (3 << SH_REG_CR_SPI_dr8_16_32)
#define   fl_REG_CR_SPI_spif         (1 << 15)

#define   fl_REG_AMPL_FACTOR_rx_ampl0      1
#define   fl_REG_AMPL_FACTOR_rx_ampl1      2
#define   fl_REG_AMPL_FACTOR_rx_ampl2      4
#define   fl_REG_AMPL_FACTOR_rx_ampl3      8
#define   FL_REG_AMPL_FACTOR_rx_ampl      15
#define   fl_REG_AMPL_FACTOR_rx_oe_ampl (1 << 4)

#define   FL_REG_PLL_ns                 0x3F
#define   FL_REG_PLL_ms                 0xFC0
#define   fl_REG_PLL_frange          (1 << 12)
#define   fl_REG_PLL_pd              (1 << 13)


/// Значения регистров после сброса.
#define   REG_VSK_VAL_time_rsp_RESET                 0xFFFF
#define   REG_VSK_VAL_g_sp_RESET                     0x1FDD
#define   REG_VSK_VAL_g_1_sp_high_RESET              0x4080
#define   REG_VSK_VAL_g_1_sp_low_RESET               0x48B9
#define   REG_VSK_VAL_pll_reg_RESET                  0x3044


/// Значение номера прошивки.
#define   REG_VSK_id_VER_0_0                  0xFDE6


/// Начальные настройки регистров.
#define  REG_VSK_ram_tx_rx_INIT_DEFAULT            0x0000
#define  REG_VSK_rx_cntr_INIT_DEFAULT              0x0000
#define  REG_VSK_creg_INIT_RESET   (fl_REG_CREG_com_res | fl_REG_CREG_tx_res | fl_REG_CREG_rx_res)
#define  REG_VSK_cr_spi_INIT_DEFAULT               0x0000
#define  REG_VSK_time_rsp_INIT_DEFAULT             0xC350
#define  REG_VSK_lvl_sync_kf_rx_msw_INIT_DEFAULT   0x0000
#define  REG_VSK_lvl_sync_kf_rx_lsw_INIT_DEFAULT   0x0200
#define  REG_VSK_lvl_sync_pre_rx_msw_INIT_DEFAULT  0x0040
#define  REG_VSK_lvl_sync_pre_rx_lsw_INIT_DEFAULT  0x0000
#define  REG_VSK_lvl_qam16_INIT_DEFAULT            0x1BBE
#define  REG_VSK_lvl_qam64_low_INIT_DEFAULT        0x0DF7
#define  REG_VSK_lvl_qam64_midle_INIT_DEFAULT      0x1BBE
#define  REG_VSK_lvl_qam64_high_INIT_DEFAULT       0x29E5
#define  REG_VSK_VAL_pll_reg_DEFAULT               0x3048
// счётчики
#define  REG_VSK_cnt_pct_tx_msw_INIT_0             0
#define  REG_VSK_cnt_pct_tx_lsw_INIT_0             0
#define  REG_VSK_cnt_pct_rx_msw_INIT_0             0
#define  REG_VSK_cnt_pct_rx_lsw_INIT_0             0


inline int manType(reg16_t val)
{
    return val & FL_REG_CFG_type_man;
}

inline int chgManType(reg16_t cfg, int man_type)
{
    return (cfg & ~FL_REG_CFG_type_man) | (man_type & FL_REG_CFG_type_man);
}

unsigned short calcNumWordInSymbol(int mt, bool codec);

inline unsigned int getBufTrm(int status) { return (status & fl_REG_STATUS_tx_num_buf) ? 0x80000 : 0x40000; }
inline unsigned int getBufRec(int status) { return (status & fl_REG_STATUS_rx_num_buf) ? 0xA0000 : 0x60000; }

inline unsigned int getBufAddrTrm(int num) { return num ? 0x80000 : 0x40000; }
inline unsigned int getBufAddrRec(int num) { return num ? 0xA0000 : 0x60000; }

#endif  // REGISTERS_H
