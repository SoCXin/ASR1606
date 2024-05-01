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
#ifndef __LWIP_NETIF_H__
#define __LWIP_NETIF_H__

#include "opt.h"
#include "err.h"
#include "ip_addr.h"
#include "ip6_addr.h"
#include "def.h"
#include "pbuf.h"

#if LWIP_DHCP
struct dhcp;
#endif
#if LWIP_AUTOIP
struct autoip;
#endif
#if LWIP_IPV6_DHCP6
#include "dhcp6.h"
#endif /* LWIP_IPV6_DHCP6 */

#ifdef __cplusplus
extern "C" {
#endif

/*macro defined for lte netif cid process*/
#define LTE_GET_U8_CID_PART(a)      ((a) & 0x3F) /*for a is char*/
#define LTE_GET_U8_SIMID_PART(a)    (((a) >> 7) & 0x1)
#define LTE_GET_U32_CID_PART(a)     ((a) & 0x3FU)  /*for a is int32*/
#define LTE_GET_U32_SIMID_PART(a)   (((a) >> 31U) & 0x1U)

#define ENABLE_LOOPBACK                     (LWIP_NETIF_LOOPBACK || LWIP_HAVE_LOOPIF)
#define NETIF_IP6_ADDR_LINK_POS             (0)
#define NETIF_IP6_ADDR_GLOABAL_POS          (1)

/* Throughout this file, IP addresses are expected to be in
 * the same byte order as in IP_PCB. */

/** must be the maximum of all used hardware address lengths
    across all types of interfaces in use */
#define NETIF_MAX_HWADDR_LEN        (6U)
#define NETIF_NO_INDEX              (0)
#define NETIF_MAX_HOSTNAME_LEN      (16)
#define NETIF_TUNE_HOSTNAME_SIZE    (8)

/**********************************/
/*define netif flags definition*/
/**********************************/
/** Whether the network interface is 'up'. This is
 * a software flag used to control whether this network
 * interface is enabled and processes traffic.
 * It is set by the startup code (for static IP configuration) or
 * by dhcp/autoip when an address has been assigned.
 */
#define NETIF_FLAG_UP           0x01U
/** If set, the netif has broadcast capability.
 * Set by the netif driver in its init function. */
#define NETIF_FLAG_BROADCAST    0x02U
/** If set, the netif is one end of a point-to-point connection.
 * Set by the netif driver in its init function. */
#define NETIF_FLAG_POINTTOPOINT 0x04U
/** If set, the interface is configured using DHCP.
 * Set by the DHCP code when starting or stopping DHCP. */
#define NETIF_FLAG_DHCP         0x08U
/** If set, the interface has an active link
 *  (set by the network interface driver).
 * Either set by the netif driver in its init function (if the link
 * is up at that time) or at a later point once the link comes up
 * (if link detection is supported by the hardware). */
#define NETIF_FLAG_LINK_UP      0x10U
/** If set, the netif is an ethernet device using ARP.
 * Set by the netif driver in its init function.
 * Used to check input packet types and use of DHCP. */
#define NETIF_FLAG_ETHARP       0x20U
/** If set, the netif is an ethernet device. It might not use
 * ARP or TCP/IP if it is used for PPPoE only.
 */
#define NETIF_FLAG_ETHERNET     0x40U
/** If set, the netif has IGMP capability.
 * Set by the netif driver in its init function. */
#define NETIF_FLAG_IGMP         0x80U

/* internal network Interface */
#define NETIF_FLAG_INTER        0x0100U
/* external network Interface */
#define NETIF_FLAG_EXTER        0x0200U
/* DHCP is finished */
#define NETIF_FLAG_DHCP_DONE    0x0400U
/* Init is finished */
#define NETIF_FLAG_INIT_DONE    0x0800U
/* netif is static memory define flag */
/* netif_uap, netif_pc, netif_loop, need set this flag*/
/* netif_pdp, malloc by predefined pool ,no need to set */
/* when remove netif, check this flag, decide to call free() */
#define NETIF_FLAG_STATIC_MEM   0x1000U
/* for malloc netif, need set to this flag */
/* when remove netif, check this flag, call free(netif) */
#define NETIF_FLAG_MALLOC_MEM   0x2000U
/* denote this netif can do nat */
#define NETIF_FLAG_NAT_CONFIG   0x4000U

/* local netif type normal config */
#define NETIF_FLAG_IF_TUNE   0x08000U
#define NETIF_FLAG_IF_USB    0x10000U
#define NETIF_FLAG_IF_UAP    0x20000U
#define NETIF_FLAG_IF_PPP    0x40000U
#define NETIF_FLAG_IF_PDN    0x80000U
#define NETIF_FLAG_IF_LOOP   0x100000U
#define NETIF_FLAG_IF_ETH    0x200000U


/**********************************/
/*define netif flagsEx definition*/
/**********************************/
/* LTE SET IPv4 Info */
#define NETIF_FLAG_EX_IPV4_CFG     0x01U
/* LTE SET IPv6 Info */
#define NETIF_FLAG_EX_IPV6_CFG     0x02U
/* rs send done */
#define NETIF_FLAG_EX_RS_DONE      0x04U
/* ICMPv6 RA is finished */
#define NETIF_FLAG_EX_RA_DONE      0x08U
/* netif epsid set flag */
#define NETIF_FLAG_EX_EPSID_CFG    0x10U
/* DHCP6 DNS Server is finished */
#define NETIF_FLAG_EX_DHCP6_DNS    0x20U
/* DHCP6 PD is finished */
#define NETIF_FLAG_EX_DHCP6_PD     0x40U
/* LTE SET IPV6 Info DNS is finished */
#define NETIF_FLAG_EX_ND6_DNS      0x80U
/* denote this netif config ipv6 address by OTA(config by pdp directly, not by rs&ra) */
#define NETIF_FLAG_EX_IPV6_OTA     0x100U
/* denote this netif config ipv6 address by ifconfig(config by if config api) */
#define NETIF_FLAG_EX_IPV6_IFCONFIG 0x200U

/* source flag:LTE PDP type is default pdp */
#define NETIF_FLAG_EX_DEFAULT_PDP  0x400U
/* source flag: LTE PDP type is ppp config */
#define NETIF_FLAG_EX_PPP_PDP      0x1000U
/* source flag: LTE PDP type is mbin config */
#define NETIF_FLAG_EX_MBIM_PDP     0x2000U
/* source flag: LTE PDP type is APP config */
#define NETIF_FLAG_EX_APP_PDP      0x4000U
/* source flag: LTE PDP type is normal config */
#define NETIF_FLAG_EX_NORMAL_PDP   0x8000U
/* source flag: LTE PDP type is IMS config */
#define NETIF_FLAG_EX_IMS_PDP      0x10000U
/* source flag: LTE PDP type is pass-through config */
#define NETIF_FLAG_EX_PASS_PDP     0x20000U


/** Function prototype for netif->bypass functions. This function is saved as 'bypass'
 * callback function in the netif struct. Call it when a packet need forward.
 *
 * @param p The received packet, copied into a pbuf
 * @param inp The netif which received the packet
 */
typedef err_t (*netif_bypass_fn)(struct pbuf *p, struct netif *inp);

/** Function prototype for netif init functions. Set up flags and output/linkoutput
 * callback functions in this function.
 *
 * @param netif The netif to initialize
 */
typedef err_t (*netif_init_fn)(struct netif *netif);
/** Function prototype for netif->input functions. This function is saved as 'input'
 * callback function in the netif struct. Call it when a packet has been received.
 *
 * @param p The received packet, copied into a pbuf
 * @param inp The netif which received the packet
 */
typedef err_t (*netif_input_fn)(struct pbuf *p, struct netif *inp);
/** Function prototype for netif->output functions. Called by lwIP when a packet
 * shall be sent. For ethernet netif, set this to 'etharp_output' and set
 * 'linkoutput'.
 *
 * @param netif The netif which shall send a packet
 * @param p The packet to send (p->payload points to IP header)
 * @param ipaddr The IP address to which the packet shall be sent
 */
typedef err_t (*netif_output_fn)(struct netif *netif, struct pbuf *p, ip_addr_t *ipaddr);
#if LWIP_IPV6
/** Function prototype for netif->output_ip6 functions. Called by lwIP when a packet
 * shall be sent. For ethernet netif, set this to 'nd_output' and set
 * 'linkoutput'.
 *
 * @param netif The netif which shall send a packet
 * @param p The packet to send (p->payload points to IP header)
 * @param ipaddr The IPv6 address to which the packet shall be sent
 */
typedef err_t (*netif_output_ip6_fn)(struct netif *netif, struct pbuf *p, ip6_addr_t *ipaddr);
#endif /* LWIP_IPV6 */
/** Function prototype for netif->linkoutput functions. Only used for ethernet
 * netifs. This function is called by ARP when a packet shall be sent.
 *
 * @param netif The netif which shall send a packet
 * @param p The packet to send (raw ethernet packet)
 */
typedef err_t (*netif_linkoutput_fn)(struct netif *netif, struct pbuf *p);
/** Function prototype for netif status- or link-callback functions. */
typedef void (*netif_status_callback_fn)(struct netif *netif);
/** Function prototype for netif igmp_mac_filter functions */
typedef err_t (*netif_igmp_mac_filter_fn)(struct netif *netif,
       ip_addr_t *group, u8_t action);
#if LWIP_IPV6 && LWIP_IPV6_MLD
/** Function prototype for netif mld_mac_filter functions */
typedef err_t (*netif_mld_mac_filter_fn)(struct netif *netif,
       ip6_addr_t *group, u8_t action);
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
typedef err_t (*cb_for_td)(struct netif *netif, struct pbuf *p, u8_t id);
typedef err_t (*cb_for_lc)(struct netif *netif, struct pbuf *p, ip_addr_t *ipaddr);

/** Generic data structure used for all lwIP network interfaces.
 *  The following fields should be filled in by the initialization
 *  function for the device driver: hwaddr_len, hwaddr[], mtu, flags */
struct netif {
  /** pointer to next in linked list */
  struct netif *next;

  u8_t simid; /** sim card id, 0 or 1*/
  u8_t cid;   /** primary cid, denote for netif pdp*/
  u8_t sid;
  u8_t epsid; /** epsid, denote for all pdp context for lte using*/

  u8_t ip_proto; /*denote this netif which ipv4 protocol using*/
  u8_t ip6_proto; /*denote this netif which ipv6 protocol using*/
  u8_t ref;   /** refer value for share pdp when using ppp or mbin with app*/  
  u8_t num;   /** the number of this interface */

  /** IP address configuration in network byte order */
  ip_addr_t ip_addr;
  ip_addr_t netmask;
  ip_addr_t gw;
  ip_addr_t dns_server [2];

#if LWIP_IPV6
  /** IPv6 gateway for this netif. */
  ip6_addr_t ip6_gw;
  /** Array of IPv6 addresses for this netif. [0]local, [1]global [2]site*/
  ip6_addr_t ip6_addr[LWIP_IPV6_NUM_ADDRESSES];
  /** Array of IPv6 dns addresses for this netif.*/
  ip6_addr_t dns6_server [2];
  /** IPv6 backup addr for pass through pdp, restore the old linklocal addr now. */
  ip6_addr_t ip6_addr_backup;
  /** The state of each IPv6 address (Tentative, Preferred, etc).
   * @see ip6_addr.h */
  u8_t ip6_addr_state[LWIP_IPV6_NUM_ADDRESSES];
  u8_t ip6_addr_state_aligned;/*only for aligned bytes using*/
#endif /* LWIP_IPV6 */
  /** This function is called by the network device driver
   *  to bypass a packet to ppp or mbin. */
  netif_bypass_fn bypass;
  /** This function is called by the network device driver
   *  to pass a packet up the TCP/IP stack. */
  netif_input_fn input;
  /** This function is called by the IP module when it wants
   *  to send a packet on the interface. This function typically
   *  first resolves the hardware address, then sends the packet. */
  netif_output_fn output;
  /** This function is called by the ARP module when it wants
   *  to send a packet on the interface. This function outputs
   *  the pbuf as-is on the link medium. */
  netif_linkoutput_fn linkoutput;
#if LWIP_IPV6
  /** This function is called by the IPv6 module when it wants
   *  to send a packet on the interface. This function typically
   *  first resolves the hardware address, then sends the packet. */
  netif_output_ip6_fn output_ip6;
#endif /* LWIP_IPV6 */
#if LWIP_NETIF_STATUS_CALLBACK
  /** This function is called when the netif state is set to up or down
   */
  netif_status_callback_fn status_callback;
#endif /* LWIP_NETIF_STATUS_CALLBACK */
#if LWIP_NETIF_LINK_CALLBACK
  /** This function is called when the netif link is set to up or down
   */
  netif_status_callback_fn link_callback;
#endif /* LWIP_NETIF_LINK_CALLBACK */
#if LWIP_NETIF_REMOVE_CALLBACK
  /** This function is called when the netif has been removed */
  netif_status_callback_fn remove_callback;
#endif /* LWIP_NETIF_REMOVE_CALLBACK */
  /** This field can be set by the device driver and could point
   *  to state information for the device. */
  void *state;
#if LWIP_DHCP
  /** the DHCP client state information for this netif */
  struct dhcp *dhcp;
#endif /* LWIP_DHCP */
#if LWIP_IPV6_DHCP6
  /** the DHCPv6 client state information for this netif */
  struct dhcp6 *dhcp6;
#endif /* LWIP_IPV6_DHCP6 */
#if LWIP_AUTOIP
  /** the AutoIP client state information for this netif */
  struct autoip *autoip;
#endif
#if LWIP_NETIF_HOSTNAME
  /* the hostname for this netif, NULL is a valid value */
  char*  hostname;
#endif /* LWIP_NETIF_HOSTNAME */

#if LWIP_IPV6_AUTOCONFIG
  /** is this netif enabled for IPv6 autoconfiguration */
  u8_t ip6_autoconfig_enabled;
#endif /* LWIP_IPV6_AUTOCONFIG */
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
  /** Number of Router Solicitation messages that remain to be sent. */
  u8_t rs_send;
  /** interval for Router Solicitation messages that sent. */
  u8_t rs_count;
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */
  /** number of bytes used in hwaddr */
  u8_t hwaddr_len;
  /** link level hardware address of this interface */
  u8_t hwaddr[NETIF_MAX_HWADDR_LEN];
  /** descriptive abbreviation */
  char name[2];

  /** maximum transfer unit (in bytes) */
  u16_t mtu;  
  /** maximum transfer unit for IPv6(in bytes) */
  u16_t mtu6;

  /* recorder the tcpdump copy length, if set 0, do not do tcpdump */
  u16_t tcpdump_len;
  u16_t tcpdump_aligned;

  /** flags (see NETIF_FLAG_ above) */
  u32_t flags;
  /** flags (see NETIF_FLAG_EX above) */
  u32_t flagsEx;

  u32_t if_apin_pkts; /** record pkts check in for this netif marked by app */
  u32_t if_apout_pkts; /** record pkts output for this netif marked by app */
  u32_t if_in_pkts; /** record pkts in for this netif*/
  u32_t if_out_pkts; /** record pkts output for this netif*/
  u32_t if_in_discards; /** record pkts in discard for this netif*/
  u32_t if_out_discards; /** record pkts output discard for this netif*/
  u32_t if_lte_pkts; /** record pkts from lte for this netif process*/

#if LWIP_SNMP
  /** link type (from "snmp_ifType" enum from snmp.h) */
  u8_t link_type;
  u8_t link_type_aligned[3]; /*only for aligned bytes using*/
  /** (estimate) link speed */
  u32_t link_speed;
  /** timestamp at last change made (up/down) */
  u32_t ts;
  /** counters */
  u32_t ifinoctets;
  u32_t ifinucastpkts;
  u32_t ifinnucastpkts;
  u32_t ifindiscards;
  u32_t ifoutoctets;
  u32_t ifoutucastpkts;
  u32_t ifoutnucastpkts;
  u32_t ifoutdiscards;
#endif /* LWIP_SNMP */
#if LWIP_IGMP
  /** This function could be called to add or delete an entry in the multicast
      filter table of the ethernet MAC.*/
  netif_igmp_mac_filter_fn igmp_mac_filter;
#endif /* LWIP_IGMP */
#if LWIP_IPV6 && LWIP_IPV6_MLD
  /** This function could be called to add or delete an entry in the IPv6 multicast
      filter table of the ethernet MAC. */
  netif_mld_mac_filter_fn mld_mac_filter;
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
#if LWIP_NETIF_HWADDRHINT
  u8_t *addr_hint;
#endif /* LWIP_NETIF_HWADDRHINT */
#if ENABLE_LOOPBACK
  /* List of packets to be queued for ourselves. */
  struct pbuf *loop_first;
  struct pbuf *loop_last;
#if LWIP_LOOPBACK_MAX_PBUFS
  u16_t loop_cnt_current;
  u16_t loop_cnt_current_aligned; /*only for aligned bytes using*/
#endif /* LWIP_LOOPBACK_MAX_PBUFS */
#endif /* ENABLE_LOOPBACK */

#if LWIP_IPV6
  void *nd6_opt_pf;
  void *nd6_opt_dns;
  void *nd6_opt_pf_old;
  u16_t oldPfEnableFlg;    //old prefix exit: 0, for disable; 1, enable
  u16_t oldPfRaTmrSet;     //old prefix send ra timer
#endif

  /*define for vowifi netif tune, for netif_tune, this data is real*/
  /*for other netif, thid data not valid, or only transfer pointer, no need to free*/
  struct udp_pcb *tune_pcb;
  struct udp_pcb *tune_pcb6;
  struct netif_tune_buf *tune_buf;

  u32_t crtick; /*recorder the ostick when this netif is create.*/
}__attribute__((packed));

struct netif_tune_buf {
  struct netif *netif_turn; /*real netif*/
  struct netif *netif_tune; /*tune netif*/
  void *hint_entry;
  void *payload;
  ipX_addr_t src;
  ipX_addr_t dest;
  u16_t payload_len;
  u16_t reserved;

  u8_t direction; /*LWIP_DL_DIRECTION (0) LWIP_UL_DIRECTION (1)*/
  u8_t tune_proto; /*IPPROTO_IP, IPPROTO_UDP, IPPROTO_TCP*/
  u16_t tune_sport;
  u16_t tune_dport;

  u8_t ip_type;/*4: denote ipv4; 6: denote ipv6*/
  u8_t proto;
  u8_t ttl;
  u8_t tos;
};

#if LWIP_SNMP
#define NETIF_INIT_SNMP(netif, type, speed) \
  /* use "snmp_ifType" enum from snmp.h for "type", snmp_ifType_ethernet_csmacd by example */ \
  (netif)->link_type = (type);    \
  /* your link speed here (units: bits per second) */  \
  (netif)->link_speed = (speed);  \
  (netif)->ts = 0;              \
  (netif)->ifinoctets = 0;      \
  (netif)->ifinucastpkts = 0;   \
  (netif)->ifinnucastpkts = 0;  \
  (netif)->ifindiscards = 0;    \
  (netif)->ifoutoctets = 0;     \
  (netif)->ifoutucastpkts = 0;  \
  (netif)->ifoutnucastpkts = 0; \
  (netif)->ifoutdiscards = 0
#else /* LWIP_SNMP */
#define NETIF_INIT_SNMP(netif, type, speed)
#endif /* LWIP_SNMP */


/** The list of network interfaces. */
extern struct netif *netif_list;
/** The default network interface. */
extern struct netif *netif_default;
extern struct netif *netif_default_ip6;
extern struct netif *netif_default_ppp;

#if LWIP_HAVE_LOOPIF
struct netif * netif_loop_get_if(void);
#endif
void netif_loop_if_init(void);
struct netif *netif_add(struct netif *netif, ip_addr_t *ipaddr, ip_addr_t *netmask,
                            ip_addr_t *gw, void *state, netif_init_fn init, netif_input_fn input);
void netif_set_addr(struct netif *netif, ip_addr_t *ipaddr,
                            ip_addr_t *netmask, ip_addr_t *gw);
void netif_remove(struct netif * netif);
err_t netif_remove_config(struct netif * netif);
err_t netif_remove_config_ip6(struct netif * netif);
err_t netif_remove_config_all(struct netif * netif);
void netif_do_ip_addr_changed(const ip_addr_t *old_addr, const ip_addr_t *new_addr, int isIPv6);

/* Returns a network interface given its name. The name is of the form
   "et0", where the first two letters are the "name" field in the
   netif structure, and the digit is in the num field in the same
   structure. */
struct netif *netif_find_by_name(char *name);
struct netif *netif_find_by_hostname(char *hostname);
struct netif *netif_find_by_num(int num);
struct netif *netif_find_by_ipaddr(u32_t addr);
struct netif *netif_find_by_ipaddr_ex(u32_t addr, int simid);
struct netif *netif_find_by_ip6addr(ip6_addr_t *addr);
struct netif *netif_find_by_ip6prefix(ip6_addr_t *addr);
struct netif *netif_find_by_cid(char cid);
struct netif *netif_find_by_pcid(char cid);
struct netif *netif_find_by_scid(char cid);
struct netif *netif_find_by_cid_dsim(char simid, char cid);
struct netif *netif_find_by_pcid_dsim(char simid, char cid);
struct netif *netif_find_by_scid_dsim(char simid, char cid);
#if 0
err_t netif_init_done(struct netif * netif);
err_t netif_ready(struct netif * netif);
#endif
void netif_set_default(struct netif *netif);
void netif_set_default_ip6(struct netif *netif);
void netif_set_default_nw_ppp(struct netif *netif);
void netif_remove_default(struct netif *netif);
void netif_remove_default_ip6(struct netif *netif);
void netif_remove_default_nw_ppp(struct netif *netif);
struct netif* netif_get_usb_if(void);
struct netif* netif_get_uap_if(void);
struct netif* netif_get_ppp_if(void);
struct netif* netif_get_eth_if(void);
struct netif* netif_get_internal_if(void);
#if 0
struct netif* netif_get_default_if(void);
struct netif* netif_get_default_if6(void);
struct netif* netif_get_default_ifppp(void);
struct netif* netif_get_default_nw(void);
struct netif* netif_get_default_nw_ip6(void);
struct netif* netif_get_default_nw_ppp (void);
u8_t netif_check_nw_default(struct netif *inp);
u8_t netif_check_nw_default_ip6(struct netif *inp);
u8_t netif_check_nw_if(struct netif *inp);
#endif
u8_t netif_check_nw_status(void);
u8_t netif_check_nw_status_ip6(void);
u8_t netif_get_default_nw_status(void);
u8_t netif_get_default_nw_status_ip6(void);
void netif_set_default_nw_status(u8_t flag);
void netif_set_default_nw_status_ip6(u8_t flag);
void netif_set_mac(struct netif *netif, char* mac_addr);
void netif_set_ipaddr(struct netif *netif, ip_addr_t *ipaddr);
void netif_set_netmask(struct netif *netif, ip_addr_t *netmask);
void netif_set_gw(struct netif *netif, ip_addr_t *gw);
void netif_set_dns1(struct netif* netif, ip_addr_t *addr);
void netif_set_dns2(struct netif* netif, ip_addr_t *addr);
void netif_set_dns(struct netif* netif, ip_addr_t *addr1, ip_addr_t *addr2);
void netif_set_up(struct netif *netif);
void netif_set_down(struct netif *netif);
void netif_set_lan(struct netif *netif);
void netif_set_wan(struct netif *netif);
err_t netif_add_nat(struct netif * netif);
err_t netif_remove_nat(struct netif * netif);
err_t netif_get_status(struct netif * netif); //can use macro netif_is_up to replace
err_t netif_get_status_by_cid(char cid);
err_t netif_get_status_by_cid_dsim(char simid, char cid);
err_t netif_set_bypass(char simid, char cid, netif_bypass_fn bypass);
err_t netif_remove_bypass(char simid, char cid);
err_t netif_share_nw_exist(struct netif *inp);
err_t netif_nw_check_by_ipaddr(u32_t addr, s32_t type);
u8_t netif_local_if_exist(void);
u8_t netif_bridge_if_exist(void);
u8_t netif_special_if_check(struct netif *netif);
void netif_set_client_mode(struct netif *netif);
void netif_set_host_mode(struct netif *netif);
void netif_set_hdhcp_mode(struct netif *netif);

int netif_usb_get_status(void);
err_t netif_usb_set_status(u8_t status);
int netif_uap_get_status(void);
err_t netif_uap_set_status(u8_t status);
int netif_ppp_get_status(void);
err_t netif_ppp_set_status(u8_t status);

#if LWIP_NETIF_TUNE
err_t netif_tune_input(struct pbuf *p, struct netif *inp);
err_t netif_tune_buf_alloc(struct netif *inp);
void netif_tune_buf_free(struct netif *inp);
#endif

/** Ask if an interface is up */
#define netif_is_init_done(netif)            (((netif)->flags & NETIF_FLAG_INIT_DONE) ? 1 : 0)
#define netif_is_up(netif)                   (((netif)->flags & NETIF_FLAG_UP) ? 1 : 0)
#define netif_is_ready(netif)                ((netif_is_init_done(netif) && netif_is_up(netif)) ? ERR_OK : ERR_IF)
#define netif_ready(netif)                    netif_is_ready(netif)
#define netif_init_done(netif)               (netif_is_init_done(netif) ? ERR_OK : ERR_IF)

/** kind of if, external, internal, iftune, or ethernet type*/
#define netif_is_external(netif)             (((netif)->flags & NETIF_FLAG_EXTER) ? 1 : 0)
#define netif_is_internal(netif)             (((netif)->flags & NETIF_FLAG_INTER) ? 1 : 0)
#define netif_is_iftune(netif)               (((netif)->flags & NETIF_FLAG_IF_TUNE ) ? 1 : 0)
#define netif_is_ethernet(netif)             (((netif)->flags & NETIF_FLAG_ETHERNET) ? 1 : 0)
#define netif_is_nat_cfg(netif)              (((netif)->flags & NETIF_FLAG_NAT_CONFIG) ? 1 : 0)

/** kind of if detail interface type*/
#define netif_is_IF_USB(netif)               (((netif)->flags & NETIF_FLAG_IF_USB) ? 1 : 0)
#define netif_is_IF_UAP(netif)               (((netif)->flags & NETIF_FLAG_IF_UAP) ? 1 : 0)
#define netif_is_IF_PPP(netif)               (((netif)->flags & NETIF_FLAG_IF_PPP ) ? 1 : 0)
#define netif_is_IF_PDN(netif)               (((netif)->flags & NETIF_FLAG_IF_PDN ) ? 1 : 0)
#define netif_is_IF_TUNE(netif)              (((netif)->flags & NETIF_FLAG_IF_TUNE ) ? 1 : 0)
#define netif_is_IF_LOOP(netif)              (((netif)->flags & NETIF_FLAG_IF_LOOP ) ? 1 : 0)
#define netif_is_IF_ETH(netif)               (((netif)->flags & NETIF_FLAG_IF_ETH) ? 1 : 0)

#define netif_is_ip4_set(netif)              (((netif)->flagsEx & NETIF_FLAG_EX_IPV4_CFG) ? 1 : 0)
#define netif_is_ip6_set(netif)              (((netif)->flagsEx & NETIF_FLAG_EX_IPV6_CFG) ? 1 : 0)
#define netif_is_ip4_cfg(netif)              (((netif)->flagsEx & NETIF_FLAG_EX_IPV4_CFG) ? 1 : 0)
#define netif_is_ip6_cfg(netif)              (((netif)->flagsEx & (NETIF_FLAG_EX_RA_DONE \
                                                                 | NETIF_FLAG_EX_DHCP6_PD \
                                                                 | NETIF_FLAG_EX_IPV6_OTA \
                                                                 | NETIF_FLAG_EX_IPV6_IFCONFIG \
                                                                 )) ? 1 : 0)
