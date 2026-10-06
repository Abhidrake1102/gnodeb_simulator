#include "Header.h"

#include <stdexcept>

using namespace std;

uint8_t MacSubheader::get_f_bit() const
{
    return length > 255 ? 1 : 0;
}

vector<uint8_t> MacSubheader::pack() const
{
    if (lcid > 63)
        throw invalid_argument("MAC LCID must fit in six bits");

    // Reserved bits are always transmitted as zero.
    vector<uint8_t> buffer;
    const uint8_t f = has_length ? get_f_bit() : 0;
    buffer.push_back(static_cast<uint8_t>((f << 6) | lcid));
    if (has_length)
    {
        if (f != 0)
            buffer.push_back(static_cast<uint8_t>(length >> 8));
        buffer.push_back(static_cast<uint8_t>(length & 0xFF));
    }
    return buffer;
}

size_t MacSubheader::unpack(const uint8_t *buffer, size_t size,
                            bool expect_length)
{
    if (buffer == nullptr || size == 0)
        return 0;

    const uint8_t second_bit = (buffer[0] >> 6) & 1;
    const std::size_t required = expect_length ? (second_bit ? 3 : 2) : 1;
    if (size < required)
        return 0;

    MacSubheader result;
    result.r1 = (buffer[0] >> 7) & 1;
    result.r2_or_f = second_bit;
    result.lcid = buffer[0] & 0x3F;
    result.has_length = expect_length;
    if (expect_length)
    {
        result.length = second_bit
                            ? static_cast<uint16_t>((static_cast<uint16_t>(buffer[1]) << 8) | buffer[2])
                            : buffer[1];
    }
    *this = result;
    return required;
}
