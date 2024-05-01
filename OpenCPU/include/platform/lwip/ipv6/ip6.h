/**
 * @file
 *
 * IPv6 layer.
 */

/*
 * Copyright (c) 2010 Inico Technologies Ltd.
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
 * Author: Ivan Delamer <delamer@inicotech.com>
 *
 *
 * Please coordinate changes and requests with Ivan Delamer
 * <delamer@inicotech.com>
 */
#ifndef __LWIP_IP6_H__
#define __LWIP_IP6_H__

#include "opt.h"

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "ip.h"
#include "ip6_addr.h"
#include "def.h"
#include "pbuf.h"
#include "netif.h"

#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IP6_HLEN 40
#define IP6_MIN_MTU_LENGTH  1280

#define IP6_NEXTH_IP4ENCAPS  4
#define IP6_NEXTH_IP6ENCAPS  41

#define IP6_NEXTH_HOPBYHOP  0
#define IP6_NEXTH_TCP       6
#define IP6_NEXTH_UDP       17
#define IP6_NEXTH_ENCAPS    41
#define IP6_NEXTH_ROUTING   43
#define IP6_NEXTH_FRAGMENT  44
#define IP6_NEXTH_GRE       47
#define IP6_NEXTH_ESP       50
#define IP6_NEXTH_AH        51
#define IP6_NEXTH_ICMP6     58
#define IP6_NEXTH_NONE      59
#define IP6_NEXTH_DESTOPTS  60
#define IP6_NEXTH_UDPLITE   136

#define IP6_FRAG_INFO_BACKUP_LEN        20