#define netif_is_epsid_cfg(netif)            (((netif)->flagsEx & NETIF_FLAG_EX_EPSID_CFG) ? 1 : 0)
#define netif_is_normal_pdp(netif)           (((netif)->flagsEx & NETIF_FLAG_EX_NORMAL_PDP) ? 1 : 0)
#define netif_is_ppp_pdp(netif)              (((netif)->flagsEx & NETIF_FLAG_EX_PPP_PDP) ? 1 : 0)
#define netif_is_mbim_pdp(netif)             (((netif)->flagsEx & NETIF_FLAG_EX_MBIM_PDP) ? 1 : 0)
#define netif_is_app_pdp(netif)              (((netif)->flagsEx & NETIF_FLAG_EX_APP_PDP) ? 1 : 0)
#define netif_is_ims_pdp(netif)              (((netif)->flagsEx & NETIF_FLAG_EX_IMS_PDP) ? 1 : 0)
#define netif_is_pass_pdp(netif)             (((netif)->flagsEx & NETIF_FLAG_EX_PASS_PDP ) ? 1 : 0)
#define netif_is_default_pdp(netif)          (((netif)->flagsEx & NETIF_FLAG_EX_DEFAULT_PDP ) ? 1 : 0)

#define netif_is_dhcp_done(netif)            (((netif)->flags & NETIF_FLAG_DHCP_DONE ) ? 1 : 0)
#define netif_clear_dhcp_done_flag(netif)    ((netif)->flags &= ~NETIF_FLAG_DHCP_DONE)
#define netif_set_dhcp_done_flag(netif)      ((netif)->flags |= NETIF_FLAG_DHCP_DONE)
#define netif_clear_exter_flag(netif)        ((netif)->flags &= ~NETIF_FLAG_EXTER)
#define netif_set_exter_flag(netif)          ((netif)->flags |= NETIF_FLAG_EXTER)
#define netif_clear_inter_flag(netif)        ((netif)->flags &= ~NETIF_FLAG_INTER)
#define netif_set_inter_flag(netif)          ((netif)->flags |= NETIF_FLAG_INTER)

