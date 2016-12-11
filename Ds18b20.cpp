#include "Ds18b20.h"

namespace ds18b20 {

Ds18b20::Ds18b20(transport::ITransport& transport)
    :
        transport_(transport)
{
}

} /* namespace ds18b20 */
