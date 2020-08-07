#include "AEndian.h"

uint16 bswap_16(uint16 value)
{
    #ifndef _BIG_ENDIAN
        return (value >> 8) | (value << 8);
    #else
        return value;
    #endif
}

uint32 bswap_32(uint32 value)
{
    #ifndef _BIG_ENDIAN
        return (bswap_16(value & 0xffff) << 16) | (bswap_16(value >> 16));
    #else
        return value;
    #endif
}

uint64 bswap_64(uint64 value)
{
    #ifndef _BIG_ENDIAN
        return (((uint64)bswap_32(value & 0xffffffffull)) << 32) | (bswap_32(value >> 32));
    #else
        return value;
    #endif
}
