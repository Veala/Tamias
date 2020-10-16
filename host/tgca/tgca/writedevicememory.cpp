#include "writedevicememory.h"

CWriteDeviceMemory::CWriteDeviceMemory()
{
}

CWriteDeviceMemory::~CWriteDeviceMemory()
{
    addrvals.clear();
}

void CWriteDeviceMemory::addReg(word32_t num, word32_t val)
{
    word32_t addr = num*4;
    addrvals.append((char*)&addr, 4);
    addrvals.append((char*)&val, 4);
}

void CWriteDeviceMemory::addRamLSC(word32_t addr, word32_t val)
{
    word32_t real_addr = 0x4000 + addr*4;
    addrvals.append((char*)&real_addr, 4);
    addrvals.append((char*)&val, 4);
}

void CWriteDeviceMemory::writeDev(Device* dev, bool clean)
{
    if (!addrvals.isEmpty())
    {
        dev->write_F1(addrvals.data(), addrvals.size());
        if (clean)
            addrvals.clear();
    }
}

void CWriteDeviceMemory::reset()
{
    addrvals.clear();
}
