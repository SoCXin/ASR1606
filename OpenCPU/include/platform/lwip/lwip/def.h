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
#ifndef __LWIP_DEF_H__
#define __LWIP_DEF_H__

/* arch.h might define NULL already */
#include "arch.h"
#include "opt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LWIP_MAX(x , y)     (((x) > (y)) ? (x) : (y))
#define LWIP_MIN(x , y)     (((x) < (y)) ? (x) : (y))
#define LWIP_ARRAYSIZE(x)   (sizeof(x) / sizeof((x)[0]))
#ifndef NULL
#define NULL ((void *)0)
#endif

/** Get the absolute difference between 2 u32_t values (correcting overflows)
 * 'a' is expected to be 'higher' (without overflow) than 'b'. */
#define LWIP_U32_DIFF(a, b) (((a) >= (b)) ? ((a) - (b)) : (((a) + ((b) ^ 0xFFFFFFFF) + 1)))

/* Endianess-optimized shifting of two u8_t to create one u16_t */
#if BYTE_ORDER == LITTLE_ENDIAN
#define LWIP_MAKE_U16(a, b) ((a << 8) | b)
#else
#define LWIP_MAKE_U16(a, b) ((b << 8) | a)
#endif

#ifndef LWIP_PLATFORM_BYTESWAP
#define LWIP_PLATFORM_BYTESWAP 0
#endif

#ifndef LWIP_PREFIX_BYTEORDER_FUNCS
/* workaround for naming collisions on some platforms */

#ifdef htons
#undef htons
#endif /* htons */
#ifdef htonl
#undef htonl
#endif /* htonl */
#ifdef ntohs
#undef ntohs
#endif /* ntohs */
#ifdef ntohl
#undef ntohl
#endif /* ntohl */

#define htons(x) lwip_htons(x)
#define ntohs(x) lwip_ntohs(x)
#define htonl(x) lwip_htonl(x)
#define ntohl(x) lwip_ntohl(x)
#endif /* LWIP_PREFIX_BYTEORDER_FUNCS */

#if BYTE_ORDER == BIG_ENDIAN
#define lwip_htons(x) (x)
#define lwip_ntohs(x) (x)
#define lwip_htonl(x) (x)
#define lwip_ntohl(x) (x)
#define PP_HTONS(x) (x)
#define PP_NTOHS(x) (x)
#define PP_HTONL(x) (x)
#define PP_NTOHL(x) (x)
#else /* BYTE_ORDER != BIG_ENDIAN */
#if LWIP_PLATFORM_BYTESWAP
#define lwip_htons(x) LWIP_PLATFORM_HTONS(x)
#define lwip_ntohs(x) LWIP_PLATFORM_HTONS(x)
#define lwip_htonl(x) LWIP_PLATFORM_HTONL(x)
#define lwip_ntohl(x) LWIP_PLATFORM_HTONL(x)
#else /* LWIP_PLATFORM_BYTESWAP */

#if 0
u16_t lwip_htons(u16_t x);
u16_t lwip_ntohs(u16_t x);
u32_t lwip_htonl(u32_t x);
u32_t lwip_ntohl(u32_t x);
#endif

/**
 * Convert an u16_t from host- to network byte order.
 *
 * @param n u16_t in host byte order
 * @return n in network byte order
 */
static inline u16_t lwip_htons(u16_t n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

/**
 * Convert an u16_t from network- to host byte order.
 *
 * @param n u16_t in network byte order
 * @return n in host byte order
 */
static inline u16_t lwip_ntohs(u16_t n)
{
  return lwip_htons(n);
}

/**
 * Convert an u32_t from host- to network byte order.
 *
 * @param n u32_t in host byte order
 * @return n in network byte order
 */
static inline u32_t lwip_htonl(u32_t n)
{
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24);
}

/**
 * Convert an u32_t from network- to host byte order.
 *
 * @param n u32_t in network byte order
 * @return n in host byte order
 */
static inline u32_t lwip_ntohl(u32_t n)
{
  return lwip_htonl(n);
}

#endif /* LWIP_PLATFORM_BYTESWAP */

/* These macros should be calculated by the preprocessor and are used
   with compile-time constants only (so that there is no little-endian
   overhead at runtime). */
#define PP_HTONS(x) ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define PP_NTOHS(x) PP_HTONS(x)
#define PP_HTONL(x) ((((x) & 0xff) << 24) | \
                     (((x) & 0xff00) << 8) | \
                     (((x) & 0xff0000UL) >> 8) | \
                     (((x) & 0xff000000UL) >> 24))
#define PP_NTOHL(x) PP_HTONL(x)

#endif /* BYTE_ORDER == BIG_ENDIAN */

#ifndef lwip_itoa
/* This can be #defined to itoa() or snprintf(result, bufsize, "%d", number) depending on your platform */
void  lwip_itoa(char* result, size_t bufsize, int number);
#endif
#ifndef lwip_strnicmp
/* This can be #defined to strnicmp() or strncasecmp() depending on your platform */
int   lwip_strnicmp(const char* str1, const char* str2, size_t len);
#endif
#ifndef lwip_stricmp
/* This can be #defined to stricmp() or strcasecmp() depending on your platform */
int   lwip_stricmp(const char* str1, const char* str2);
#endif
#ifndef lwip_strnstr
/* This can be #defined to strnstr() depending on your platform */
char* lwip_strnstr(const char* buffer, const char* token, size_t n);
#endif

/**
 * lwip_HexToBin - convert a hex digit to its real value
 * @ch: ascii character represents hex digit
 *
 * lwip_HexToBin() return:
 *    converts one hex digit to its actual value 
 *    or -1 in case of bad input.
 */
int lwip_HexToBin(char ch);

/**
 * lwip_mac_pton - convert a mac string to its real value
 * @ch: ascii character represents hex digit
 *
 * return: 
 *      0, turn failed.
 *      1, turn ok.
 */
int lwip_mac_pton(const char *s, u8_t *mac);


#ifdef __cplusplus
}
#endif

#endif /* __LWIP_DEF_H__ */

