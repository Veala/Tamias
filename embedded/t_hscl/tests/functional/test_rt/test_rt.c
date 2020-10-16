// -*- mode:c; coding:utf-8;
///
/// \file		test_rt.c
///
/// \brief 	Тестирование библиотеки t_hcsl
///             в режиме оконечного устройства
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

//#include "stdafx.h"

#include "hscl.h"
#include "hscl_reg.h"

#define MAIN_TEST_
#include "testing.h"
#undef MAIN_TEST_

#include "test_parameters_rt.h"

void loadSettings()
{
}


int main(int argc, char* argv[])
{
    struct S_TestSettings params;
    addr_t addr = 1;
    int iRet = test_Unknown_Error;

    word32_t command;
//    word32_t data[MAXPACKAGESIZE];
//    word32_t buffer[MAXPACKAGESIZE];

    unsigned int code = 99;
    unsigned int num_s =  1;

    char type = 'd';  /// default
    if (argc > 1)
        type = argv[1][0];



    if ((iRet = hscl_CheckDevice()) != hscl_OK)
        return iRet;

    if ((iRet = hscl_Configurate(hscl_Config_Default)) != hscl_OK)
        return iRet;
    
    // hscl_fl_REG_CFG_ena_codec | hscl_fl_REG_CFG_rtavsk_ena | hscl_fl_REG_CFG_en_rt_bc_int | hscl_val_REG_CFG_type_man_QPSK | flags hscl_REG_CFG_rt_bc
    if ((iRet = hscl_WriteConfigReg(params.cfg_reg)) != hscl_OK)
        return iRet;

    //FillData(data, 

    hscl_CreateCommandWord(&command, addr, num_s, hscl_tr_REC, code);
    //hscl_CreateCommandPack(buffer, command, data);

    hscl_Start();

    return 0;
}
