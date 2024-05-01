/*
 * embedded IPsec
 * Copyright (c) 2003 Niklaus Schild and Christian Scheurer, HTI Biel/Bienne
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
 */

/** @file ipsec_types.h
 *  @brief This module contains the all the data types used by embedded IPsec
 *
 *  @author Niklaus Schild <n.schild@gmx.ch>
 *
 * This document is part of <EM>embedded IPsec<BR>
 * Copyright (c) 2003 Niklaus Schild and Christian Scheurer, HTI Biel/Bienne<BR>
 * All rights reserved.<BR>
 * This file contains code from the OpenSSL Project<BR>
 * portions Copyright (c) 1998-2003 OpenSSL (www.openssl.org)
 *</EM><HR>
 */

#ifndef __IPSEC_TYPES_H__
#define __IPSEC_TYPES_H__
#include "ip6_addr.h"

#ifndef MIN_SYS
#if LWIP_IPSEC

#define IPSEC_DES_KEY_LEN		(8)							/**< Defines the size of a DES key in bytes */
#define IPSEC_3DES_KEY_LEN		(IPSEC_DES_KEY_LEN*3)		/**< Defines the length of a 3DES key in bytes */
#define IPSEC_AES_KEY_LEN		(IPSEC_DES_KEY_LEN*4)		/**< Defines the max length of a AES key in bytes */
#define IPSEC_MAX_ENCKEY_LEN	(IPSEC_AES_KEY_LEN)		    /**< Defines the maximum encryption key length of our IPsec system */

#define IPSEC_AUTH_ICV			(12)						/**< Defines the authentication key length in bytes (12 bytes for 96bit keys) */
#define IPSEC_AUTH_MD5_KEY_LEN	(16)						/**< Length of MD5 secret key  */
#define IPSEC_AUTH_SHA1_KEY_LEN	(20)						/**< Length of SHA1 secret key */
#define IPSEC_MAX_AUTHKEY_LEN   (IPSEC_AUTH_SHA1_KEY_LEN) 	/**< Maximum length of authentication keys */

#define IPSEC_MIN_IPHDR_SIZE	(20) 	/**< Defines the minimum IP header size (in bytes).*/
#define IPSEC_SEQ_MAX_WINDOW	(32)	/**< Defines the maximum window for Sequence Number checks (used as anti-replay protection) */

#define IPSEC_IPV4		4							/**< Defines the ipv4 version */
#define IPSEC_IPV6		6							/**< Defines the ipv6 version */

#define IPSEC_IPH_V(hdr)          ((hdr)->v_hl >> 4)
#define IPSEC_IPH_HL(hdr)         ((hdr)->v_hl & 0x0f)
#define IPSEC_IPH_LEN(hdr)        ((hdr)->len)

#define IPSEC_IP6H_V(hdr)         ((ipsec_ntohl((hdr)->ip6_vtf) >> 28) & 0x0f)
#define IPSEC_IP6H_PLEN(hdr)      (ipsec_ntohs((hdr)->ip6_len))
#define IPSEC_IP6H_NEXTH(hdr)     ((hdr)->ip6_nextheader)
#define IPSEC_IP6H_NEXTH_P(hdr)   ((u8_t *)(hdr) + 6)

typedef unsigned   char    __u8;
typedef signed     char    __s8;
typedef unsigned   short   __u16;
typedef signed     short   __s16;
typedef unsigned   long    __u32;
typedef signed     long    __s32;


/** return code convention:
 *
 *  return code < 0 indicates globally defines error messages
 *  return code == 0 indicates success
 *  return code > 0 is used as error count (i.e. "return 20;" means there are 20 errors)
 *
 */
typedef enum ipsec_status_list {				/** This value is returned if ... */
	IPSEC_STATUS_SUCCESS   			=  0,		/**<  processing was successful */ 
	IPSEC_STATUS_NOT_IMPLEMENTED 	= -1,		/**<  the function is already there but the functionality is not yet implemented */
	IPSEC_STATUS_FAILURE			= -2,		/**<  failure */
	IPSEC_STATUS_DATA_SIZE_ERROR	= -3,		/**<  buffer is (unexpectedly) empty or haves wrong size */
	IPSEC_STATUS_NO_SPACE_IN_SPD	= -4,		/**<  ipsec_spd_add() failed because there was no space left in SPD */
	IPSEC_STATUS_NO_POLICY_FOUND	= -5,		/**<  no matching SPD policy was found */
	IPSEC_STATUS_NO_SA_FOUND		= -6,		/**<  no matching SA was found */
	IPSEC_STATUS_BAD_PACKET			= -7,		/**<  packet has a bad format or invalid fields */
	IPSEC_STATUS_BAD_PROTOCOL		= -8,		/**<  SA has an unsupported protocol */
	IPSEC_STATUS_BAD_KEY			= -9,		/**<  key is invalid or weak and was rejected */
	IPSEC_STATUS_TTL_EXPIRED		= -10,		/**<  TTL value of a packet reached 0 */
	IPSEC_STATUS_NOT_SUPPORT_MODE	= -11,		/**<  NOT support  IPSEC_TUNNEL or IPSEC_TRANSPORT*/
	IPSEC_STATUS_NOT_INITIALIZED   	= -100		/**<  variables has never been initialized */
} ipsec_status;


