#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstddef>
#include <cstdint>
#include <vector>

using namespace std;

class MacSubheader
{
public:
    uint8_t r1 = 0;
    uint8_t r2_or_f = 0;
    uint8_t lcid = 0;
    uint16_t length = 0;
    bool has_length = true;

    uint8_t get_f_bit() const;
    vector<uint8_t> pack() const;

    size_t unpack(const uint8_t *buffer, std::size_t size,
                  bool expect_length = true);
};

#endif
