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
#ifndef __LWIP_OPTS_H__
#define __LWIP_OPTS_H__

#define LWIP_LIB_ENABLE              (1)
#if LWIP_LIB_ENABLE

#include "cc.h"

/*****************************************
*lwip macro define for different platform
*default settings is according to LWIP_PLATFORM_DEFINE
******************************************/
#define LWIP_PLATFORM_DEFINE_1802           (0x0001)
#define LWIP_PLATFORM_DEFINE_1802S          (0x0002)
#define LWIP_PLATFORM_DEFINE_CRANE          (0x0004)
#define LWIP_PLATFORM_DEFINE_MINIS          (0x0008) /*based on crane*/
#define LWIP_PLATFORM_DEFINE_NBIOT   	    (0x0010)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_ALIOS   	    (0x0020)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_C4M     	    (0x0040)  /*based on crane*/

/*set default platform to crane*/
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE)

#ifdef PLAT_NEZHAC /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_PLATFORM_DEFINE
#define LWIP_PLATFORM_DEFINE                LWIP_PLATFORM_DEFINE_1802S
#endif

#if 0
#ifdef CRANE_MCU_DONGLE /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_PLATFORM_DEFINE
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_CRANE_MODULE)
#endif
#endif

#ifdef LWIP_NBIOT /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_PLATFORM_DEFINE
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_NBIOT)
#endif

#ifdef PLAT_USE_ALIOS /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_PLATFORM_DEFINE
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_ALIOS)
#endif

#ifdef LTEONLY_THIN /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_PLATFORM_DEFINE
#ifdef PLAT_USE_ALIOS /*this macro is defined from tavor/build/arbel.mak*/
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_C4M | LWIP_PLATFORM_DEFINE_ALIOS)
#else
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_C4M)
#endif
#endif

#ifdef MIN_SYS
#undef LWIP_PLATFORM_DEFINE
#ifdef PLAT_USE_ALIOS /*this macro is defined from tavor/build/arbel.mak*/
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_MINIS | LWIP_PLATFORM_DEFINE_ALIOS)
#else
#define LWIP_PLATFORM_DEFINE                (LWIP_PLATFORM_DEFINE_CRANE | LWIP_PLATFORM_DEFINE_MINIS)
#endif
#endif

#define IS_LWIP_PLATFORM_1802_1802S         (((LWIP_PLATFORM_DEFINE & (LWIP_PLATFORM_DEFINE_1802 | LWIP_PLATFORM_DEFINE_1802S)) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_1802               (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_1802) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_1802S              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_1802S) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_CRANE              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_CRANE) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_MINIS              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_MINIS) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_NBIOT              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_NBIOT) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_USE_ALIOS          (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_ALIOS) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_C4M                (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_C4M) > 0) ? 1 : 0 )

/*****************************************
*Global macro define here for Lwip
******************************************/
/*mifi service port setting config here!*/
#define MFG_S_PORT_NUM          (9930)
#define MFG_C_PORT_NUM          (9931)
#define SHELL_SVR_PORT          (4320)
#define REDIRECT_PORT_80        (80)
#define REDIRECT_PORT_8090      (8090)
#define MONGOOSE_LISTEN_PORT    (80)
#define SIP_ALG_PORT            (0xC413) //5060 in network order

/*bit set for IPv4v6, bit 0 set 1 for ipv4, bit 1 set 1 for ipv6*/
/*if both v4 and v6, value is 0x3*/
#ifndef IP_TYPE_IPV4V6
#define IP_TYPE_IPV4V6			0x3
#endif
#ifndef IP_TYPE_IPV4
#define IP_TYPE_IPV4			0x1
#endif
#ifndef IP_TYPE_IPV6
#define IP_TYPE_IPV6			0x2
#endif
#ifndef IP_TYPE_LOCAL
#define IP_TYPE_LOCAL			0x4
#endif

#ifndef PDP_TYPE_DEFAULT
#define PDP_TYPE_DEFAULT        0x0
#endif
#ifndef PDP_TYPE_DEDICATED
#define PDP_TYPE_DEDICATED      0x1
#endif