typedef enum ipsec_audit_list {					/** This value is returned if ... */
	IPSEC_AUDIT_SUCCESS   			=  0,		/**<  processing was successful */ 
	IPSEC_AUDIT_NOT_IMPLEMENTED 	=  1,		/**<  the function is already there but the functionality is not yet implemented */
	IPSEC_AUDIT_FAILURE				=  2,		/**<  failure  */
	IPSEC_AUDIT_APPLY				=  3,		/**<  packet must be processed by IPsec */
	IPSEC_AUDIT_BYPASS				=  4,		/**<  packet is forwarded (without IPsec processing) */
	IPSEC_AUDIT_DISCARD				=  5,		/**<  packet must be dropped */
	IPSEC_AUDIT_SPI_MISMATCH		=  6,		/**<  SPI does not match the SPD lookup */
	IPSEC_AUDIT_SEQ_MISMATCH		=  7,		/**<  Sequence Number differs more than IPSEC_SEQ_MAX_WINDOW from the previous packets */
	IPSEC_AUDIT_POLICY_MISMATCH		=  8		/**<  If a policy for an incoming IPsec packet does not specify APPLY */
} ipsec_audit;


typedef enum ipsec_ip_protocol_list {			/** IP protocol number for ... */
	IPSEC_PROTO_ICMP				= 0x01,		/**<  ICMP */
	IPSEC_PROTO_TCP 				= 0x06, 	/**<  TCP  */
	IPSEC_PROTO_UDP					= 0x11, 	/**<  UDP  */
	IPSEC_PROTO_ESP					= 0x32,		/**<  ESP  */
	IPSEC_PROTO_AH					= 0x33		/**<  AH   */
} ipsec_ip_protocol;


#pragma pack(1)
//#pragma bytealign

typedef struct ipsec_ip_hdr_struct 
{
  __u8	v_hl;				/**< version / header length        */
  __u8	tos;				/**< type of service                */
  __u16 len;				/**< total length                   */
  __u16 id;					/**< identification                 */
  __u16 offset;				/**< fragment offset field / flags  */
  __u8 	ttl;				/**< time to live                   */
  __u8  protocol;			/**< protocol                       */
  __u16 chksum;				/**< checksum                       */
  __u32 src;				/**< source address                 */
  __u32 dest; 				/**< destination address            */
} ipsec_ip_header;

typedef __u32 ip6_address_t[4];     /**< IPv6 adress type    */
typedef struct ipsec_ip6_hdr_struct {
    __u32 ip6_vtf;        /**<  Version (4), Traffic Class (8), Flow Label 20 */

#define IP6_VERSION_M		0xf0000000 /**< IPv6 version bitmask          */
#define IP6_TRAFFIC_CLASS_M	0x0ff00000 /**< IPv6 traffic class  bitmask   */
#define IP6_FLOW_LABEL_M	0x000fffff /**< IPv6 flow label bitmask       */
#define IP6_VERSION		0x60000000 /**< IPv6 version                  */

    __u16 ip6_len;         /**<  Length              */
    __u8 ip6_nextheader;   /**<  Next header         */
    __u8 ip6_hoplimit;     /**<  Hop Limit           */
    ip6_addr_t ip6_src; /**<  Source address      */
    ip6_addr_t ip6_dst; /**<  Destination address */
} ipsec_ip6_header; /**< @see struct ip6_hdr */

typedef struct ipsec_tcp_hdr_struct 
{
  __u16 src;				/**< source port number             */
  __u16 dest;				/**< destination port number        */
  __u32 seqno;				/**< sequence number                */
  __u32 ackno;				/**< acknowledge number             */
  __u16 offset_flags;		/**< offset /flags                  */
  __u16 wnd;				/**< window                         */
  __u16 chksum;				/**< checksum                       */
  __u16 urgp;				/**< urgent pointer                 */
} ipsec_tcp_header;

typedef struct ipsec_udp_hdr_struct 
{
	__u16	src ;			/**< source port number             */
	__u16	dest ;			/**< destination port number        */
	__u16	len ;			/**< length of UDP header and data  */
	__u16	chksum ;		/**< checksum                       */
} ipsec_udp_header ;

#endif
#endif

#endif
