#ifndef SCIOSENSE_ENS213A_H
#define SCIOSENSE_ENS213A_H

#include "ens21x.h"

namespace ScioSense
{
    class ENS213A : public ENS21x
    {
    public:
        ENS213A() : ENS21x()
        {
            debugPrefix = "ENS213A debug -- ";
        }

        bool isConnected() override
        {
            return partId ==  0xa213;
        }

        bool begin(TwoWire& twoWire = Wire, uint8_t address= 0x46) override
        {
            return ENS21x::begin(twoWire, address);
        }
    };
}

#endif //SCIOSENSE_ENS213A_H