/* The IPv6 header. */
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip6_hdr {
  /* version / traffic class / flow label */
  PACK_STRUCT_FIELD(u32_t _v_tc_fl);
  /* payload length */
  PACK_STRUCT_FIELD(u16_t _plen);
  /* next header */
  PACK_STRUCT_FIELD(u8_t _nexth);
  /* hop limit */
  PACK_STRUCT_FIELD(u8_t _hoplim);
  /* source and destination IP addresses */
  PACK_STRUCT_FIELD(ip6_addr_p_t src);
  PACK_STRUCT_FIELD(ip6_addr_p_t dest);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "epstruct.h"
#endif
#define IP6H_V(hdr)  ((ntohl((hdr)->_v_tc_fl) >> 28) & 0x0f)
#define IP6H_TC(hdr) ((ntohl((hdr)->_v_tc_fl) >> 20) & 0xff)
#define IP6H_FL(hdr) (ntohl((hdr)->_v_tc_fl) & 0x000fffff)
#define IP6H_PLEN(hdr) (ntohs((hdr)->_plen))
#define IP6H_NEXTH(hdr) ((hdr)->_nexth)
#define IP6H_NEXTH_P(hdr) ((u8_t *)(hdr) + 6)
#define IP6H_HOPLIM(hdr) ((hdr)->_hoplim)

#define IP6H_VTCFL_SET(hdr, v, tc, fl) (hdr)->_v_tc_fl = (htonl(((v) << 28) | ((tc) << 20) | (fl)))
#define IP6H_PLEN_SET(hdr, plen) (hdr)->_plen = htons(plen)
#define IP6H_NEXTH_SET(hdr, nexth) (hdr)->_nexth = (nexth)
#define IP6H_HOPLIM_SET(hdr, hl) (hdr)->_hoplim = (u8_t)(hl)

/* ipv6 extended options header */
#define IP6_PAD1_OPTION             0
#define IP6_PADN_OPTION             1
#define IP6_ROUTER_ALERT_OPTION     5
#define IP6_JUMBO_OPTION            194
#define IP6_HOME_ADDRESS_OPTION     201
#define IP6_ROUTER_ALERT_DLEN       2
#define IP6_ROUTER_ALERT_VALUE_MLD  0

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip6_opt_hdr {
  /* router alert option type */
  PACK_STRUCT_FLD_8(u8_t _opt_type);
  /* router alert option data len */
  PACK_STRUCT_FLD_8(u8_t _opt_dlen);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif
#define IP6_OPT_HLEN 2
#define IP6_OPT_TYPE_ACTION(hdr) ((((hdr)->_opt_type) >> 6) & 0x3)
#define IP6_OPT_TYPE_CHANGE(hdr) ((((hdr)->_opt_type) >> 5) & 0x1)
#define IP6_OPT_TYPE(hdr) ((hdr)->_opt_type)
#define IP6_OPT_DLEN(hdr) ((hdr)->_opt_dlen)

/* Hop-by-Hop header. */
#define IP6_HBH_HLEN    2

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip6_hbh_hdr {
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* header length in 8-octet units */
  PACK_STRUCT_FLD_8(u8_t _hlen);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif
#define IP6_HBH_NEXTH(hdr) ((hdr)->_nexth)

/* Destination header. */
#define IP6_DEST_HLEN   2

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip6_dest_hdr {
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* header length in 8-octet units */
  PACK_STRUCT_FLD_8(u8_t _hlen);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif
#define IP6_DEST_NEXTH(hdr) ((hdr)->_nexth)

/* Routing header */
#define IP6_ROUT_TYPE2  2
#define IP6_ROUT_RPL    3

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip6_rout_hdr {
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* reserved */
  PACK_STRUCT_FLD_8(u8_t _hlen);
  /* fragment offset */
  PACK_STRUCT_FIELD(u8_t _routing_type);
  /* fragmented packet identification */
  PACK_STRUCT_FIELD(u8_t _segments_left);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif
#define IP6_ROUT_NEXTH(hdr) ((hdr)->_nexth)
#define IP6_ROUT_TYPE(hdr) ((hdr)->_routing_type)
#define IP6_ROUT_SEG_LEFT(hdr) ((hdr)->_segments_left)

/* Fragment header. */
#define IP6_FRAG_HLEN    8
#define IP6_FRAG_OFFSET_MASK    0xfff8
#define IP6_FRAG_MORE_FLAG      0x0001

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip6_frag_hdr {
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* reserved */
  PACK_STRUCT_FLD_8(u8_t reserved);
  /* fragment offset */
  PACK_STRUCT_FIELD(u16_t _fragment_offset);
  /* fragmented packet identification */
  PACK_STRUCT_FIELD(u32_t _identification);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "epstruct.h"
#endif

#define IP6_FRAG_NEXTH(hdr) ((hdr)->_nexth)
#define IP6_FRAG_MBIT(hdr) (ntohs((hdr)->_fragment_offset) & 0x1)
#define IP6_FRAG_ID(hdr) (ntohl((hdr)->_identification))

#define ip6_init() /* TODO should we init current addresses and header pointer? */
struct netif *ip6_route(struct ip6_addr *src, struct ip6_addr *dest);
struct netif *ip6_route_app(struct ip6_addr *src, struct ip6_addr *dest);
ip6_addr_t   *ip6_select_source_address(struct netif *netif, ip6_addr_t * dest);
err_t         ip6_input(struct pbuf *p, struct netif *inp);
err_t         ip6_output(struct pbuf *p, struct ip6_addr *src, struct ip6_addr *dest,
                         u8_t hl, u8_t tc, u8_t nexth);
err_t         ip6_output_if(struct pbuf *p, struct ip6_addr *src, struct ip6_addr *dest,
                            u8_t hl, u8_t tc, u8_t nexth, struct netif *netif);
#if LWIP_NETIF_HWADDRHINT
err_t         ip6_output_hinted(struct pbuf *p, ip6_addr_t *src, ip6_addr_t *dest,
                                u8_t hl, u8_t tc, u8_t nexth, u8_t *addr_hint);
#endif /* LWIP_NETIF_HWADDRHINT */
#if LWIP_IPV6_MLD
err_t         ip6_options_add_hbh_ra(struct pbuf * p, u8_t nexth, u8_t value);
#endif /* LWIP_IPV6_MLD */
err_t         ip6_get_proto_info(struct ip6_hdr *ip6hdr, void** proto_addr, int* proto_type);

#define ip6_netif_get_local_ip6(netif, dest)    (((netif) != NULL) ? ip6_select_source_address(netif, dest) : NULL)
#define ip6_netif_get_local_ipX(netif, dest)    (((netif) != NULL) ? ip6_2_ipX(ip6_select_source_address(netif, dest)) : NULL)
#define ip6_netif_get_global_ipX(netif)         (((netif) != NULL) ? ip6_2_ipX(netif_get_global_ip6addr(netif)) : NULL)

typedef struct 
{
        u16_t   ttl;
        u16_t   resvd;
        u32_t   identification;
        char    tcpudp_buffer [IP6_FRAG_INFO_BACKUP_LEN];        
}ip6_frag_info_backup_t;

#if LWIP_DEBUG
void ip6_debug_print(struct pbuf *p);
#else
#define ip6_debug_print(p)
#endif
void ip6_pbuf_log_input(struct netif *inp, struct pbuf *p);
void ip6_pbuf_log_output(struct netif *inp, struct pbuf *p);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV6 */

#endif /* __LWIP_IP6_H__ */