#define LWIP_INVALID_CID        (127) /*for char type*/
#define LWIP_DL_DIRECTION       (0)
#define LWIP_UL_DIRECTION       (1)
#define LWIP_IN_DIRECTION       (0)
#define LWIP_OUT_DIRECTION      (1)
#define LWIP_DNS_NAME_LEN       (128)

#define ETH_PAD_SIZE                   0   /*2*/
#define SIZEOF_ETH_HDR                 (14 + ETH_PAD_SIZE)
#define ETH_HEADER_LEN                 (14 + ETH_PAD_SIZE)
#define RNDIS_HEADER_LEN               44 
#define ECM_HEADER_LEN                 0
#define MBIM_HEADER_LEN                64 
#define PPP_HEADER_LEN                 0 
#define WIFI_HEADER_LEN                108
#define EXTEND_HEADER_LEN              128 /*max set, can adapter to rndis,wifi hedaer*/

#define RNDIS_HDR_LEN                   (PMSG_SIZE + RNDIS_HEADER_LEN)  /*16+44*/
#define WIFI_HDR_LEN                    (PMSG_SIZE + WIFI_HEADER_LEN) /*contain pmsg size, not cantain eth_header*/
#define LWIP_CPY_HDR                    96  /*max 128, IP6 Header (40) + TCP Header(32) + eth(14) + extend(10)*/

#define RNDIS_HEADER_SIZE               44  /*keep compiler adapter, will not using anymore*/
#define WIFI_HDR                        108 /*keep compiler adapter, contain pmsg size*/

/*72: IP Header (40) + TCP Header(32), tcp type*/
/*extend to 88 */
#define LWIP_LTE_IP6_PLEN               (88)
/*52: IP Header (20) + TCP Header(32), tcp type*/
/*56: IP Header (20) + ICMP Header(8) + IP Header (20) + ICMP Header(8), ICMP_TE type*/
/*extend to 64 */
#define LWIP_LTE_IP4_PLEN               (64)

/***feature macro define for all platform***/
#define LWIP_NETCONN_DO_RECV_NOBLOCK    (1)
#define LWIP_SOCKET_DEBUG_OP_FLAG       (1)
#define LWIP_NETCONN_DEBUG_OP_FLAG      (1)
#define LWIP_NETCONN_EVENT_OP_FLAG      (1)
#define LWIP_MALLOC_NETCONN_STATIC      (1)
#define LWIP_WND_SCALE                  (1)
#define LWIP_TCPFLOW_FALG               (1)
#define LWIP_RECORED_LINE_FLAG          (0)
#define LWIP_APP_NAT_TABLE_FLAG         (1)
#define LWIP_CUSTOMER_HOOK              (1)
#define LWIP_IMS_SUPPORT                (1)
#define LWIP_TCP_FLOW_USING_STATIC      (1)

#define LWIP_DEBUG                      LWIP_DBG_ON /*LWIP_DBG_ON or LWIP_DBG_OFF */
#define LWIP_DEBUG_DISABLE_ALL          0 /* 1 or 0, set 1 for no need all debug trace now except must debug */
#define LWIP_DEBUG_TIMERNAMES           ( 0 && LWIP_DEBUG )
#define LWIP_DEBUG_MEMP                 ( 0 && LWIP_DEBUG )
#define LWIP_DEBUG_ARP                  ( 0 && LWIP_DEBUG )

/*macro replaced by variable, and will use api to control*/
extern u32_t tcp_rcv_win;
extern u32_t tcp_snd_win;
extern u8_t tcp_rcv_scale;
extern u8_t ipreass_maxage;
extern u8_t ip_default_ttl;

#define TCP_SND_BUF                     tcp_snd_win
#define TCP_WND                         tcp_rcv_win
#define TCP_RCV_SCALE                   tcp_rcv_scale
#define IP_REASS_MAXAGE                 ipreass_maxage
#define IP_DEFAULT_TTL                  ip_default_ttl

#if IS_LWIP_PLATFORM_C4M
#include "lwipopts_c4m.h"
#elif IS_LWIP_PLATFORM_MINIS
#include "lwipopts_minis.h"
#elif IS_LWIP_PLATFORM_USE_ALIOS
#include "lwipopts_alios.h"
#elif IS_LWIP_PLATFORM_NBIOT
#include "lwipopts_nb.h"
#elif IS_LWIP_PLATFORM_CRANE
#include "lwipopts_crane.h"
#elif IS_LWIP_PLATFORM_1802_1802S
#include "lwipopts_1802.h"
#else
#include "lwipopts_crane.h" /*set for default*/
#endif

