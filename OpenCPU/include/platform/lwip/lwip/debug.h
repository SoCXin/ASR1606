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
#ifndef __LWIP_DEBUG_H__
#define __LWIP_DEBUG_H__

#include "arch.h"
#include "opt.h"

//ICAT EXPORTED ENUM
typedef enum
{
	LOG_DISABLE     = 0x0,
	UART_LOG_ENABLE = 0x1,
	ACAT_LOG_ENABLE = 0x2
} Log_ConfigE;

//ICAT EXPORTED ENUM
typedef enum
{
	MSG_DISABLE     = 0x0,
	ACAT_MSG_ENABLE = 0x1
} Msg_ConfigE;

//ICAT EXPORTED ENUM
typedef enum
{
	RTI_LOG_DISABLE = 0x0,
	RTI_DUMP_ENABLE = 0x1,
	RTI_TASK_ENABLE = 0x2,
	RTI_MIPS_ENABLE = 0x3
} RTI_ConfigE;

#if 0
//ICAT EXPORTED ENUM
typedef enum
{
	MASS_STORAGE_DISABLE = 0x0,
	MASS_STORAGE_ENABLE  = 0x1
} MassStorage_ConfigE;
#endif
//ICAT EXPORTED STRUCT
typedef struct {
	Log_ConfigE log_cfg;
	Msg_ConfigE msg_cfg;
	RTI_ConfigE rti_cfg;
} Log_ConfigS;

//add by shimingrui 220711 编译
extern Log_ConfigS log_config;

extern u8_t lwip_urt_debug;
void sys_msleep(u32_t ms);
extern void lwip_record_assert(const char *string, unsigned int line);

/** lower two bits indicate debug level
 * - 0 all
 * - 1 warning
 * - 2 serious
 * - 3 severe
 */
#define LWIP_DBG_LEVEL_ALL     0x00
#define LWIP_DBG_LEVEL_OFF     LWIP_DBG_LEVEL_ALL /* compatibility define only */
#define LWIP_DBG_LEVEL_INFO    0x01 /* normal info */
#define LWIP_DBG_LEVEL_WARNING 0x02 /* bad checksums, dropped packets, ... */
#define LWIP_DBG_LEVEL_SERIOUS 0x03 /* memory allocation failures, ... */
#define LWIP_DBG_LEVEL_SEVERE  0x04
#define LWIP_DBG_LEVEL_END     (LWIP_DBG_LEVEL_SEVERE + 1)

#define LWIP_DBG_MASK_LEVEL    0x000F /*low 4 bit mask for level*/
#define LWIP_DBG_MASK_MODUL    0xFF00 /*high 8 bit mask for modul */

/** flag for LWIP_DEBUGF to enable that debug message */
#define LWIP_DBG_ON            0x80U
/** flag for LWIP_DEBUGF to disable that debug message */
#define LWIP_DBG_OFF           0x00U

/** flag for LWIP_DEBUGF indicating a tracing message (to follow program flow) */
#define LWIP_DBG_TRACE         0x40U
/** flag for LWIP_DEBUGF indicating a state debug message (to follow module states) */
#define LWIP_DBG_STATE         0x20U
/** flag for LWIP_DEBUGF indicating newly added code, not thoroughly tested yet */
#define LWIP_DBG_FRESH         0x10U
/** flag for LWIP_DEBUGF to halt after printing this debug message */
#define LWIP_DBG_HALT          0x08U

#if 0
#ifndef LWIP_NOASSERT
#define LWIP_ASSERT(assertion) ASSERT(assertion);

//  LWIP_PLATFORM_ASSERT(message); } while(0)
#else  /* LWIP_NOASSERT */
#define LWIP_ASSERT(message, assertion)
#endif /* LWIP_NOASSERT */
#endif
#define LWIP_ASSERT_MSG(message, assertion)     do { if (!(assertion)) { \
                                                    LWIP_PLATFORM_ERROR(message);\
                                                    sys_msleep(2000); \
                                                    ASSERT(assertion);}} while(0)
