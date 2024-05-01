/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIP_CC_H__
#define __LWIP_CC_H__

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;

typedef signed char     INT8;
typedef signed short    INT16;
typedef signed long     INT32;
typedef unsigned long long    UINT64;

typedef UINT8       u8_t;
typedef UINT16      u16_t;
typedef UINT32      u32_t;
typedef UINT64      u64_t;

typedef INT8        s8_t;
typedef INT16       s16_t;
typedef INT32       s32_t;
typedef void*       msg_t;

typedef u32_t       mem_ptr_t;

#if 0
//add by shimingrui 220711
#include <stdint.h>
typedef uint8_t       u8_t;
typedef uint16_t      u16_t;
typedef uint32_t      u32_t;
typedef uint64_t      u64_t;

typedef int8_t        s8_t;
typedef int16_t       s16_t;
typedef int32_t       s32_t;
typedef void*       msg_t;
#endif
#if 0
#include "UART.h"
#include "diag.h"

typedef UINT8       u8_t;
typedef UINT16      u16_t;
typedef UINT32      u32_t;
typedef UINT64      u64_t;

typedef INT8        s8_t;
typedef INT16       s16_t;
typedef INT32       s32_t;
typedef void*       msg_t;

typedef u32_t       mem_ptr_t;
#endif

/* Define printf formatters for these lwIP types */
#define U16_F "u"
#define U32_F "u"
#define S16_F "d"
#define S32_F "d"
#define X16_F "x"
#define X32_F "x"

/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x)    x
#define PACK_STRUCT_FLD_8(x) PACK_STRUCT_FIELD(x)
#define PACK_STRUCT_FLD_S(x) PACK_STRUCT_FIELD(x)
#define PACK_STRUCT_STRUCT  __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

//extern void CPUartLogPrintf(const char *fmt, ...);

//add by shimingrui 220711
/* Plaform specific diagnostic output */
#define LWIP_PLATFORM_DIAG(x)   do {                \
    } while (0)

#define LWIP_PLATFORM_ASSERT(x) do {                                    \
    } while (0)

#define LWIP_PLATFORM_ERROR(x) do {                                    \
    } while (0)


#define MBOX_MAX_SIZE   (sizeof(void*))


#endif /* __LWIP_CC_H__ */

