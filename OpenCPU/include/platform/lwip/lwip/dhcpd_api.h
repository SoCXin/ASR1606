/** @file
 */

#ifndef __LWIP_DHCPD_API_H__
#define __LWIP_DHCPD_API_H__

#include "opt.h"
#include "netif.h"
#include "sys_arch.h"

#undef HAVE_LEASE_TIME
#define HAVE_LEASE_TIME 1

#undef HOST_NAME_LEN
#define HOST_NAME_LEN   40

#ifndef LWIP_DHCPD_MAXLEASES
#define LWIP_DHCPD_MAXLEASES 253
#endif

#ifndef time_t
typedef unsigned int time_t;
#endif

#ifndef in_addr_t
typedef u32_t in_addr_t;
#endif

#ifndef bool
typedef u32_t bool;
#endif

#define DHCP_HTYPE_ETHERNET       1
#define DHCP_MAC_LEN              6

#ifdef __cplusplus
extern "C" {
#endif

struct lease_s
{
    u8_t     mac[DHCP_MAC_LEN];       /* MAC address (network order) -- could be larger! */
    bool     allocated;               /* true: IP address is allocated */
    bool     is_static;               /* true: IP address is static for mac addr */
#ifdef HAVE_LEASE_TIME
    time_t   expiry;                  /* Lease expiration time (seconds past Epoch) */
#endif
};

struct dhcpmsg_s
{
    u8_t  op;
    u8_t  htype;
    u8_t  hlen;
    u8_t  hops;
    u8_t  xid[4];
    u16_t secs;
    u16_t flags;
    u8_t  ciaddr[4];
    u8_t  yiaddr[4];
    u8_t  siaddr[4];
    u8_t  giaddr[4];
    u8_t  chaddr[16];
    u8_t  sname[64];
    u8_t  file[128];
    u8_t  options[256];
}__attribute__((packed));


typedef struct dhcpd_state_s
{
    /* Server configuration, host bytes mode */
    u32_t            ds_serverip;     /* The server IP address */
    u32_t            ds_netmask;      /* The server netmask */
    u32_t            ds_dhcp_ips;     /* dhcp ip pool start point */
    u32_t            ds_dhcp_ipe;     /* dhcp ip pool end point */
    u32_t            ds_ls_tm;        /* dhcp lease time in senconds */

    /* Message buffers */
    struct dhcpmsg_s ds_inpacket;     /* Holds the incoming DHCP client message */
    struct dhcpmsg_s ds_outpacket;    /* Holds the outgoing DHCP server message */

    /* Parsed options from the incoming DHCP client message */
    u8_t             ds_optmsgtype;   /* Incoming DHCP message type */
    in_addr_t        ds_ciaddr;       /* client IP address (host order) */
    in_addr_t        ds_optreqip;     /* Requested IP address (host order) */
    in_addr_t        ds_optserverip;  /* Serverip IP address (host order) */
    time_t           ds_optleasetime; /* Requested lease time (host order) */

    /* End option pointer for outgoing DHCP server message */
    u8_t            *ds_optend;

    /* Leases */
    struct lease_s   ds_leases[LWIP_DHCPD_MAXLEASES];
}dhcpd_state_s;

#if 0
typedef struct mac_ip_t
{
    u32_t   ipaddr;
    u8_t    macaddr[6];
    char    hostname[HOST_NAME_LEN];
}mac_ip_t;
#endif

typedef struct dongle_config_info
{
    u32_t   dongle_ipaddr;
    u32_t   dongle_netmask;
    u32_t   dongle_gw;
    u32_t   dongle_dns1;
    u32_t   dongle_dns2;
    u32_t   dongle_flag; /*0, invalid; 1, config; 2, detect mode.*/
    u32_t   dongle_detect;
    struct netif * netif_nw; /*record dongle dhcp config get from netif_nw*/
    struct netif * netif_lc; /*record dongle dhcp coming from netif*/
    char    dongle_mac_addr[DHCP_MAC_LEN];
}dongle_config_info_s;

extern dongle_config_info_s g_dongle_config_info;
extern u32_t                g_server_ip; // Router server ip

/*api function or inline function declare*/
void dhcpd_set_dongle_info(struct netif *netif);
u8_t dhcpd_check_dongle_ip(u32_t ipaddr);
u8_t dhcpd_check_dongle_gw(u32_t ipaddr);
u8_t dhcpd_check_dongle_dns1(u32_t ipaddr);
u8_t dhcpd_check_dongle_dns2(u32_t ipaddr);
u8_t dhcpd_check_dongle_mask(u32_t ipaddr);
void dhcpd_remove_dongle_info(void);
void dhcpd_trigger_dongle_info(void);
void dhcpd_hostdhcp_config(struct netif * inp);
struct netif * dhcpd_ip_route_netif(void);
dongle_config_info_s* dhcpd_get_dongle_info(char *macadd);
dongle_config_info_s* dhcpd_get_dongle_info_api(void);

// return router ip address in network order
static inline u32_t dhcpd_router_ip(void)
{
    return htonl(g_server_ip);
}

static inline u32_t dhcpd_get_ip(void)
{
    return g_server_ip;
}

#ifdef __cplusplus
}
#endif

#endif /*__LWIP_DHCP_H__*/
