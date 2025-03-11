#ifndef SCIOSENSE_ENS215_H
#define SCIOSENSE_ENS215_H

#include "ens21x.h"

class ENS215 : public ENS21x
{
public:
    ENS215() : ENS21x()
    {
        debugPrefix = "ENS215 debug -- ";
    }

    bool isConnected() override
    {
        return Ens215_isConnected(this);
    }

    void begin(TwoWire* twoWire = &Wire, uint8_t address = 0x47) override
    {
        ENS21x::begin(twoWire, address);
    }
};

#endif //SCIOSENSE_ENS215_H