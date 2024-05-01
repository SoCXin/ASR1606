/**
 * \file aes.h
 *
 * \brief AES block cipher
 *
 *  Copyright (C) 2006-2010, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef IPSEC_AES_H
#define IPSEC_AES_H

#include <string.h>
#include "ipsec_types.h"

#ifndef MIN_SYS
#if LWIP_IPSEC

#ifdef _MSC_VER
#include <basetsd.h>
typedef UINT32 uint32_t;
#else
#include <inttypes.h>
#endif

#ifndef NO_SUPPORT_TLS
#include "mbedtls/aes.h"
#include "mbedtls/compat-1.3.h" // mbedtls
#else
#define AES_ENCRYPT     1
#define AES_DECRYPT     0

#define POLARSSL_ERR_AES_INVALID_KEY_LENGTH                -0x0020  /**< Invalid key length. */
#define POLARSSL_ERR_AES_INVALID_INPUT_LENGTH              -0x0022  /**< Invalid data input length. */

/**
 * \brief          AES context structure
 */
typedef struct
{
    int nr;                     /*!<  number of rounds  */
    uint32_t *rk;               /*!<  AES round keys    */
    uint32_t buf[68];           /*!<  unaligned data    */
}
aes_context;
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * AES-CBC function calculates a digest from a given data buffer and a given key.
 *
 * @param text		pointer to input data
 * @param text_len	length of input data
 * @param key		pointer to encryption key (128/192/256 bits)
 * @param keysize	length of input encryption key (128/192/256 bits)
 * @param iv		initialization vector
 * @param mode		defines whether encryption or decryption should be performed
 * @param output	en- or decrypted input data
 * @return void
 *
 */
void ipsec_cipher_aes_cbc(unsigned char* text, 
                    int text_len, 
                    unsigned char* key, 
                    unsigned int keysize, 
                    unsigned char* iv, 
                    int mode, 
                    unsigned char*  output);


#ifdef __cplusplus
}
#endif

#endif
#endif

#endif /* aes.h */
