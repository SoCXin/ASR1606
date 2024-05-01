#ifndef NETIF_TD_HDR_H
#define NETIF_TD_HDR_H

#include "err.h"
#include "sys.h"
#include "sys_arch.h"
#include "netif.h"

#if LWIP_NETIF_LTE

/**************************************************************************
 *                          Define local macro                            *
 **************************************************************************/
#define _BYTES_ALIGN_8(len)         ((len+8)-(len+8)%8)
#define RECORD_TICK                 (200) /* 1s */
#define LTE_SIM_MAX_NUM             (2)
#define LTE_CID_MAX_NUM             (16)
#define LTE_NETIF_PDP_MAX           (16)
#define LTE_PKG_TRIGGER_SIZE        (64)

#define PDP_FLAG_DEFAULT_BIT_POS    (0x1)   /*bit1: 0 denote default pdp; 1 denote dedicated pdp*/
#define PDP_FLAG_PPP_BIT_POS        (0x2)   /*bit2: if set to 1, denote ppp pdp config mode*/
#define PDP_FLAG_MBIM_BIT_POS       (0x4)   /*bit3: if set to 1, denote mbin pdp config mode*/
#define PDP_FLAG_APP_BIT_POS        (0x8)   /*bit4: if set to 1, denote app pdp config mode*/
#define PDP_FLAG_IMS_BIT_POS        (0x10)  /*bit5: if set to 1, denote ims pdp config mode*/
#define PDP_FLAG_PASS_BIT_POS       (0x20)  /*bit6: if set to 1, denote pass-through pdp config mode*/

#ifndef LWIP_ROUTER_IP
#define LWIP_ROUTER_IP (0xC0A80000) /* 192.168.x.x */
#endif

extern unsigned char IS_LTE_W_PS;

/**************************************************************************
 *                          Define local struct                           *
 **************************************************************************/
struct td_info_t {
    u32_t ip_addr;
    u32_t dns_ip_1;
    u32_t dns_ip_2;
    u32_t gw_ip;
    u32_t mask;
};

struct td_speed_t {
    u32_t ticks;
    u32_t t1;
    u32_t t2;
    u32_t speed;
};

/*match to dialer_task.h
struct  Ipv4Info definition match lwip_ipv4_info;
struct  Ipv6Info definition match lwip_ipv6_info;
*/
typedef struct {
        int  IPAddr;
        int  PrimaryDNS;
        int  SecondaryDNS;
        int  GateWay;
        int  Mask;
} lwip_ipv4_info;

typedef struct {
        int  IPV6Addr[4];
        int  PrimaryDNS[4];
        int  SecondaryDNS[4];
        int  GateWay[4];
        int  Mask[4];
} lwip_ipv6_info;

typedef struct{
	char cid;          /* for PDP context: 
	                    (cid >> 7), sim id, 0 denote sim card 0, 1 denote sim card 1; 
	                    (cid & 0x3F), cid index config. 
	                      for tune netif context :
	                     cid, denote tune index config.
	                      */
	char status;       /* 0 for disconnect, 1 fot connect, 0xFF, denote delete directly */
	char pdp_flag;
	char ip_type;      /* 0 bit, set for IPv4; 1 bit, set for IPv6; value can:1\2\3 */
	lwip_ipv4_info *ip4info; /*if only v6, set to NULL */
	lwip_ipv6_info *ip6info; /*if only v4, set to NULL */
    sys_sem_t *sem;   /* This semaphore is posted when tcpip_thread process finish*/
} lwip_ip_configinfo;

typedef struct {
    u16_t status;
    u8_t simid;
    u8_t cid;
    u32_t IPAddr;
    u32_t PrimaryDNS;
    u32_t SecondaryDNS;
    u32_t GateWay;
    u32_t NetMask;
} netif_ip4_info;

typedef struct {
    u16_t status;
    u8_t simid;
    u8_t cid;
    u32_t IPV6Addr[4];
    u32_t PrimaryDNS[4];
    u32_t SecondaryDNS[4];
} netif_ip6_info;

typedef struct {
    u32_t  local_ip6[4];
    u32_t  global_ip6[4];
    u32_t  gateway_ip6[4];
} lte_ip6_addr;

#endif /* LWIP_NETIF_TD */

#endif