#define netif_clear_epsid_cfg(netif)         ((netif)->flagsEx &= ~NETIF_FLAG_EX_EPSID_CFG)
#define netif_set_epsid_cfg(netif)           ((netif)->flagsEx |= NETIF_FLAG_EX_EPSID_CFG)

#define netif_get_mac(netif)                 ((netif)->hwaddr)
#define netif_get_ipaddr(netif)              (&((netif)->ip_addr))
#define netif_get_ipaddr_u32(netif)          ((netif)->ip_addr.addr)
#define netif_get_gateway(netif)             (&((netif)->gw))
#define netif_get_netmask(netif)             (&((netif)->netmask))
#define netif_get_dns_server_n(netif, n)     (&((netif)->dns_server[n])) /*becareful: n < 2*/
#define netif_get_dns_server1(netif)         (&((netif)->dns_server[0]))
#define netif_get_dns_server2(netif)         (&((netif)->dns_server[1]))
#define netif_set_epsid(netif, value)        ((netif)->epsid = (value))
#define netif_get_epsid(netif)               ((netif)->epsid)
#define netif_get_cid(netif)                 ((netif)->cid)
#define netif_get_ref(netif)                 ((netif)->ref)
#define netif_set_ip4_proto(netif, value)    ((netif)->ip_proto = (value))
#define netif_get_ip4_proto(netif)           ((netif)->ip_proto)
#define netif_set_ip6_proto(netif, value)    ((netif)->ip6_proto = (value))
#define netif_get_ip6_proto(netif)           ((netif)->ip6_proto)
#define netif_set_mtu(netif, value)          ((netif)->mtu = (value))
#define netif_get_mtu(netif)                 ((netif)->mtu)
#define netif_set_mtu6(netif, value)         ((netif)->mtu6 = (value))
#define netif_get_mtu6(netif)                ((netif)->mtu6)
#define netif_set_tcpdump_len(netif, len)    ((netif)->tcpdump_len = (len))
#define netif_get_tcpdump_len(netif)		 ((netif)->tcpdump_len)

