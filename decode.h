#ifndef DECODE_H
#define DECODE_H

#include <iostream>

using namespace std;

class Decoder
{
public:
    virtual void decode();
    ~Decoder();
};

class MACdecode : public Decoder
{
};

class RLCdecode : public Decoder
{
};

class PDCPdecode : public Decoder
{
};

class RRCdecode : public Decoder
{
};

#endif