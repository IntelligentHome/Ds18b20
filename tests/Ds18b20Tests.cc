#include "gtest/gtest.h"
#include "Ds18b20.h"
#include <vector>

namespace test_Ds18b20 {

enum DataType {
    TYPE_TIME_US = 0,
    TYPE_TIME_MS,
    TYPE_TRANSPORT_SEND,
    TYPE_TRANSPORT_GET,
    TYPE_TRANSPORT_RESET,
};

class TestStruct {
public:
    TestStruct(DataType type, int value)
        :
            type_(type),
            value_(value)
    {}

    DataType type_;
    int value_;
};

std::vector<std::unique_ptr<TestStruct>> recv_data;

class Ds18b20TransportMock : public transport::ITransport {

public:

    Ds18b20TransportMock(std::vector<uint8_t>& get_data)
        :
            get_data_(get_data)
    {}

    virtual uint8_t Reset(void) {
        recv_data.push_back(std::unique_ptr<TestStruct>(new TestStruct(TYPE_TRANSPORT_RESET, 0)));
        return 0;
    }

    virtual void Send(uint8_t send_buff[], uint16_t size) {
        for (int i = 0; i < size; i++)
            recv_data.push_back(
                    std::unique_ptr<TestStruct>(new TestStruct(TYPE_TRANSPORT_SEND, send_buff[i])));
    }

    virtual void Get(uint8_t recv_buff[], uint16_t size) {
        for (int i = 0; i < size; i++) {
            assert(!get_data_.empty());
            recv_buff[i] = get_data_.back();
            get_data_.pop_back();
            recv_data.push_back(
                    std::unique_ptr<TestStruct>(new TestStruct(TYPE_TRANSPORT_GET, 0)));
        }
    }

    virtual void SendAndGet(uint8_t send_buff[], uint8_t recv_buff[], uint16_t size) {

    }

    std::vector<uint8_t>& get_data_;
};

class Ds18b20WaitMock : public iwait::IWait {

public:
    virtual void wait_us(uint16_t time) {
        recv_data.push_back(std::unique_ptr<TestStruct>(new TestStruct(TYPE_TIME_US, time)));
    }

    virtual void wait_ms(uint16_t time) {
        recv_data.push_back(std::unique_ptr<TestStruct>(new TestStruct(TYPE_TIME_MS, time)));
    }
};

TEST(Ds18b20, GetTemp) {

    std::vector<TestStruct> expected_recv {
        TestStruct(TYPE_TRANSPORT_RESET, 0),

        TestStruct(TYPE_TRANSPORT_SEND, 0xCC),
        TestStruct(TYPE_TRANSPORT_SEND, 0x44),

        TestStruct(TYPE_TIME_MS, 750),

        TestStruct(TYPE_TRANSPORT_RESET, 0),

        TestStruct(TYPE_TRANSPORT_SEND, 0xCC),
        TestStruct(TYPE_TRANSPORT_SEND, 0xBE),
        TestStruct(TYPE_TRANSPORT_GET, 0),
        TestStruct(TYPE_TRANSPORT_GET, 0),

        TestStruct(TYPE_TRANSPORT_RESET, 0)
    };

    std::vector<uint8_t> get_data { 0x33, 0x99 };
    std::reverse(get_data.begin(), get_data.end());

    Ds18b20WaitMock wait;
    Ds18b20TransportMock transport(get_data);

    ds18b20::Ds18b20 thermometer(transport, wait);

    recv_data.clear();

    ds18b20::Temp temp = thermometer.GetTemp();

    EXPECT_TRUE(get_data[0] == temp.low_value);
    EXPECT_TRUE(get_data[1] == temp.high_value);

    EXPECT_TRUE(expected_recv.size() == recv_data.size());

    for (int i = 0; i < expected_recv.size() && i < recv_data.size(); i++) {
        EXPECT_TRUE(expected_recv[i].type_ == recv_data[i]->type_);
        EXPECT_TRUE(expected_recv[i].value_ == recv_data[i]->value_);
    }
}

}; /* namespace test_Bs18b20 */
