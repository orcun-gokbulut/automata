#ifndef _BIG_ENDIAN
	#ifdef _MSC_VER
		#include <stdlib.h>
		#define bswap_16(x) _byteswap_ushort(x)
		#define bswap_32(x) _byteswap_ulong(x)
		#define bswap_64(x) _byteswap_uint64(x)
	#else
		#include <byteswap.h>
	#endif
#else
	#define bswap_16(x) ((uint16)(x))
	#define bswap_32(x) ((uint32)(x))
	#define bswap_64(x) ((uint64)(x))
#endif