/**
 * @file
 *
 * IPv6 address autoconfiguration as per RFC 4862.
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
 * IPv6 address autoconfiguration as per RFC 4862.
 *
 * Please coordinate changes and requests with Ivan Delamer
 * <delamer@inicotech.com>
 */

#ifndef __LWIP_IP6_DHCP6_H__
#define __LWIP_IP6_DHCP6_H__

#include "opt.h"

#if LWIP_IPV6_DHCP6  /* don't build if not configured for use in lwipopts.h */

struct dhcp6
{
  /*TODO: implement DHCP6*/
};

typedef struct _dhcp6_msg
{
    u8_t    type;
    u8_t    xxid[3];
    u32_t   opts[1];
}__attribute__((packed)) dhcp6_msg;

typedef struct _dhcp6_opt
{
    u16_t   type;
    u16_t   size;
}__attribute__((packed)) dhcp6_opt;

typedef struct _dhcp6_opt_time
{
    u16_t   type; // 8
    u16_t   size; // 2
    u16_t   time;
}__attribute__((packed)) dhcp6_opt_time;

typedef struct _dhcp6_opt_xid
{
    u16_t   type;   // 1 for cid, 2 for sid
    u16_t   size;   // 14
    u16_t   duid;   // 1
    u16_t   hdwr;   // 1 for Ethernet,  6 for IEEE 802, network order
    u32_t   time;   // time stamp, network order
    u8_t    mac[6]; // mac address
}__attribute__((packed)) dhcp6_opt_xid;

typedef struct _dhcp6_opt_request
{
    u16_t   type; // 6
    u16_t   size; // 2
    u16_t   code[1];  // 23 for ip address of dns server
}__attribute__((packed)) dhcp6_opt_request;

typedef struct _dhcp6_opt_dns
{
    u16_t   type;       // 23
    u16_t   size;       // 32
    u32_t   dns1[4];    // dns server 1
    u32_t   dns2[4];    // dns server 2
}__attribute__((packed)) dhcp6_opt_dns;

typedef struct _dhcp6_opt_domain
{
    u16_t   type;       // 24
    u16_t   size;       // 32
    u8_t    domain[32]; // domain search list
}__attribute__((packed)) dhcp6_opt_domain;

typedef struct _dhcp6_opt_pd
{
    u16_t   type;       // 25
    u16_t   size;       // 12
    u32_t   iaid;       // unique id
    u32_t   t1;         // 0xffffffff
    u32_t   t2;         // 0xffffffff
}__attribute__((packed)) dhcp6_opt_pd;

typedef struct _dhcp6_opt_ia
{
    u16_t   type;       // 26
    u16_t   size;       // 25
    u32_t   pt;         // preferred lifetime
    u32_t   vt;         // valid lifetime
    u8_t    len;        // prefix length
    u32_t   prefix[4];  // prefix address
}__attribute__((packed)) dhcp6_opt_ia;

typedef struct _dhcp6_opt_status
{
    u16_t   type;       // 13
    u16_t   size;       // 24
    u16_t   code;       // 0 for success
    u8_t    msg[1];
}__attribute__((packed)) dhcp6_opt_status;

typedef struct _dhcp6_opt_ip6
{
    u16_t   type;       // 3
    u16_t   size;       // 12 or 66
    u32_t   iaid;       // 1
    u32_t   t1;         //
    u32_t   t2;         //
}__attribute__((packed)) dhcp6_opt_ip6;

typedef struct _dhcp6_opt_ip6_addr
{
    u16_t   type;       // 5
    u16_t   size;       // 24
    u32_t   ip6[4];
    u32_t   t1;
    u32_t   t2;
}__attribute__((packed)) dhcp6_opt_ip6_addr;

typedef struct _dhcp6_opt_preference
{
    u16_t   type;       // 7
    u16_t   size;       // 1
    u8_t    val;        // 0
}__attribute__((packed)) dhcp6_opt_preference;

#define dhcp6_find_opt_dns(opt, tail) dhcp6_find_opt((opt), 0x1700, (tail))
#define dhcp6_find_opt_sid(opt, tail) dhcp6_find_opt((opt), 0x0200, (tail))
#define dhcp6_find_opt_cid(opt, tail) dhcp6_find_opt((opt), 0x0100, (tail))
#define dhcp6_find_opt_pd(opt, tail) dhcp6_find_opt((opt), 0x1900, (tail))
#define dhcp6_find_opt_ia(opt, tail) dhcp6_find_opt((opt), 0x1a00, (tail))
#define dhcp6_find_opt_status(opt, tail) dhcp6_find_opt((opt), 0x0d00, (tail))
#define dhcp6_find_opt_ip6(opt, tail) dhcp6_find_opt_2((opt), 0x0300, (tail))
#define dhcp6_find_opt_ip6_addr(opt, tail) dhcp6_find_opt_2((opt), 0x0500, (tail))

dhcp6_opt *dhcp6_find_opt(u8_t *opt_in, u16_t type, u16_t len);
dhcp6_opt *dhcp6_find_opt_2(u8_t *opt_in, u16_t type, u8_t *opt_end);
int dhcp6_reply_opt_dns(dhcp6_opt_dns *opt);
int dhcp6_reply_opt_ip6(dhcp6_opt_ip6 *opt, u16_t len, u32_t iaid);
int dhcp6_reply_opt_ip6_addr(dhcp6_opt_ip6_addr *opt, ip6_addr_t *ip6);
int dhcp6_reply_opt_status(dhcp6_opt_status *opt, u16_t val);
int dhcp6_reply_opt_prefer(dhcp6_opt_preference *opt);
int dhcp6_reply_opt_xid(dhcp6_opt_xid *opt, u16_t id, u8_t *mac);
int dhcp6_reply_opt_domain(dhcp6_opt_domain *opt);
void dhcp6_stop(struct netif *netif);
err_t dhcp6_start(struct netif *netif);

#else
#define dhcp6_stop(inp)
#define dhcp6_start(inp)

#endif /* LWIP_IPV6_DHCP6 */

#endif /* __LWIP_IP6_DHCP6_H__ */