/**Debug macro control, quickly setting here**/
#define LWIP_DEBUG_HIGH_BITS            8
#define DIAG_ON_DEBUG                   0x00 << LWIP_DEBUG_HIGH_BITS
#define ETHARP_DEBUG                    0x01 << LWIP_DEBUG_HIGH_BITS //Enable debugging in etharp.c
#define NETIF_DEBUG                     0x02 << LWIP_DEBUG_HIGH_BITS //Enable debugging in netif.c
#define PBUF_DEBUG                      0x03 << LWIP_DEBUG_HIGH_BITS //Enable debugging in pbuf.c
#define API_LIB_DEBUG                   0x04 << LWIP_DEBUG_HIGH_BITS //Enable debugging in api_lib.c
#define API_MSG_DEBUG                   0x05 << LWIP_DEBUG_HIGH_BITS //Enable debugging in api_msg.c
#define SOCKETS_DEBUG                   0x06 << LWIP_DEBUG_HIGH_BITS //Enable debugging in sockets.c
#define NETCONN_DEBUG                   0x07 << LWIP_DEBUG_HIGH_BITS //Enable debugging in api_msg.c or api_lib.c
#define ICMP_DEBUG                      0x08 << LWIP_DEBUG_HIGH_BITS //Enable debugging in icmp.c
#define IGMP_DEBUG                      0x09 << LWIP_DEBUG_HIGH_BITS //Enable debugging in igmp.c
#define INET_DEBUG                      0x0A << LWIP_DEBUG_HIGH_BITS //Enable debugging in inet.c
#define IP_DEBUG                        0x0B << LWIP_DEBUG_HIGH_BITS //Enable debugging for IP
#define IP_REASS_DEBUG                  0x0C << LWIP_DEBUG_HIGH_BITS //Enable debugging in ip_frag.c for both frag & reass
#define RAW_DEBUG                       0x0D << LWIP_DEBUG_HIGH_BITS //Enable debugging in raw.c
#define MEM_DEBUG                       0x0E << LWIP_DEBUG_HIGH_BITS //Enable debugging in mem.c
#define MEMP_DEBUG                      0x0F << LWIP_DEBUG_HIGH_BITS //Enable debugging in memp.c
#define SYS_DEBUG                       0x10 << LWIP_DEBUG_HIGH_BITS //Enable debugging in sys.c
#define TIMERS_DEBUG                    0x11 << LWIP_DEBUG_HIGH_BITS //Enable debugging in timers.c
#define TCP_DEBUG                       0x12 << LWIP_DEBUG_HIGH_BITS //Enable debugging for TCP
#define TCP_SLOWTMR_DEBUG               0x13 << LWIP_DEBUG_HIGH_BITS //Enable debugging for TCP
#define TCP_INPUT_DEBUG                 0x14 << LWIP_DEBUG_HIGH_BITS //Enable debugging in tcp_in.c for incoming debug
#define TCP_FR_DEBUG                    0x15 << LWIP_DEBUG_HIGH_BITS //Enable debugging in tcp_in.c for fast retransmit
#define TCP_RTO_DEBUG                   0x16 << LWIP_DEBUG_HIGH_BITS //Enable debugging in TCP for retransmi
#define TCP_CWND_DEBUG                  0x17 << LWIP_DEBUG_HIGH_BITS //Enable debugging for TCP congestion window.
#define TCP_WND_DEBUG                   0x18 << LWIP_DEBUG_HIGH_BITS //Enable debugging in tcp_in.c for window updating
#define TCP_OUTPUT_DEBUG                0x19 << LWIP_DEBUG_HIGH_BITS //Enable debugging in tcp_out.c output functions
#define TCP_RST_DEBUG                   0x1A << LWIP_DEBUG_HIGH_BITS //Enable debugging for TCP with the RST message
#define TCP_QLEN_DEBUG                  0x1B << LWIP_DEBUG_HIGH_BITS //Enable debugging for TCP queue lengths
#define UDP_DEBUG                       0x1C << LWIP_DEBUG_HIGH_BITS //Enable debugging in UDP
#define TCPIP_DEBUG                     0x1D << LWIP_DEBUG_HIGH_BITS //Enable debugging in tcpip.c
#define LWIP_PPP_DEBUG                  0x1E << LWIP_DEBUG_HIGH_BITS //Enable debugging for PPP, fix build error--redefine PPP_DEBUG,so define LWIP_PPP_DEBUG
#define SLIP_DEBUG                      0x1F << LWIP_DEBUG_HIGH_BITS //Enable debugging in slipif.c
#define DHCP_DEBUG                      0x20 << LWIP_DEBUG_HIGH_BITS //Enable debugging in dhcp.c
#define AUTOIP_DEBUG                    0x21 << LWIP_DEBUG_HIGH_BITS //Enable debugging in autoip.c
#define SNMP_MSG_DEBUG                  0x22 << LWIP_DEBUG_HIGH_BITS //Enable debugging for SNMP messages
#define SNMP_MIB_DEBUG                  0x23 << LWIP_DEBUG_HIGH_BITS //Enable debugging for SNMP MIBs
#define DNS_DEBUG                       0x24 << LWIP_DEBUG_HIGH_BITS //Enable debugging for DNS
#define IP6_DEBUG                       0x25 << LWIP_DEBUG_HIGH_BITS //Enable debugging for IPv6
#define IP_NAT_DEBUG                    0x26 << LWIP_DEBUG_HIGH_BITS //Enable debugging in ip_nat.c
#define LWIP_API_DEBUG                  0x27 << LWIP_DEBUG_HIGH_BITS //Enable debugging in lwip_api.c
#define ND6_DEBUG                       0x28 << LWIP_DEBUG_HIGH_BITS //Enable debugging in nd6.c
#define IP_TRACE                        0x29 << LWIP_DEBUG_HIGH_BITS //Enable debugging in ipv4 trace
#define IF_BRIDGE_DEBUG                 0x2A << LWIP_DEBUG_HIGH_BITS //Enable debugging in if bridge
#define IF_USB_DEBUG                    0x2B << LWIP_DEBUG_HIGH_BITS //Enable debugging in if usb
#define IF_WIFI_DEBUG                   0x2C << LWIP_DEBUG_HIGH_BITS //Enable debugging in if wifi
#define IF_PDP_DEBUG                    0x2D << LWIP_DEBUG_HIGH_BITS //Enable debugging in if pdp
#define IP_SEC_DEBUG                    0x2E << LWIP_DEBUG_HIGH_BITS //Enable debugging in ipsec
#define TFT_DEBUG                       0x2F << LWIP_DEBUG_HIGH_BITS //Enable debugging in tft module
#define REDIRECT_DEBUG                  0x30 << LWIP_DEBUG_HIGH_BITS //Enable debugging in redirect module

#else /* LWIP_LIB_ENABLE */

#define NO_SYS                          1
#define LWIP_NETCONN                    0
#define LWIP_DEBUG                      0
#define LWIP_SOCKET                     0
#define LWIP_DNS                        0
#define LWIP_NETIF_API                  0
#define UDP_ECHO_SVR                    0
#define TCP_ECHO_SVR                    0
#define LWIP_DNS_RELAY                  0
#define LWIP_DHCP_SERVER                0
#define IP_NAT                          0
#define LWIP_ICMP                       0
#define LWIP_IGMP                       0
#define LWIP_STATS                      0
#define LWIP_RAW                        0
#define LWIP_DHCP                       0
#define LWIP_UDP                        0
#define LWIP_TCP                        0
#define LWIP_TIMERS                     0
#define LWIP_NETIF_LTE                  0
#define LWIP_NETIF_PC                   0
#define LWIP_NETIF_BRIDGE               0
#define LWIP_ARP                        0
#define LWIP_ETHERNET                   0
#define LWIP_NETIF_WF_UAP               0
#define IP_FILTER_EN			        0

#endif /* LWIP_LIB_ENABLE */

#endif /* __LWIP_OPTS_H__ */
