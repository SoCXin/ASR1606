/**
 * lwip DNS resolver header file.

 * Author: Jim Pettinato 
 *   April 2007

 * ported from uIP resolv.c Copyright (c) 2002-2003, Adam Dunkels.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __LWIP_DNS_H__
#define __LWIP_DNS_H__

#include "opt.h"
#include "sys_arch.h"
#include "api_msg.h"

#if LWIP_DNS /* don't build if not configured for use in lwipopts.h */

#ifdef __cplusplus
extern "C" {
#endif

/** DNS timer period */
#define DNS_TMR_INTERVAL          1000

/** DNS maximum number of retries when asking for a name, before "timeout". */
#ifndef DNS_MAX_RETRIES
#define DNS_MAX_RETRIES           8
#endif
#ifndef DNS_MIN_RETRIES
#define DNS_MIN_RETRIES           2
#endif

/** DNS field TYPE used for "Resource Records" */
#define DNS_RRTYPE_A              1     /* a host address */
#define DNS_RRTYPE_NS             2     /* an authoritative name server */
#define DNS_RRTYPE_MD             3     /* a mail destination (Obsolete - use MX) */
#define DNS_RRTYPE_MF             4     /* a mail forwarder (Obsolete - use MX) */
#define DNS_RRTYPE_CNAME          5     /* the canonical name for an alias */
#define DNS_RRTYPE_SOA            6     /* marks the start of a zone of authority */
#define DNS_RRTYPE_MB             7     /* a mailbox domain name (EXPERIMENTAL) */
#define DNS_RRTYPE_MG             8     /* a mail group member (EXPERIMENTAL) */
#define DNS_RRTYPE_MR             9     /* a mail rename domain name (EXPERIMENTAL) */
#define DNS_RRTYPE_NULL           10    /* a null RR (EXPERIMENTAL) */
#define DNS_RRTYPE_WKS            11    /* a well known service description */
#define DNS_RRTYPE_PTR            12    /* a domain name pointer */
#define DNS_RRTYPE_HINFO          13    /* host information */
#define DNS_RRTYPE_MINFO          14    /* mailbox or mail list information */
#define DNS_RRTYPE_MX             15    /* mail exchange */
#define DNS_RRTYPE_TXT            16    /* text strings */
#define DNS_RRTYPE_AAAA			  28 /*ipv6 host address*/

/** DNS field CLASS used for "Resource Records" */
#define DNS_RRCLASS_IN            1     /* the Internet */
#define DNS_RRCLASS_CS            2     /* the CSNET class (Obsolete - used only for examples in some obsolete RFCs) */
#define DNS_RRCLASS_CH            3     /* the CHAOS class */
#define DNS_RRCLASS_HS            4     /* Hesiod [Dyer 87] */
#define DNS_RRCLASS_FLUSH         0x800 /* Flush bit */

/* The size used for the next line is rather a hack, but it prevents including socket.h in all files
   that include memp.h, and that would possibly break portability (since socket.h defines some types
   and constants possibly already define by the OS).
   Calculation rule:
//sizeof(struct addrinfo) + sizeof(struct sockaddr_in6) + DNS_MAX_NAME_LENGTH + 1 byte zero-termination */
#define NETDB_ELEM_SIZE           (32 + 24 + DNS_MAX_NAME_LENGTH + 1)
#if 0
//sizeof(struct addrinfo) + sizeof(struct sockaddr_in) + DNS_MAX_NAME_LENGTH + 1 byte zero-termination */
#define NETDB_ELEM_SIZE           (32 + 16 + DNS_MAX_NAME_LENGTH + 1)
#endif

#if DNS_LOCAL_HOSTLIST
/** struct used for local host-list */
struct local_hostlist_entry {
  /** static hostname */
  const char *name;
  /** static host address in network byteorder */
  ip_addr_t addr;
  struct local_hostlist_entry *next;
};
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
#ifndef DNS_LOCAL_HOSTLIST_MAX_NAMELEN
#define DNS_LOCAL_HOSTLIST_MAX_NAMELEN  DNS_MAX_NAME_LENGTH
#endif
#define LOCALHOSTLIST_ELEM_SIZE ((sizeof(struct local_hostlist_entry) + DNS_LOCAL_HOSTLIST_MAX_NAMELEN + 1))
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC */
#endif /* DNS_LOCAL_HOSTLIST */

/** Callback which is invoked when a hostname is found.
 * A function of this type must be implemented by the application using the DNS resolver.
 * @param name pointer to the name that was looked up.
 * @param ipaddr pointer to an ip_addr_t containing the IP address of the hostname,
 *        or NULL if the name could not be found (or on any other error).
 * @param callback_arg a user-specified callback argument passed to dns_gethostbyname
*/
typedef void (*dns_found_callback)(void *dns_entry, ip_addr_t *ipaddr);

/** DNS table entry */
struct dns_table_entry {
  u8_t  state;
  u8_t  numans;   /* numans < DNS_MAX_ANS_NUM_GET */
  u8_t  tmr;
  u8_t  retries;

  u8_t  seqno;
  u8_t  err;
  u8_t  ipMode; /* 0: NULL, not finish PPOE; 4: using IPv4; 6: using IPv6; */
  u8_t  dnsServ; /*dns server index, 0 or 1*/

  u32_t ipEnv;
  u32_t ttl;

  ip_addr_t ex_dnsserv_v4;
  ip6_addr_t ex_dnsserv_v6;

  char name[DNS_MAX_NAME_LENGTH];
  ip_addr_t ipaddr [DNS_MAX_ANS_NUM_GET];
  ip6_addr_t ip6addr [DNS_MAX_ANS_NUM_GET];

  struct udp_pcb *pcb; /*record send pcb, for check recv pcb next*/
  struct netif *netif; /*primary netif, denote for netif pdp*/

  /* pointer to callback on DNS query done */
  dns_found_callback found;
  void *arg;
  /** This semaphore copy from dns_api_msg, for abort case, do release sem. */
  sys_sem_t *sem;
  /** This tick and thread copy from dns_api_msg, for abort case, do mem check. */
  sys_thread_t debug_thread; 
  u32_t debug_tick;
};

void           dns_init(void);
void           dns_tmr(void);
err_t          dns_gethostbyname(const char *hostname, ip_addr_t *addr,
                                 dns_found_callback found, void *callback_arg);

#if DNS_LOCAL_HOSTLIST && DNS_LOCAL_HOSTLIST_IS_DYNAMIC
int            dns_local_removehost(const char *hostname, const ip_addr_t *addr);
err_t          dns_local_addhost(const char *hostname, const ip_addr_t *addr);
#endif /* DNS_LOCAL_HOSTLIST && DNS_LOCAL_HOSTLIST_IS_DYNAMIC */

u8_t dns_get_DNSserv(struct dns_ipMode_st *dns_ip_mode);
void dns_set_DNSserv(u8_t ipMode, u8_t index, struct dns_ipMode_st *dns_ip_mode);
void dns_set_IPmode(u8_t ipMode, struct dns_ipMode_st *dns_ip_mode);
u8_t dns_get_IPmode(struct dns_ipMode_st *dns_ip_mode);
u8_t dns_get_seqno(void);
struct netif * dns_get_netif(struct dns_ipMode_st *dns_ip_mode);
void dns_clear_setflg(struct dns_ipMode_st *dns_ip_mode);
void dns_set_exDNSserv(const char * servname, struct dns_ipMode_st *dns_ip_mode);
u8_t dns_check_IPEnv(struct dns_ipMode_st *dns_ip_mode);
u8_t dns_check_IPEnv4(struct dns_ipMode_st *dns_ip_mode);
u8_t dns_check_IPEnv6(struct dns_ipMode_st *dns_ip_mode);
void dns_set_IPEnv(const char *netif_name, struct dns_ipMode_st *dns_ip_mode);
void dns_set_default(struct dns_ipMode_st *dns_ip_mode);
void dns_table_flush(struct dns_table_entry *pEntry);
err_t dns_get_ttl(const char *name, u32_t *ttl_val);
err_t dns_set_ttl(const char *name, u32_t ttl_val);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DNS */

#endif /* __LWIP_DNS_H__ */
