#ifndef DATA_STRUCTS_SWITCH411_H
#define DATA_STRUCTS_SWITCH411_H

#include <QtGlobal>

///  АДРЕСА РЕГИСТРОВ

#define ADDR_REG_Switch411_RESET                 0x200
#define ADDR_REG_Switch411_OLD_TKPA              0x201
#define ADDR_REG_Switch411_CFG_SGVM502           0x202
#define ADDR_REG_Switch411_CFG_NRSHM             0x203
#define ADDR_REG_Switch411_DELAY_START           0x204
#define ADDR_REG_Switch411_CTRL                  0x205
#define ADDR_REG_Switch411_START_STOP            0x206
#define ADDR_REG_Switch411_STATUS                0x207
#define ADDR_REG_Switch411_COUNTER_TEST          0x208

class BaseSwitch411Reg
{
public:
    BaseSwitch411Reg(quint16 address) : address(address) {  }
    const quint16 address;
    void setData(quint16& data) {
        *((quint16*)this+1) = data;
    }
    quint16 getData() {
        return *((quint16*)this+1);
    }
};

//------------------------------ Switch411 --------------------------------------

class REG_Switch411_RESET : public BaseSwitch411Reg
{
public:
    REG_Switch411_RESET(quint16 reset = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_RESET),
        reset(reset) { }
    quint16 reset : 16;
};

class REG_Switch411_OLD_TKPA : public BaseSwitch411Reg
{
public:
    REG_Switch411_OLD_TKPA(quint16 old_tkpa = 0, quint16 reserve = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_OLD_TKPA),
        old_tkpa(old_tkpa), reserve(reserve) { }
    quint16 old_tkpa : 1;
    quint16 reserve : 15;

};

class REG_Switch411_CFG_SGVM502 : public BaseSwitch411Reg
{
public:
    REG_Switch411_CFG_SGVM502(quint16 rtadp = 0, quint16 rtad = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_CFG_SGVM502),
        rtadp(rtadp), rtad(rtad) { }
    quint16 rtadp : 1;
    quint16 rtad :  5;

};

class REG_Switch411_CFG_NRSHM : public BaseSwitch411Reg
{
public:
    REG_Switch411_CFG_NRSHM(quint16 measure_write_read = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_CFG_NRSHM),
        measure_write_read(measure_write_read) { }
    quint16 measure_write_read : 2;
};

class REG_Switch411_DELAY_START : public BaseSwitch411Reg
{
public:
    REG_Switch411_DELAY_START(quint16 delay_start = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_DELAY_START),
        delay_start(delay_start) { }
    quint16 delay_start : 16;
};

class REG_Switch411_CTRL : public BaseSwitch411Reg
{
public:
    REG_Switch411_CTRL(quint16 mem_reg = 0, quint16 exch_nrshm_sgvm502 = 0, quint16 high_adr_mem = 0, quint16 type_test = 0, quint16 type_test_command = 0, quint16 num_gvm_411 = 0, quint16 illegal_test = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_CTRL),
        mem_reg(mem_reg), exch_nrshm_sgvm502(exch_nrshm_sgvm502), high_adr_mem(high_adr_mem), type_test(type_test), type_test_command(type_test_command), num_gvm_411(num_gvm_411), illegal_test(illegal_test) { }
    quint16 : 1;
    quint16 mem_reg : 1;
    quint16 exch_nrshm_sgvm502 : 1;
    quint16 : 1;
    quint16 high_adr_mem : 3;
    quint16 : 1;
    quint16 type_test : 3;
    quint16 type_test_command : 1;
    quint16 num_gvm_411 : 2;
    quint16 : 1;
    quint16 illegal_test : 1;
};

class REG_Switch411_START_STOP : public BaseSwitch411Reg
{
public:
    REG_Switch411_START_STOP(quint16 start_stop_test = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_START_STOP),
        start_stop_test(start_stop_test) { }
    quint16 start_stop_test : 1;
};

class REG_Switch411_STATUS : public BaseSwitch411Reg
{
public:
    REG_Switch411_STATUS(quint16 test_complete = 0, quint16 test_error = 0, quint16 error_data = 0, quint16 error_fail_timeout = 0, quint16 error_exchange_nrshm_gvm = 0, quint16 tcs_err = 0, quint16 st_run = 0, quint16 st_err = 0, quint16 counter_command = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_STATUS),
        test_complete(test_complete), test_error(test_error), error_data(error_data), error_fail_timeout(error_fail_timeout), error_exchange_nrshm_gvm(error_exchange_nrshm_gvm), tcs_err(tcs_err), st_run(st_run), st_err(st_err), counter_command(counter_command) { }
    quint16 test_complete : 1;
    quint16 test_error : 1;
    quint16 error_data : 1;
    quint16 error_fail_timeout : 1;
    quint16 error_exchange_nrshm_gvm : 1;
    quint16 tcs_err : 1;
    quint16 st_run : 1;
    quint16 st_err : 1;
    quint16 counter_command : 3;
};

class REG_Switch411_COUNTER_TEST : public BaseSwitch411Reg
{
public:
    REG_Switch411_COUNTER_TEST(quint16 counter_test = 0) : BaseSwitch411Reg(ADDR_REG_Switch411_COUNTER_TEST),
        counter_test(counter_test) { }
    quint16 counter_test : 16;
};

#endif // DATA_STRUCTS_SWITCH411_H