#define netif_inc_ifltekts(netif)            ((netif)->if_lte_pkts++)
#define netif_inc_ifinpkts(netif)            ((netif)->if_in_pkts++)
#define netif_inc_ifoutpkts(netif)           ((netif)->if_out_pkts++)
#define netif_inc_ifindiscards(netif)        ((netif)->if_in_discards++)
#define netif_inc_ifoutdiscards(netif)       ((netif)->if_out_discards++)
#define netif_inc_ifapinpkts(netif)          ((netif)->if_apin_pkts++)
#define netif_inc_ifapoutkts(p, netif)       do {\
                                                if (0 == (p)->ifnum) {\
                                                    (netif)->if_apout_pkts++;\
                                                }\
                                             } while(0);

#if LWIP_NETIF_STATUS_CALLBACK
void netif_set_status_callback(struct netif *netif, netif_status_callback_fn status_callback);
#endif /* LWIP_NETIF_STATUS_CALLBACK */
#if LWIP_NETIF_REMOVE_CALLBACK
void netif_set_remove_callback(struct netif *netif, netif_status_callback_fn remove_callback);
#endif /* LWIP_NETIF_REMOVE_CALLBACK */

void netif_set_link_up(struct netif *netif);
void netif_set_link_down(struct netif *netif);
/** Ask if a link is up */
#define netif_is_link_up(netif) (((netif)->flags & NETIF_FLAG_LINK_UP) ? (u8_t)1 : (u8_t)0)

