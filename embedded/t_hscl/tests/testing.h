// -*- mode:c; coding:utf-8;
///
/// \file		testing.h
///
/// \brief 	Общие определения для файлов-тестов библиотеки t_hcsl
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

#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>

/// Коды ошибок.
#define test_Unknown_Error         41
#define test_Error_LoadSettings    42
#define test_Error_IdNumber        43
#define test_Error_WrongStatus     44
#define test_Error_Timeout         45
#define test_Error_RWPresence      46
//#define test_Error_      4
//#define test_Error_      4

enum command_type
{
    ct_NONE,  /// не выполнять команду
    ct_TRM,   /// обеспечить передачу num_chars байт данных ОУ->КШ
    ct_REC,   /// обеспечить передачу num_chars байт данных КШ->ОУ
    ct_CODE   /// сгенерировать и выполнить команду с заданными параметрами addr, size, tr и code
};

/// Настройки теста.
struct S_TestSettings
{
    int  version;               /// номер прошивки
    bool check_version;         /// проверять номер прошивки
    bool use_interruption;      /// использование прерывания для определения завершения обмена
    int  wait_time;             /// время ожидания завершения обмена (миллисекунд)

    char data_gen_type[4];      /// 3-буквенный идентификатор правила заполнения массива данных:
                                /// "000" - нули, "FFF" - 0xFFFFFFFF, "111" - 0x11111111, "Alt" - в байтах чередуются номера слов и смволов,
                                /// "Inc" или "INC" - числа по возрастанию, начиная с data_init_value (по умолчанию с 0),
                                /// "AAA" - 0xAAAAAAAA, "abc" - буквы алфавита маленькие, "555" - 0x55555555, "Num" - номер ячейки данных,
                                /// "FIX" или "Fix" - заполнение одним и тем же числом data_init_value, "ABC" - буквы алфавита большие,
                                /// "Def" - заполнение слов специальным значением 0x2114F001 - это заполнение используется
                                ///   по умолчанию, если тип заполнения не задан или определён некорректно
    int  data_unit_size;        /// размер одной ячейки данных - 1, 2 или 4 байта; если не определено, то для ct_CODE используется 4, а для остальных - 1
    int  data_init_value;       /// необязательный числовой параметр, который используется в правилах заполнения массива данных

    /// Тест поддерживает последовательный запуск нескольких команд.
    /// Параметры команд size, addr_rt и code и параметры теста use_interruption и wait_time для всех команд одинаковы.
    enum command_type command1;
    enum command_type command2;
    bool broadcast1;            /// вызов первой команды в групповом режиме
    bool broadcast2;            /// вызов второй команды в групповом режиме
    int repeat1;                /// число повторений первой команды (>=0), только при command1==ct_CODE
    int repeat2;                /// число повторений второй команды (>=0), только при command2==ct_CODE

    unsigned int addr_rt;       /// адрес ОУ
    unsigned int code;          /// код команды
    unsigned int num_sym;       /// число символов в командном слове, используется только в режиме ct_CODE
    unsigned int size_data;     /// размер передаваемых данных в байтах, используется только в режимах ct_TRM и ct_REC

    int  init_scenario;         /// выбор начальной конфигурации ВСК, пока доступна только одна конфигурация
    struct S_hscl_Config cfg_reg;   /// настройки конфигурационного регистра ВСК
};

/// Oбработка флага прерывания.

#ifndef MAIN_TEST_
extern
#endif
int interruption_flag;

int Test_IntFlagSetOff(void)
#ifdef MAIN_TEST_
{ interruption_flag = 0; return 0; }
#endif
;

int Test_IntFlagSetOn(void)
#ifdef MAIN_TEST_
{ interruption_flag = 1; return 1; }
#endif
;

bool Test_IntFlagCheck()
#ifdef MAIN_TEST_
{ if (interruption_flag) {
      interruption_flag = 0;
      return true;
  }
  return false;
}
#endif
;

#endif // TESTING_H
