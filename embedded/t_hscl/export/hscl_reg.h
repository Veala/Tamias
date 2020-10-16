// -*- mode:c; coding:utf-8;
///
/// \file		hscl_reg.h
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

#ifndef HSCL_REG_H
#define HSCL_REG_H

///  НОМЕРА РЕГИСТРОВ
#define   hscl_NUMREG_ram_tx_rx             0x20
//                                         0x21
#define   hscl_NUMREG_id                    0x22   // только чтение
#define   hscl_NUMREG_status                0x23   // только чтение
#define   hscl_NUMREG_cfg                   0x24
#define   hscl_NUMREG_tx_cntr               0x25
//                                         0x26
#define   hscl_NUMREG_rx_cntr               0x27
#define   hscl_NUMREG_creg                  0x28
#define   hscl_NUMREG_cr_spi                0x29
#define   hscl_NUMREG_dr_spi_msw            0x2A
#define   hscl_NUMREG_dr_spi_lsw            0x2B
//                                         0x2C
#define   hscl_NUMREG_time_rsp              0x2D
#define   hscl_NUMREG_cnt_pct_tx_msw        0x2E
#define   hscl_NUMREG_cnt_pct_tx_lsw        0x2F
#define   hscl_NUMREG_cnt_pct_rx_msw        0x30
#define   hscl_NUMREG_cnt_pct_rx_lsw        0x31

#define   hscl_NUMREG_lvl_sync_kf_rx_msw    0x32   // чтение/запись
#define   hscl_NUMREG_prcs_max_sync_msw     0x32   // только чтение

#define   hscl_NUMREG_lvl_sync_kf_rx_lsw    0x33   // чтение/запись
#define   hscl_NUMREG_prcs_max_sync_lsw     0x33   // только чтение

#define   hscl_NUMREG_lvl_sync_pre_rx_msw   0x34   // чтение/запись
#define   hscl_NUMREG_prs_level_max_rn_msw  0x34   // только чтение

#define   hscl_NUMREG_lvl_sync_pre_rx_lsw   0x35   // чтение/запись
#define   hscl_NUMREG_prs_level_max_rn_lsw  0x35   // только чтение

#define   hscl_NUMREG_lvl_qam16             0x36
#define   hscl_NUMREG_lvl_qam64_low         0x37
#define   hscl_NUMREG_lvl_qam64_midle       0x38
#define   hscl_NUMREG_lvl_qam64_high        0x39
#define   hscl_NUMREG_amplification_factor  0x3A
#define   hscl_NUMREG_amplitude_signal      0x3B   // только чтение
#define   hscl_NUMREG_g_sp                  0x3C
#define   hscl_NUMREG_g_1_sp_high           0x3D
#define   hscl_NUMREG_g_1_sp_low            0x3E
#define   hscl_NUMREG_pll_reg               0x3F


#define hscl_ReadStatusReg(data)   hscl_ReadReg(data, hscl_NUMREG_status)
#define hscl_WriteConfigReg_(data) hscl_WriteReg(data, hscl_NUMREG_cfg)
word16_t hscl_CreateConfigReg(struct S_hscl_Config cfg);       /// возвращает образ конфигурационного регистра

/// Разряды регистра статуса.
#define   hscl_fl_REG_STATUS_rt_bc_int      1         // флаг прерывания
#define   hscl_fl_REG_STATUS_rs_err        (1 << 4)   // флаг ошибки кодека Рида - Соломона
#define   hscl_fl_REG_STATUS_no_aw_err     (1 << 5)   // флаг ошибки отсутствия ответного пакета на негрупповую команду
#define   hscl_fl_REG_STATUS_yes_aw_gr_err (1 << 6)   // флаг ошибки получения ответного пакета на групповую команду
#define   hscl_fl_REG_STATUS_tx_num_buf    (1 << 10)  // номер активного буфера передатчика во внешней памяти
#define   hscl_fl_REG_STATUS_rx_num_buf    (1 << 11)  // номер активного буфера приёмника во внешней памяти
#define   hscl_FL_REG_STATUS_flags          0xFF      // маска, содержащая все разряды флагов, но исключающая номера активных буферов


/// Поля конфигурационного регистра.
#define   hscl_FL_REG_CFG_type_man          3         // тип манипуляции, занимает 2 бита (0-1)
#define   hscl_fl_REG_CFG_ena_codec        (1 << 2)   // разрешение работы кодека Рида-Соломона
#define   hscl_fl_REG_CFG_ena_aru          (1 << 3)   // разрешение работы автоматической регулировки усиления
#define   hscl_fl_REG_CFG_ena_mem_vsk      (1 << 4)   // разрешение доступа к внутренней памяти функциональной логики канала ВСК
#define   hscl_fl_REG_CFG_rtavsk_ena       (1 << 6)   // разрешение использования адреса ОУ ВСК 
#define   hscl_fl_REG_CFG_mode_rt_bc       (1 << 12)  // режим работы ВСК КШ/ОУ - 0, если КШ
#define   hscl_fl_REG_CFG_en_rt_bc_int     (1 << 13)  // разрешение выработки прерывания
#define   hscl_FL_REG_CFG_rtavsk           (0xF80)    // поле адреса оконечного устройства канала ВСК: биты 7-11