#if 0
#define LWIP_ASSERT(assertion)                  do { if (!(assertion)) { \
                                                    sys_msleep(2000); \
                                                    ASSERT(assertion);}} while(0)
#endif
#define LWIP_ASSERT_NOW(assertion)                  do { if (!(assertion)) { \
                                                    ASSERT(assertion);}} while(0)

/** if "expression" isn't true, then print "message" and execute "handler" expression */
#if 0
#ifndef LWIP_ERROR
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
  LWIP_PLATFORM_ASSERT(message); handler;}} while(0)
#endif /* LWIP_ERROR */
#endif
#ifndef LWIP_ERROR
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
      LWIP_PLATFORM_ERROR(message); handler;}} while(0)
#endif /* LWIP_ERROR */

struct moduleDebugSettingT {
  char *name;
  u8_t onoff;
  u8_t level;
  u8_t type;
};

#ifndef lwipModuleSwitchSize
#define lwipModuleSwitchSize 64
#endif

#ifdef LWIP_DEBUGF_MODUL_DATA_DEFINE
/**
 *  !!! order of array member !!!
 *  refer to macro(DIAG_ON_DEBUG/ETHARP_DEBUG...) in lwipopt.h
 **/
struct moduleDebugSettingT g_moduleDebugSettings[lwipModuleSwitchSize] = {
  {"diag",          LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         DIAG_ON_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"arp",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         ETHARP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"netif",         LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         NETIF_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"pbuf",          LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         PBUF_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"apilib",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         API_LIB_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"apimsg",        LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         API_MSG_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"sockets",       LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         SOCKETS_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"netconn",       LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         NETCONN_DEBUG >> LWIP_DEBUG_HIGH_BITS}, // 7
  {"icmp",          LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         ICMP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"igmp",          LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IGMP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"inet",          LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         INET_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ip",            LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ipreass",       LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IP_REASS_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"raw",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         RAW_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"mem",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         MEM_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"memp",          LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         MEMP_DEBUG >> LWIP_DEBUG_HIGH_BITS}, // 15
  {"sys",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         SYS_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"timers",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TIMERS_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcp",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpslowtmr",    LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         TCP_SLOWTMR_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpinput",      LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_INPUT_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpfr",         LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_FR_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcprto",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_RTO_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpcwnd",       LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_CWND_DEBUG >> LWIP_DEBUG_HIGH_BITS}, // 23
  {"tcpwnd",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_WND_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpoutput",     LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_OUTPUT_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcprst",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_RST_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpqlen",       LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCP_QLEN_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"udp",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         UDP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"tcpip",         LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TCPIP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ppp",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         LWIP_PPP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"slip",          LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         SLIP_DEBUG >> LWIP_DEBUG_HIGH_BITS}, // 31
  {"dhcp",          LWIP_DBG_ON,       LWIP_DBG_LEVEL_ALL,         DHCP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"autoip",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         AUTOIP_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"snmpmsg",       LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         SNMP_MSG_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"snmpmib",       LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         SNMP_MIB_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"dns",           LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         DNS_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ip6",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IP6_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ipnat",         LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IP_NAT_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"lwipapi",       LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         LWIP_API_DEBUG >> LWIP_DEBUG_HIGH_BITS}, // 39
  {"nd6",           LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         ND6_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"iptrace",       LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         IP_TRACE >> LWIP_DEBUG_HIGH_BITS},
  {"bridge",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IF_BRIDGE_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ifusb",         LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IF_USB_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ifwifi",        LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IF_WIFI_DEBUG >> LWIP_DEBUG_HIGH_BITS},
  {"ifpdp",         LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         IF_PDP_DEBUG >> LWIP_DEBUG_HIGH_BITS},  
  {"ipsec",         LWIP_DBG_ON,        LWIP_DBG_LEVEL_ALL,         IP_SEC_DEBUG >> LWIP_DEBUG_HIGH_BITS},  
  {"tft",           LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         TFT_DEBUG >> LWIP_DEBUG_HIGH_BITS},      
  {"redirect",      LWIP_DBG_OFF,       LWIP_DBG_LEVEL_ALL,         REDIRECT_DEBUG >> LWIP_DEBUG_HIGH_BITS},
};