#if LWIP_NETIF_LINK_CALLBACK
void netif_set_link_callback(struct netif *netif, netif_status_callback_fn link_callback);
#endif /* LWIP_NETIF_LINK_CALLBACK */

#if LWIP_NETIF_HOSTNAME
#define netif_set_hostname(netif, name) do { if((netif) != NULL) { (netif)->hostname = name; }}while(0)
#define netif_get_hostname(netif) (((netif) != NULL) ? ((netif)->hostname) : NULL)
#endif /* LWIP_NETIF_HOSTNAME */

#if LWIP_IGMP
#define netif_set_igmp_mac_filter(netif, function) do { if((netif) != NULL) { (netif)->igmp_mac_filter = function; }}while(0)
#define netif_get_igmp_mac_filter(netif) (((netif) != NULL) ? ((netif)->igmp_mac_filter) : NULL)
#endif /* LWIP_IGMP */

#if ENABLE_LOOPBACK
err_t netif_loop_output(struct netif *netif, struct pbuf *p);
void netif_poll(struct netif *netif);
#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
void netif_poll_all(void);
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */

void lwip_netif_tcpdump_fn(struct netif * netif, u8_t * data, u16_t len, u8_t isIpPkt);
#define lwip_netif_tcpdump(netif, data, len, isIpPkt) do { \
                                                        if ((netif)->tcpdump_len > 0) { \
                                                            lwip_netif_tcpdump_fn(netif, data, len, isIpPkt); \
                                                        } \
                                                      }while(0);

