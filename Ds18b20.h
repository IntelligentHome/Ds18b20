#pragma once

#include "ITransport.h"

namespace ds18b20 {

class Ds18b20 {

public:

    Ds18b20(transport::ITransport& transport);

private:

    transport::ITransport& transport_;
};

} /* namespace ds18b20 */