#else
extern struct moduleDebugSettingT g_moduleDebugSettings[lwipModuleSwitchSize];
#endif

#if LWIP_DEBUG
/** Print debug message only if debug message type is enabled...
 *  AND is of correct type AND is at least LWIP_DBG_LEVEL
 *  8 bits module id | 4 bit type | 4 bits level id
 *  eg. socket module id SOCKETS_DEBUG = 0x01 << 8 = 0x0100
 *     level id LWIP_DBG_LEVEL_WARNING = 0x01
 *     so debug = SOCKETS_DEBUG| LWIP_DBG_LEVEL_WARNING
 *
 * !!!!!!note!!!!!
 * CPUartLogPrintf message;  not CPUartLogPrintf(message);
 * because CPUartLogPrintf input is "const char *fmt, ..."
 * so, if using (message), then will beome one variable
 **/
#define LWIP_DEBUGF_SWITCH_CHECK_S(debug)     ((!IS_DIAG_BLOCKED) \
                                                && (((g_moduleDebugSettings[(debug & LWIP_DBG_MASK_MODUL) >> LWIP_DEBUG_HIGH_BITS].onoff) == LWIP_DBG_ON) ? 1 : 0))

#define LWIP_DEBUGF_SWITCH_CHECK(debug)     ((!IS_DIAG_BLOCKED)\
                                            && (((g_moduleDebugSettings[(debug & LWIP_DBG_MASK_MODUL) >> LWIP_DEBUG_HIGH_BITS].onoff) == LWIP_DBG_ON) ? 1 : 0) \
                                            && (LOG_DISABLE != log_config.log_cfg) \
                                            && (log_config.msg_cfg == ACAT_MSG_ENABLE) \
                                            && (booting_done))

/*using for inline function*/
#define LWIP_DEBUGF(debug, message)         do { \
                                                if (LWIP_DEBUGF_SWITCH_CHECK(debug)) { \
                                                    LWIP_LOG message; \
                                                    break; \
                                                }\
                                            } while(0)

#else  /* LWIP_DEBUG */
#define LWIP_DEBUGF(debug, message)
#define LWIP_DEBUGF_SWITCH_CHECK(debug)         (0)
#define LWIP_DEBUGF_SWITCH_CHECK_S(debug)       (0)
#define LWIP_DEBUGF_SWITCH_CHECK_URT(debug)     (0)
#endif /* LWIP_DEBUG */

#if 0
#if (LWIP_DBG_OFF == LWIP_DEBUG)
#if LWIP_DEBUG_DISABLE_ALL
/*no need redefine here*/
#else
#undef LWIP_DEBUGF_SWITCH_CHECK
#undef LWIP_DEBUGF
#undef LWIP_DIAG

#define LWIP_DEBUGF_SWITCH_CHECK(debug)     ((!IS_DIAG_BLOCKED)\
                                            && (((g_moduleDebugSettings[(debug & LWIP_DBG_MASK_MODUL) >> LWIP_DEBUG_HIGH_BITS].onoff) == LWIP_DBG_ON) ? 1 : 0) \
                                            && (LOG_DISABLE != log_config.log_cfg) \
                                            && (log_config.msg_cfg == ACAT_MSG_ENABLE) \
                                            && (booting_done))

/*using for inline function*/
#define LWIP_DEBUGF(debug, message)         do { \
                                                if (LWIP_DEBUGF_SWITCH_CHECK(debug)) { \
                                                    LWIP_LOG message; \
                                                    break; \
                                                }\
                                            } while(0)

#define LWIP_DIAG(debug, cat3, fmt, args...)  \
                                            {                                                           \
                                                if (LWIP_DEBUGF_SWITCH_CHECK(debug))                    \
                                                {                                                       \
                                                    DIAG_FILTER(MIFI, LWIP, cat3, DIAG_INFORMATION)     \
                                                    diagPrintf(fmt, ##args);                            \
                                                }                                                       \
                                            }
#endif
#endif
#endif

#endif /* __LWIP_DEBUG_H__ */

