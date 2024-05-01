/*
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
 * Author: Simon Goldschmidt
 *
 */
#ifndef __LWIP_NETDB_H__
#define __LWIP_NETDB_H__

#include "opt.h"

#if LWIP_DNS && LWIP_SOCKET

#include <stddef.h> /* for size_t */

#include "inet.h"
#include "sockets.h"
#include "api_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* some rarely used options */
#ifndef LWIP_DNS_API_DECLARE_H_ERRNO
#define LWIP_DNS_API_DECLARE_H_ERRNO 1
#endif

#ifndef LWIP_DNS_API_DEFINE_ERRORS
#define LWIP_DNS_API_DEFINE_ERRORS 1
#endif

#ifndef LWIP_DNS_API_DECLARE_STRUCTS
#define LWIP_DNS_API_DECLARE_STRUCTS 1
#endif

#ifndef LWIP_DNS_API_DEFINE_FLAGS
#define LWIP_DNS_API_DEFINE_FLAGS     1
#endif

#if LWIP_DNS_API_DEFINE_ERRORS
/** Errors used by the DNS API functions, h_errno can be one of them */
#define EAI_NONAME      200
#define EAI_SERVICE     201
#define EAI_FAIL        202
#define EAI_MEMORY      203
#define EAI_FAMILY      204

#define HOST_NOT_FOUND  210
#define NO_DATA         211
#define NO_RECOVERY     212
#define TRY_AGAIN       213
#endif /* LWIP_DNS_API_DEFINE_ERRORS */

#if LWIP_DNS_API_DEFINE_FLAGS
/* input flags for struct addrinfo */
#define AI_PASSIVE      0x01
//#define AI_CANONNAME    0x02
#define AI_NUMERICHOST  0x04
//#define AI_NUMERICSERV  0x08
//#define AI_V4MAPPED     0x10
//#define AI_ALL          0x20
//#define AI_ADDRCONFIG   0x40

#endif /* LWIP_DNS_API_DEFINE_FLAGS */

#if LWIP_DNS_API_DECLARE_STRUCTS
struct hostent {
    char  *h_name;      /* Official name of the host. */
    char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    int    h_addrtype;  /* Address type. */
    int    h_length;    /* The length, in bytes, of the address. */
    char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
};

struct addrinfo {
    int               ai_flags;      /* Input flags. */
    int               ai_family;     /* Address family of socket. */
    int               ai_socktype;   /* Socket type. */
    int               ai_protocol;   /* Protocol of socket. */
    socklen_t         ai_addrlen;    /* Length of socket address. */
    struct sockaddr  *ai_addr;       /* Socket address of socket. */
    char             *ai_canonname;  /* Canonical name of service location. */
    struct addrinfo  *ai_next;       /* Pointer to next in list. */
};
#endif /* LWIP_DNS_API_DECLARE_STRUCTS */

#if LWIP_DNS_API_DECLARE_H_ERRNO
/* application accessable error code set by the DNS API functions */
extern int h_errno;
#endif /* LWIP_DNS_API_DECLARE_H_ERRNO*/

struct hostent *lwip_gethostbyname(const char *name);
struct hostent *lwip_gethostbyname_with_pcid(const char *name, u8_t pcid);
struct hostent *lwip_gethostbyname_with_netif(const char *name, const char *netif_name);

int lwip_gethostbyname_r(const char *name, struct hostent *ret, char *buf,
                                    size_t buflen, struct hostent **result, int *h_errnop);
int lwip_gethostbyname_r_with_pcid(const char *name, struct hostent *ret, char *buf,
                                    size_t buflen, struct hostent **result, int *h_errnop, u8_t pcid);
int lwip_gethostbyname_r_with_netif(const char *name, struct hostent *ret, char *buf,
                                    size_t buflen, struct hostent **result, int *h_errnop, const char *netif_name);

int lwip_getaddrinfo(const char *nodename,
                           const char *servname,
                           const struct addrinfo *hints,
                           struct addrinfo **res);
int lwip_getaddrinfo_with_pcid(const char *nodename,
                                           const char *servname,
                                           const struct addrinfo *hints,
                                           struct addrinfo **res, u8_t pcid);
int lwip_getaddrinfo_with_netif(const char *nodename,
                                           const char *servname,
                                           const struct addrinfo *hints,
                                           struct addrinfo **res, const char *netif_name);

int lwip_getaddrinfo_r(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res,
       struct dns_ipMode_st *dns_ip_mode);

void lwip_freeaddrinfo(struct addrinfo *ai);

/*if success, return 0; if failed, return other.*/
int lwip_get_dns_ttl(const char *nodename, u32_t *ttl);

/*if success, return 0; if failed, return other.*/
int lwip_set_dns_ttl(const char *nodename, u32_t ttl);


#if LWIP_COMPAT_SOCKETS
#define gethostbyname(name) lwip_gethostbyname(name)
#define gethostbyname_with_pcid(name, pcid) lwip_gethostbyname_with_pcid(name, pcid)
#define gethostbyname_r(name, ret, buf, buflen, result, h_errnop) lwip_gethostbyname_r(name, ret, buf, buflen, result, h_errnop)
#define gethostbyname_r_with_pcid(name, ret, buf, buflen, result, h_errnop, pcid) lwip_gethostbyname_r_with_pcid(name, ret, buf, buflen, result, h_errnop, pcid)
#define gethostbyname_with_netif(name, netif_name) lwip_gethostbyname_with_netif(name, netif_name)
#define gethostbyname_r_with_netif(name, ret, buf, buflen, result, h_errnop, netif_name) lwip_gethostbyname_r_with_netif(name, ret, buf, buflen, result, h_errnop, netif_name)
#define freeaddrinfo(addrinfo) lwip_freeaddrinfo(addrinfo)
#define getaddrinfo(nodname, servname, hints, res) lwip_getaddrinfo(nodname, servname, hints, res)
#define getaddrinfo_with_pcid(nodname, servname, hints, res, pcid) lwip_getaddrinfo_with_pcid(nodname, servname, hints, res, pcid)       
#define getaddrinfo_with_netif(nodname, servname, hints, res, netif_name) lwip_getaddrinfo_with_netif(nodname, servname, hints, res, netif_name)       
#endif /* LWIP_COMPAT_SOCKETS */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DNS && LWIP_SOCKET */

#endif /* __LWIP_NETDB_H__ */
