#ifndef WRITEDEVICEMEMORY_H
#define WRITEDEVICEMEMORY_H

#include "terasicdevice.h"
#include "extern_hsc.h"

class CWriteDeviceMemory
{
public:
    explicit CWriteDeviceMemory();
    ~CWriteDeviceMemory();

    void addReg(word32_t num, word32_t val);
    void addRamLSC(word32_t addr, word32_t val);
    void writeDev(TerasicDevice* dev, bool clean);
    void reset();

private:
    QByteArray addrvals;
};

#endif // WRITEDEVICEMEMORY_H
