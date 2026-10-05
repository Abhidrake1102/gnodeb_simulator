#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <vector>
#include <iostream>

class MacSubheader
{
public:
    uint8_t r1 = 0;
    uint8_t r2_or_f = 0;
    uint8_t lcid = 0;
    uint16_t length = 0;
    bool has_length = true;

    uint8_t get_f_bit() const
    {
        return (length > 255) ? 1 : 0;
    }

    std::vector<uint8_t> pack() const
    {
        std::vector<uint8_t> buffer;
        uint8_t first_byte = 0;

        if (has_length)
        {
            uint8_t f = get_f_bit();

            first_byte |= (r1 & 0x01) << 7;
            first_byte |= (f & 0x01) << 6;
            first_byte |= (lcid & 0x3F);
            buffer.push_back(first_byte);

            if (f == 0)
            {
                buffer.push_back(static_cast<uint8_t>(length & 0xFF));
            }
            else
            {
                buffer.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
                buffer.push_back(static_cast<uint8_t>(length & 0xFF));
            }
        }
        else
        {

            first_byte |= (r1 & 0x01) << 7;
            first_byte |= (r2_or_f & 0x01) << 6;
            first_byte |= (lcid & 0x3F);
            buffer.push_back(first_byte);
        }

        return buffer;
    }

    size_t unpack(const uint8_t *buffer, size_t size, bool expect_length = true)
    {
        if (size < 1)
            return 0;

        has_length = expect_length;
        uint8_t first_byte = buffer[0];

        r1 = (first_byte >> 7) & 0x01;
        r2_or_f = (first_byte >> 6) & 0x01;
        lcid = first_byte & 0x3F;

        if (!has_length)
        {
            length = 0;
            return 1;
        }

        uint8_t f = r2_or_f;
        if (f == 0)
        {
            if (size < 2)
                return 0;
            length = buffer[1];
            return 2;
        }
        else
        {
            if (size < 3)
                return 0;
            length = (static_cast<uint16_t>(buffer[1]) << 8) | buffer[2];
            return 3;
        }
    }
};

#endif
