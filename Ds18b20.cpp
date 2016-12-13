#include "Ds18b20.h"

namespace ds18b20 {

Ds18b20::Ds18b20(
        transport::ITransport&  transport,
        iwait::IWait&           wait)
    :
        transport_(transport),
        wait_(wait)
{
}

Temp Ds18b20::GetTemp(void) {

    Temp temp = { { 0 } };

    this->transport_.Reset();

    {
        uint8_t send_data[] = { 0xCC, 0x44 };
        this->transport_.Send(send_data, sizeof(send_data));
    }

    this->wait_.wait_ms(750);
    this->transport_.Reset();

    {
        uint8_t send_data[] = { 0xCC, 0xBE };
        this->transport_.Send(send_data, sizeof(send_data));
    }

    {
        uint8_t get_data[] = { 0x00, 0x00 };
        this->transport_.Get(get_data, sizeof(get_data));

        temp.high_value = get_data[0];
        temp.low_value = get_data[1];
    }

    this->transport_.Reset();

    return temp;
}

} /* namespace ds18b20 */
