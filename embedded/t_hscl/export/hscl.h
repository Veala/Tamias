// -*- mode:c; coding:utf-8;
///
/// \file		hscl.h
///
/// \brief 		Заголовочный файл библиотеки программ для работы с логикой ВСК.
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

#ifndef HSCL_H
#define HSCL_H

#include <stddef.h>

// namespace HSCL
//{

/// Коды ошибок.
#define hscl_OK                    0
#define hscl_Error_Unknown         1
#define hscl_Error_DeviceNotFound  2
#define hscl_Error_SetFunction     3
#define hscl_Error_Configuration   4
#define hscl_Error_ReadReg         5
#define hscl_Error_WriteReg        6
#define hscl_Error_IllegalManType  7
#define hscl_Error_DataAbsent      8
#define hscl_Error_NegativeLength  9
#define hscl_Error_DataLong        10  /// предупреждение
#define hscl_Error_TooShortBuffer  11
#define hscl_Error_CreateCommand   12  /// предупреждение
//#define hscl_Error_        13
//#define hscl_Error_        14
//#define hscl_Error_        15
//#define hscl_Error_        16
//#define hscl_Error_        17
//#define hscl_Error_        18
//#define hscl_Error_        19
//#define hscl_Error_        20
//#define hscl_Error_        2
//#define hscl_Error_        2


/// Типы.
typedef unsigned short  addr_t;
typedef unsigned short  word16_t;
typedef unsigned int    word32_t;
typedef unsigned int*   p_mem_t;


/// Число слов, которое всегда отводится в пакете на 1 символ, независимо от реальной длины символа,
//   которая определяется кодеком и типом манипуляции.
#define NUMWORDINOFDMSYM   128
/// Максимальное число символов в пакете.   !!! Сейчас под вопросом, остается ли тоже 254 символа в режиме ena_aru
#define MAXNUMSYMBINPACK   254
#define MAXNUMSYM   256
/// Число преамбул в пакете с ena_aru == 0 (при установленном признаке ena_aru число преамбул увеличивается на 1).
#define MINNUMPREAMBULES   2
/// Размер памяти в байтах, достаточный для максимального пакета данных (256 символов по 128 32-разрядных слов).
#define MAXPACKAGESIZE  (MAXNUMSYM * NUMWORDINOFDMSYM * sizeof(word32_t))   /// 0x8000
/// Размер одного OFDM символа в байтах - 0x200 (128 32-разрядных слов)
#define NUMBYTEINOFDMSYM (NUMWORDINOFDMSYM * sizeof(word32_t))

/// Адрес ОУ для групповой команды.
#define BRD_RT_ADDR  31
/// Максимальный рарешенный адрес ОУ - 5 бит
#define MAX_RT_ADDR  31
/// Максимальный рарешенный код команды - 7 бит
#define MAX_COMMAND_CODE  127

// признак trbit: направление передачи данных
#define  hscl_tr_REC  0   // OY принимает
#define  hscl_tr_TRM  1   // ОУ передает

/// режим работы ВСК КШ/ОУ
#define   hscl_mode_RT    1 
#define   hscl_mode_BC    0 

/// Настройки, задающие начальную конфигурвцию регистров.
/// Это могут быть ключевые слова, задающие набор входных параметров конфигурации (инициализации),
/// или слова, содержащие битовую маску.
#define hscl_Config_Default  0

/// Биты ответного слова.
#define  hscl_fl_SB_NOTRDY   1
#define  hscl_fl_SB_DEVERR   2
#define  hscl_fl_SB_DATERR   4

/// Признак trbit: направление передачи данных.
//   бит tr в командном слове
#define  hscl_fl_CW_tr_bit  (1 << 7)
//   значения бита tr в командном слове
#define  hscl_val_CW_tr_REC  (hscl_fl_CW_tr_bit * hscl_tr_REC) // ОУ принимает
#define  hscl_val_CW_tr_TRM  (hscl_fl_CW_tr_bit * hscl_tr_TRM) // ОУ передает
//   извлечение признака tr из командного слова
//#define  hscl_tr_bit_VAL(cw) (((cw) & hscl_CW_tr_bit) >> 7)

