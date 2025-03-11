#ifndef SCIOSENSE_ENS211_H
#define SCIOSENSE_ENS211_H

#include "ens21x.h"

class ENS211 : public ENS21x
{
public:
    ENS211() : ENS21x()
    {
        debugPrefix = "ENS211 debug -- ";
    }

    bool isConnected() override
    {
        return Ens211_isConnected(this);
    }

    void begin(TwoWire* twoWire = &Wire, uint8_t address = 0x44) override
    {
        ENS21x::begin(twoWire, address);
    }
};

#endif //SCIOSENSE_ENS211_H