#if LWIP_IPV6
#define netif_clear_ip6addr_ota_flag(netif)         ((netif)->flagsEx &= ~NETIF_FLAG_EX_IPV6_OTA)
#define netif_set_ip6addr_ota_flag(netif)           ((netif)->flagsEx |= NETIF_FLAG_EX_IPV6_OTA)
#define netif_is_ip6addr_ota_done(netif)            (((netif)->flagsEx & NETIF_FLAG_EX_IPV6_OTA) ? 1 : 0)
#define netif_clear_ip6addr_ifconfig(netif)         ((netif)->flagsEx &= ~NETIF_FLAG_EX_IPV6_IFCONFIG)
#define netif_set_ip6addr_ifconfig(netif)           ((netif)->flagsEx |= NETIF_FLAG_EX_IPV6_IFCONFIG)
#define netif_is_ip6addr_ifconfig(netif)            (((netif)->flagsEx & NETIF_FLAG_EX_IPV6_IFCONFIG) ? 1 : 0)
#define netif_is_ra_done(netif)              		(((netif)->flagsEx & NETIF_FLAG_EX_RA_DONE) ? 1 : 0)
#define netif_is_nd6_dns_done(netif)         		(((netif)->flagsEx & NETIF_FLAG_EX_ND6_DNS) ? 1 : 0)
#define netif_is_dhcp6_dns_done(netif)       		(((netif)->flagsEx & NETIF_FLAG_EX_DHCP6_DNS) ? 1 : 0)
#define netif_is_dhcp6_pd_done(netif)        		(((netif)->flagsEx & NETIF_FLAG_EX_DHCP6_PD) ? 1 : 0)
#define netif_get_backup_ip6addr(netif)      		(&((netif)->ip6_addr_backup))
#define netif_get_local_ip6addr(netif)      		(&((netif)->ip6_addr[0]))
#define netif_get_global_ip6addr(netif)      		(&((netif)->ip6_addr[1]))
#define netif_get_site_ip6addr(netif)      		    (&((netif)->ip6_addr[2]))
#define netif_get_ip6_gateway(netif)                (&((netif)->ip6_gw))
#define netif_get_dns6_server_n(netif, n)    		(&((netif)->dns6_server[n])) /*becareful: n < 2*/
#define netif_get_dns6_server1(netif)        		(&((netif)->dns6_server[0]))
#define netif_get_dns6_server2(netif)        		(&((netif)->dns6_server[1]))
#define netif_ip6_addr(netif, i)                    (&((netif)->ip6_addr[(i)]))
#define netif_ip6_addr_state(netif, i)              ((netif)->ip6_addr_state[(i)])
#define netif_ip6_addr_set_state(netif, i, state)   ((netif)->ip6_addr_state[(i)] = (state))

