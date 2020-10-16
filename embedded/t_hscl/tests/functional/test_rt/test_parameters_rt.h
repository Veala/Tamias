// -*- mode:c; coding:utf-8;
///
/// \file		test_parameters_rt.h
///
/// \brief 	    Настраиваемые параметры команд для тестов библиотеки t_hcsl
/// \brief 	    Файл содержит параметры по умолчанию для тестов ВСК в режиме ОУ
/// \details
///
/// \copyright  ЗАО НТЦ "Модуль"
/// \author     Мисюрёва Е.В.
///
/// \date 		22.02.2018
/// \version 	0.1
///  
///  История изменений:
///  
///

#ifndef TEST_PARAMETRS_RT_H
#define TEST_PARAMETRS_RT_H

/// НАСТРОЙКИ ТЕСТА В РЕЖИМЕ ОУ

/// Параметры команды.
#define NUM_SYM  1             /// число передаваемых символов (>=1)
#define COMMAND_CODE 99        /// код команды (не используется устройством, если NUM_SYM != 0
#define MANIPULATION "QPSK"    /// "QPSK" (по умолчанию), "QAM16" или "QAM64"
#define CODEC  1               /// использовать кодек Рида-Соломона
#define DATA_DIRECTION "REC"   /// должно содержать "REC", или "TRM", или оба; если не содержит ни то, ни другое,
                               /// то будет тестироваться команда с кодом COMMAND_CODE и числом символов 0;
                               /// если задано и TRM, и REC, то все остальные настройки используются для обеих команд
#define DEV_ADDR  1            /// адрес ОУ, которому предназначены команды; разрешается использовать адрес 31
#define BROADCAST "NO"         ///

/// Параметры конфигурации устройства.
#define INTERRUPTION "NO"      /// 


/// Следующие параметры пока останутся фиксированными.
#define USE_DEV_ADDR "YES"      ///

#endif // TEST_PARAMETRS_RT_H