/// Значения полей конфигурационного регистра:
/// типы манипуляции
#define   hscl_val_REG_CFG_type_man_QPSK      0
#define   hscl_val_REG_CFG_type_man_QAM16     1
#define   hscl_val_REG_CFG_type_man_QAM64     2
#define   hscl_val_REG_CFG_type_man_ERROR     3
/// режим работы ВСК КШ/ОУ
#define   hscl_val_REG_CFG_mode_RT    hscl_fl_REG_CFG_mode_rt_bc 
#define   hscl_val_REG_CFG_mode_BC    0 


// Поля других регистров
#define   hscl_fl_REG_RX_CNTR_max_Rn_sync_pre   1
#define   hscl_fl_REG_RX_CNTR_prcs_max_sync     2

#define   hscl_fl_REG_CREG_com_res              1
#define   hscl_fl_REG_CREG_tx_res         (1 << 1)
#define   hscl_fl_REG_CREG_rx_res         (1 << 2)
#define   hscl_fl_REG_CREG_start_bc       (1 << 4)

#define   hscl_FL_REG_CR_SPI_spi_en             3
#define   hscl_val_REG_CR_SPI_spi_en_SPI_EN0    0
#define   hscl_val_REG_CR_SPI_spi_en_SPI_EN1    1
#define   hscl_val_REG_CR_SPI_spi_en_SPI_EN2    2
#define   hscl_val_REG_CR_SPI_spi_en_SPI_EN3    3
#define   hscl_FL_REG_CR_SPI_dr8_16_32    (3 << 8)
#define   hscl_val_REG_CR_SPI_dr_DEP8           0
#define   hscl_val_REG_CR_SPI_dr_DEP16    (1 << 8)
#define   hscl_val_REG_CR_SPI_dr_DEP32    (2 << 8)
#define   hscl_val_REG_CR_SPI_dr_UNDEF    (3 << 8)
#define   hscl_fl_REG_CR_SPI_spif         (1 << 15)

#define   hscl_FL_REG_AMPL_FACTOR_rx_ampl     0xF
#define   hscl_fl_REG_AMPL_FACTOR_rx_oe_ampl (1 << 4)

#define   hscl_FL_REG_PLL_ns                 0x3F
#define   hscl_FL_REG_PLL_ms                 0xFC0
#define   hscl_fl_REG_PLL_frange          (1 << 12)
#define   hscl_fl_REG_PLL_pd              (1 << 13)


/// Значения регистров после сброса.
#define   hscl_REG_time_rsp_RESET                 0xFFFF
#define   hscl_REG_g_sp_RESET                     0x1FDD
#define   hscl_REG_g_1_sp_high_RESET              0x4080
#define   hscl_REG_g_1_sp_low_RESET               0x48B9
#define   hscl_REG_pll_RESET                      0x1004


/// Значение номера прошивки.
#define   hscl_NUMREG_id_VER_0_0                  0xFDE6


/// Начальные настройки регистров.
#define  hscl_NUMREG_ram_tx_rx_INIT_DEFAULT            0x0000
#define  hscl_NUMREG_rx_cntr_INIT_DEFAULT              0x0000
#define  hscl_NUMREG_creg_INIT_RESET   (hscl_fl_REG_CREG_com_res | hscl_fl_REG_CREG_tx_res | hscl_fl_REG_CREG_rx_res)
#define  hscl_NUMREG_cr_spi_INIT_DEFAULT               0x0000
#define  hscl_NUMREG_time_rsp_INIT_DEFAULT             0xC350
#define  hscl_NUMREG_lvl_sync_kf_rx_msw_INIT_DEFAULT   0x0000
#define  hscl_NUMREG_lvl_sync_kf_rx_lsw_INIT_DEFAULT   0x0200
#define  hscl_NUMREG_lvl_sync_pre_rx_msw_INIT_DEFAULT  0x0040
#define  hscl_NUMREG_lvl_sync_pre_rx_lsw_INIT_DEFAULT  0x0000
#define  hscl_NUMREG_lvl_qam16_INIT_DEFAULT            0x1BBE
#define  hscl_NUMREG_lvl_qam64_low_INIT_DEFAULT        0x0DF7
#define  hscl_NUMREG_lvl_qam64_midle_INIT_DEFAULT      0x1BBE
#define  hscl_NUMREG_lvl_qam64_high_INIT_DEFAULT       0x29E5
// счётчики
#define  hscl_NUMREG_cnt_pct_tx_msw_INIT_0             0
#define  hscl_NUMREG_cnt_pct_tx_lsw_INIT_0             0
#define  hscl_NUMREG_cnt_pct_rx_msw_INIT_0             0
#define  hscl_NUMREG_cnt_pct_rx_lsw_INIT_0             0

#endif  // HSCL_REG
