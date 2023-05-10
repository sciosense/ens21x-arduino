#ifndef SCIOSENSE_ENS211_H
#define SCIOSENSE_ENS211_H

#include "ens21x.h"

namespace ScioSense
{
    class ENS211 : public ENS21x
    {
    public:
        ENS211() : ENS21x()
        {
            debugPrefix = "ENS211 debug -- ";
        }

        bool isConnected() override
        {
            return partId ==  0x0211;
        }

        bool begin(TwoWire& twoWire = Wire, uint8_t address= 0x44) override
        {
            return ENS21x::begin(twoWire, address);
        }
    };
}

#endif //SCIOSENSE_ENS211_H