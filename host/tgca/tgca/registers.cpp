
#include "registers.h"
/*
inline word16_t manType(word16_t val)
{
    return val & FL_REG_CFG_type_man;
}

inline word16_t chgManType(word16_t cfg, int man_type)
{
    return (cfg & ~FL_REG_CFG_type_man) | (man_type & FL_REG_CFG_type_man);
}
*/

unsigned short calcNumWordInSymbol(int mt, bool codec)
{
    switch (mt)
    {
    case val_REG_CFG_type_man_QPSK:
        return codec ? 24 : 28;
    case val_REG_CFG_type_man_QAM16:
        return codec ? 44 : 56;
    case val_REG_CFG_type_man_QAM64:
        return codec ? 56 : 84;
    default:
        return 0;
    }
}

