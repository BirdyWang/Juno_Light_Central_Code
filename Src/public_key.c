
/* This file was automatically generated by nrfutil on 2019-02-07 (YY-MM-DD) at 04:17:15 */

#include "sdk_config.h"
#include "stdint.h"
#include "compiler_abstraction.h"

#if NRF_CRYPTO_BACKEND_OBERON_ENABLED
/* Oberon backend is changing endianness thus public key must be kept in RAM. */
#define _PK_CONST
#else
#define _PK_CONST const
#endif


/** @brief Public key used to verify DFU images */
__ALIGN(4) _PK_CONST uint8_t pk[64] =
{
    0xfa, 0xc1, 0xb2, 0x09, 0xe2, 0x46, 0xe1, 0xaa, 0x3a, 0x86, 0x5c, 0xb6, 0x68, 0x8f, 0xb8, 0x09, 0x60, 0x89, 0x9e, 0x15, 0xac, 0x10, 0x6f, 0x41, 0xf6, 0xf5, 0x4b, 0xec, 0x1b, 0x49, 0xe7, 0x75, 
    0x0b, 0x68, 0xb5, 0x02, 0x91, 0xa2, 0xa3, 0x80, 0x6e, 0xf5, 0x92, 0x88, 0xe5, 0x27, 0x07, 0x80, 0xd1, 0xb1, 0x4a, 0x1f, 0xd6, 0xe9, 0x33, 0x0b, 0x9b, 0x5a, 0xc7, 0x13, 0xd3, 0x5a, 0x89, 0xec
};
