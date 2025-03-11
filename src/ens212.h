#ifndef SCIOSENSE_ENS212_H
#define SCIOSENSE_ENS212_H

#include "ens21x.h"

class ENS212 : public ENS21x
{
public:
    ENS212() : ENS21x()
    {
        debugPrefix = "ENS212 debug -- ";
    }

    bool isConnected() override
    {
        return Ens212_isConnected(this);
    }

    void begin(TwoWire* twoWire = &Wire, uint8_t address = 0x45) override
    {
        ENS21x::begin(twoWire, address);
    }
};

#endif //SCIOSENSE_ENS212_H