s8_t netif_matches_ip6_addr(struct netif * netif, ip6_addr_t * ip6addr);
void netif_create_ip6_linklocal_address(struct netif * netif, u8_t from_mac_48bit);
void netif_set_dns_ipv6(struct netif* netif, ip6_addr_t *addr1, ip6_addr_t *addr2);
void netif_set_ip6_global(struct netif *netif, ip6_addr_t * ip6addr, u32_t set_pos);
void netif_set_ip6_global_prefix(struct netif *netif, u32_t addr1, u32_t addr2, u32_t set_pos);
void netif_set_ip6_linklocal(struct netif * netif, u32_t addr1, u32_t addr2);
struct netif * netif_nw_route_mifi(struct pbuf * p, u32_t ip_src);
struct netif * netif_nw_route_dongle(struct pbuf * p, u32_t ip_src);
struct netif * netif_nw6_route_dongle(struct pbuf * p, ip6_addr_t *ip6_src);
struct netif * netif_nw_router_check(struct netif *inp, ip_addr_t *dest);
struct netif * netif_nw6_router_check(struct netif *inp, ip6_addr_t *dest);

#endif /* LWIP_IPV6 */

static inline int netif_check_nw_if(struct netif *inp)
{
    return((inp 
        && netif_is_external(inp) 
        && !netif_is_pass_pdp(inp)) ? 1 : 0);
}