/// Карта внешней памяти
#define BUFF_0_TRM  0x00000
#define BUFF_1_TRM  0x08000
#define BUFF_0_REC  0x10000
#define BUFF_1_REC  0x18000

////=====================================================================================////

struct S_hscl_Status
{
    // все поля принимают значения 0 или 1
    int  rt_bc_int;      // флаг прерывания
    int  rs_err;         // флаг ошибки кодека Рида - Соломона
    int  no_aw_err;      // флаг ошибки отсутствия ответного пакета на негрупповую команду
    int  yes_aw_gr_err;  // флаг ошибки получения ответного пакета на групповую команду
    int  tx_num_buf;     // номер активного буфера передатчика во внешней памяти
    int  rx_num_buf;     // номер активного буфера приёмника во внешней памяти
};

struct S_hscl_Config
{
    int  type_man;       // тип манипуляции: 0 - 2
    int  ena_codec;      // разрешение работы кодека Рида - Соломона
    int  ena_aru;        // разрешение работы автоматической регулировки усиления 
    int  ena_mem_vsk;    // разрешение доступа к внутренней памяти ФЛ ВСК
    int  rtavsk_ena;     // разрешение использования адреса ОУ из признака rtavsk
    int  rtavsk;         // адрес ОУ ВСК: 0 - 31
    int  rt_bc;          // режим работы ВСК ОУ/КШ
    int  en_rt_bc_int;   // разрешение выработки прерывания
};

////=====================================================================================////

/// Проверка подключения устройства.
int hscl_CheckDevice();           /// hscl_OK - устройство найдено, hscl_Error_DeviceNotFound - устройство не найдено

/// Начальная настройка. Не заполняет конфигурационный регистр.
int hscl_Configurate(int param);  /// hscl_OK - успешно, иначе - ошибка конфигурации: hscl_Error_Configuration или другая
    
/// Вызов аппаратного сброса.
void hscl_Reset();

// int hscl_WriteMem(p_mem_t buff_dst, p_mem_t buff_src, size_t size);  /// буфер -> устройство
// int hscl_ReadMem(p_mem_t buff_dst, addr_t adr, size_t size);   /// устройство -> буфер

/// Переписать num_s символов OFDM с адреса mem_src в активный буфер передатчика ВСК.
int hscl_WriteMem(void* mem_src, unsigned int num_s);
/// Скопировать содержимое активного буфера приёмника ВСК в mem_dst.
int hscl_ReadMem(void* mem_dst);

/// Чтение/запись регистров
/// Возвращает hscl_OK или код ошибки
int hscl_WriteReg(word16_t data, unsigned short reg_num);      /// 16-разрядное слово -> регистр
int hscl_ReadReg(word16_t* data, unsigned short reg_num);      /// регистр -> 16-разрядное слово
int hscl_ReadConfigReg(word16_t* data);                        /// конфигурационный регистр -> data
int hscl_WriteConfigReg(struct S_hscl_Config cfg);       /// формирует и записывает конфигурационный регистр

/// Получение номера прошивки.
int hscl_GetID();

/// Формирование командного слова.
/// Функция проверяет входные параметры addr (адрес ОУ), num_s (число OFDM символов), tr (направление передачи данных) и code (код команды)
///  на допустимые значения и формирует в command_word командное слово.
/// Если значения входных параметров правильные, то функция возвращает hscl_OK.
/// Иначе функция приводит все значения к допустимым, формирует из получившихся значений командное слово и возвращает hscl_Error_CreateCommand.
/// Недопустимые addr, num_s и code обрезаются до допустимых (до нужного количества бит), а признак tr считается 0 или не 0.
int hscl_CreateCommandWord(word32_t* command_word, addr_t addr, unsigned int num_s, int tr, unsigned int code);

