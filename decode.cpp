#include "decode.h"
using namespace std;

size_t Decoder::decode(const uint8_t *buffer, size_t size,
                       bool expect_length)
{
    decoded_ = false;
    mac_header_ = MacSubheader{};
    const size_t consumed = mac_header_.unpack(buffer, size, expect_length);
    decoded_ = consumed != 0;
    return consumed;
}