static inline int netif_check_nw_default(struct netif *inp)
{
    return((inp && (inp == netif_default)) ? 1 : 0);
}

static inline int netif_check_nw_default_ip6(struct netif *inp)
{   
    return((inp && (inp == netif_default_ip6)) ? 1 : 0);
}

static inline void netif_set_default_if(struct netif *netif)
{
    if (netif == NULL) {return;}

    /*do remove first*/
    if (netif_default
        && (netif_default != netif)) {
        netif_remove_default(netif_default);
    }

    /*assign new default netif*/
    netif_default = netif;
}

static inline void netif_set_default_if6(struct netif *netif)
{
    if (netif == NULL) {return;}

    /*do remove first*/
    if (netif_default_ip6
        && (netif_default_ip6 != netif)) {
        netif_remove_default_ip6(netif_default_ip6);
    }

    /*assign new default netif*/
    netif_default_ip6 = netif;
}

static inline struct netif* netif_get_default_if(void)
{
    return((netif_default == NULL) ? NULL : netif_default);
}

static inline struct netif* netif_get_default_if6(void)
{
    return((netif_default_ip6 == NULL) ? NULL : netif_default_ip6);
}

static inline struct netif* netif_get_default_ifppp(void)
{
    return((netif_default_ppp == NULL) ? NULL : netif_default_ppp);
}

static inline struct netif* netif_get_default_nw(void)
{
    return(((netif_default == NULL)
        || (!netif_is_up(netif_default))
        || (!netif_is_ip4_cfg(netif_default))) ? NULL : netif_default);
}

static inline struct netif* netif_get_default_nw_ip6(void)
{
    return(((netif_default_ip6 == NULL)
        || (!netif_is_up(netif_default_ip6))
        || (!netif_is_ip6_cfg(netif_default_ip6))) ? NULL : netif_default_ip6);
}

static inline struct netif* netif_get_default_nw_ppp (void)
{
    return(((netif_default_ppp == NULL) 
        || (!netif_is_up(netif_default_ppp))) ? NULL : netif_default_ppp);
}

#if LWIP_NETIF_HWADDRHINT
#define NETIF_SET_HWADDRHINT(netif, hint) ((netif)->addr_hint = (hint))
#else /* LWIP_NETIF_HWADDRHINT */
#define NETIF_SET_HWADDRHINT(netif, hint)
#endif /* LWIP_NETIF_HWADDRHINT */

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_NETIF_H__ */
