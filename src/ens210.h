#ifndef SCIOSENSE_ENS210_H
#define SCIOSENSE_ENS210_H

#include "ens21x.h"

class ENS210 : public ENS21x
{
public:
    ENS210() : ENS21x()
    {
        debugPrefix = "ENS210 debug -- ";
    }

    bool isConnected() override
    {
        return Ens210_isConnected(this);
    }

    void begin(TwoWire* twoWire = &Wire, uint8_t address = 0x43) override
    {
        ENS21x::begin(twoWire, address);
    }
};

#endif //SCIOSENSE_ENS210_H