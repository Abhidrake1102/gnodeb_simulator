#ifndef DECODE_H
#define DECODE_H

#include "Header.h"
using namespace std;

class Decoder
{
public:
    size_t decode(const uint8_t *buffer, size_t size,
                  bool expect_length = true);
    // Currently means one MAC subheader was decoded successfully.
    bool isDecoded() const { return decoded_; }
    const MacSubheader &macHeader() const { return mac_header_; }

private:
    MacSubheader mac_header_;
    bool decoded_ = false;
};

#endif
