
#pragma once

/// Номера регистров, используемых в тестах проекта "Tamias"

#define  mr_NUMREG_CONFIG_1           0x01
#define  mr_NUMREG_CONFIG_2           0x02
#define  mr_NUMREG_COMMAND            0x03
//#define  mr_NUMREG_CONTROL_WORD       0x04

//#define  mr_NUMREG_INTERRUPT_STATUS   0x06
#define  mr_NUMREG_CONFIG_3           0x07
//#define  mr_NUMREG_CONFIG_4          0x08
#define  mr_NUMREG_CONFIG_5          0x09

// регистр длительности кадра КШ / регистр командного слова ОУ / регистр слова срабатывания режима «монитор слов»
#define  mr_NUMREG_BCFTIME_RTCW_WWMT  0x0D
//#define  mr_NUMREG_RT_STATUS_WORD     0x0E

#define  mr_NUMREG_EXT_RTA          0x60
#define  mr_NUMREG_EXT_BUS          0x62

/// Поля регистров

#define mr_REG_CFG1_DBL_RETRY_FLAG   0x0008
#define mr_REG_CFG1_RETRY_ENA_FLAG   0x0010
#define mr_REG_CFG1_BC_IMGTE_FLAG    0x0020

#define mr_REG_CFG1_RT_RTF_FLAG      0x0080
#define mr_REG_CFG1_RT_SF_FLAG       0x0100
#define mr_REG_CFG1_RT_SR_FLAG       0x0200
#define mr_REG_CFG1_RT_BUSY_FLAG     0x0400
#define mr_REG_CFG1_RT_DBCA_FLAG     0x0800
#define mr_REG_CFG1_RT_RW_ALL_FLAGS  \
  (mr_REG_CFG1_RT_RTF_FLAG | mr_REG_CFG1_RT_SF_FLAG | mr_REG_CFG1_RT_SR_FLAG | mr_REG_CFG1_RT_BUSY_FLAG | mr_REG_CFG1_RT_DBCA_FLAG)

#define mr_REG_CFG1_RTMT_MME_FLAG     0x1000

#define mr_REG_CFG1_MT_START_OT_FLAG  0x0400
#define mr_REG_CFG1_MT_STOP_OT_FLAG   0x0200

#define mr_REG_CFG1_CURR_AREA_FLAG    0x2000
// значения флага mr_REG_CFG1_CURR_AREA_FLAG
#define mr_REG_CFG1_CURR_AREA_A       0
#define mr_REG_CFG1_CURR_AREA_B       1

#define mr_REG_CFG2_RT_SBD_FLAG       0x0001
#define mr_REG_CFG2_RT_ERMM_FLAG      0x0002
#define mr_REG_CFG2_INT_SAC_FLAG      0x0010
#define mr_REG_CFG2_256_WBD_FLAG      0x0400
#define mr_REG_CFG2_RT_RSDBE_FLAG     0x1000
#define mr_REG_CFG2_RT_BLUTE_FLAG     0x2000
#define mr_REG_CFG2_ENCH_INT_FLAG     0x8000

#define mr_REG_CFG3_B_RX_TD_FLAG      0x0008
#define mr_REG_CFG3_ASWE_FLAG         0x0020
#define mr_REG_CFG3_ILL_DIS_FLAG      0x0080
#define mr_REG_CFG3_BT_CSZ_FLD        0x6000
#define mr_REG_CFG3_BT_CSZ_SHFT       13
#define mr_REG_CFG3_ENCH_FLAG         0x8000
// для режимов КШ/ОУ значения поля mr_REG_CFG3_BT_CSZ_FLD:
#define mr_STACK_SIZE_FLD_VAL_64      0x0000
#define mr_STACK_SIZE_FLD_VAL_128     0x2000
#define mr_STACK_SIZE_FLD_VAL_256     0x4000
#define mr_STACK_SIZE_FLD_VAL_512     0x6000

// поля регистра mr_NUMREG_CONTROL_WORD для режима КШ
#define mr_REG_CW_BC_RTRTF_FLAG       0x0001
#define mr_REG_CW_BC_BF_FLAG          0x0002
#define mr_REG_CW_BC_MCF_FLAG         0x0004
#define mr_REG_CW_BC_1553_ABS_FLAG    0x0008
#define mr_REG_CW_BC_EOM_IE_FLAG      0x0010
#define mr_REG_CW_BC_MBB_FLAG         0x0020
#define mr_REG_CW_BC_OLST_FLAG        0x0040
#define mr_REG_CW_BC_BC_A_B_FLAG      0x0080
#define mr_REG_CW_BC_CW_RE_FLAG       0x0100
#define mr_REG_CW_BC_RBM_FLAG         0x0200
#define mr_REG_CW_BC_TFBM_FLAG        0x0400
#define mr_REG_CW_BC_SFBM_FLAG        0x0800
#define mr_REG_CW_BC_SBBM_FLAG        0x1000
#define mr_REG_CW_BC_SRBM_FLAG        0x2000
#define mr_REG_CW_BC_MEM_FLAG         0x4000
// поля регистра mr_NUMREG_CONTROL_WORD для режима ОУ
#define mr_REG_CW_RT_BCST_MM_FLD    0x0007
#define mr_REG_CW_RT_BCST_CBI_FLAG    0x0008
#define mr_REG_CW_RT_BCST_EOMI_FLAG   0x0010
#define mr_REG_CW_RT_RX_MM_FLD      0x00E0
#define mr_REG_CW_RT_RX_CBI_FLAG      0x0100
#define mr_REG_CW_RT_RX_EOMI_FLAG     0x0200
#define mr_REG_CW_RT_TX_MN_FLD      0x1C00
#define mr_REG_CW_RT_TX_CBI_FLAG      0x2000
#define mr_REG_CW_RT_TX_EOMI_FLAG     0x4000
#define mr_REG_CW_RT_RX_DBE_FLAG      0x8000


#define mr_REG_RW_STATUS_RTA_FLD      0xF800   // Адрес абонента
#define mr_REG_RW_STATUS_BUSY_FLAG    0x0008   // Абонент занят

#define mr_REG_INT_STAT_EOM_FLAG      0x0001
#define mr_REG_INT_STAT_TTROL_FLAG    0x0040
#define mr_REG_INT_STAT_CDF_FLAG      0x0200  // RT MT Data Stack Frosen
#define mr_REG_INT_STAT_MT_CSR_FLAG   0x0800
#define mr_REG_INT_STAT_BT_CSR_FLAG   0x1000

#define mr_REG_COMMAND_RESET_FLAG     0x0001
#define mr_REG_COMMAND_START_FLAG     0x0002
#define mr_REG_COMMAND_INTERR_RESET_FLAG  0x0004
#define mr_REG_COMMAND_TTAG_RESET     0x0008
#define mr_REG_COMMAND_TTAG_TST_CLCK  0x0010
#define mr_REG_COMMAND_STOP_ON_F_FLAG 0x0020
#define mr_REG_COMMAND_STOP_ON_M_FLAG 0x0040