/// Создание командного пакета.
/// Функция формирует пакет, состоящий из 2 или 3 преамбул и нужного количества OFDM символов, и копирует его в буфер назначения.
/// В нулевом слове нулевого символа функция формирует командное слово.
/// Для команды приёма (т.е. size_src > 0 и tr == hscl_val_CW_tr_REC) количество символов OFDM вычисляется в зависимости от настроек устройства.
/// Если команда не является командой приёма (size_src == 0 или tr != hscl_val_CW_tr_REC), создается один (нулевой) символ OFDM.
/// Параметры:
///    mem_dst -  буфер назначения, должен быть создан заранее и иметь достаточный размер;
///    size_dst - размер буфера назначения в байтах, при заполнении данными будет проверяться на переполнение,
///               NUMBYTEINOFDMSYM - минимальный допустимый размер, MAXPACKAGESIZE - размер, достаточный для максимального пакета;
///    mem_src  - указатель на исходные данные, может быть NULL при num_s == 0 или tr != 0;
///    size_src - число байтов для приёма/передачи; функция вычисляет количество символов OFDM, которое потребуется, и проверяет на максимальное значение;
///    addr  - адрес ОУ (0 - 30) или адрес групповой команды 31;
///    tr    - направление передачи (для команды приёма - hscl_tr_REC, передачи - hscl_tr_TRM);
///    code  - код команды, при size_src==0 игнорируется.
/// Возвращаемое значение: hscl_OK или код ошибки; в случае ошибки содержимое mem_dst может оказаться произвольным.
int hscl_CreateCommandPack(void* mem_dst, size_t size_dst, void* mem_src, size_t size_src, addr_t addr, int tr, unsigned int code);

int hscl_Start();
void hscl_Stop();

/// Установка пользовательской функции обработки прерывания.
/// Возвращает hscl_OK или код ошибки.
int hscl_SetProcIntBC(int(*pF)(void));
int hscl_SetProcIntRT(int(*pF)(void));

/// Функция запрашивает регистр статуса и возвращает его значение, если регистр содержит ненулевые флаги.
/// При этом флаги в самом регистре сбрасывается.
word16_t CatchStatusChange();

/// Установка и получение типа манипуляции.
int hscl_SetManType(int man_type); // записывает man_type в регистр конфигурации
int  hscl_GetManType();      // запрашивает тип манипуляции из регистра; в случае ошибки возвращает hscl_val_REG_CFG_type_man_ERROR
int hscl_TypeMan(char* type_man, char* default_on_error);  // вход: "QPSK", "QAM16" или "QAM64"; в случае ошибки возвращает hscl_val_REG_CFG_type_man_QPSK

/// Определение .
/// Функция запрашивает признаки из конфигурационного регистра и вычисляет количество ОФДМ символов,
//   требуемых для передачи сообщения.
/// Входные параметры:
//           num_byte - длина сообщения в байтах.
/// Выходные параметры:
//   num_sym - число, на 1 меньшее количества символов OFDM, требуемых для передачи КС и num_byte байт данных;
//   sym_len - размер OFDM символа в байтах.
/// Возвращаемое значение:
//   hscl_OK - нет ошибки и сообщение помещается в один пакет;
//   hscl_Error_LongData - нет ошибки, но для передачи сообщения требуется передача более одного пакета
//    (в этом случае количество символов num_sym больше длины пакета, это не является ошибкой);
//   код ошибки в остальных случаях.
/// В случае ошибки num_sym содержит -1, а значение выходного параметра sym_len не определено.
/// 
int hscl_NumSymOFDM(int num_byte, int* num_sym, int* sym_len);

/// Функция запрашивает заданные тип манипуляции и установку кодека Рида-Соломона
/// и возвращает длину символа OFDM (число 32-разрядных слов).
size_t hscl_NumWordInSymbol_0p();

/// Функция возвращает длину символа OFDM (число 32-разрядных слов).
/// Входной параметр cfg - содержимое конфигурационного регистра.
size_t hscl_NumWordInSymbol_1p(word16_t cfg);

/// Функция возвращает длину символа OFDM (число 32-разрядных слов) при заданных типе манипуляции mt (0-2)
/// и признаке использования кодека Рида-Соломона ena_codec (0 или 1).
size_t hscl_NumWordInSymbol_2p(int mt, int ena_codec);

/// Разрешение работы автоматической регулировки усиления в приемной части (== формирование третьей преамбулы).
/// Входной параметр cfg - содержимое конфигурационного регистра.
/// Возвращает бит hscl_fl_REG_CFG_ena_aru.
int hscl_CheckEnaAru(word16_t cfg);

/// Возвращаеи максимальное число байт, которое поместится в один пакет, с учетом текущих настроек.
size_t hscl_MaxNumByte();



//} // namespace HSCL

#endif   // HSCL_H
