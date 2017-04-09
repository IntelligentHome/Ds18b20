#pragma once

#include "ITransport.h"
#include "IWait.h"

namespace ds18b20 {

#define DS18B20_INVALID 0x5555

union Temp {
    struct {
        uint8_t low_value;
        uint8_t high_value;
    };

    uint16_t value;

    uint8_t raw_data[2];
};

class Ds18b20 {

public:

    Ds18b20(
        transport::ITransport&  transport,
        iwait::IWait&           wait);

    Temp GetTemp(void);

private:

    transport::ITransport&  transport_;
    iwait::IWait&           wait_;
};

} /* namespace ds18b20 */
