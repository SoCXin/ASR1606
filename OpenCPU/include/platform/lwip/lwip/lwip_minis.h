/*************************************************************************
*filename: lwip_minis.h
*description: support mini system interface define
*history:
*-------20200423, create file.
**************************************************************************/

#ifndef __LWIP_MINIS_PROVIDER_H__
#define __LWIP_MINIS_PROVIDER_H__

#include "sys_arch.h"
#include "opt.h"
#include "pbuf.h"

void lwip_init_all(void);

#if LWIP_DNS_RELAY
void dnsr_set_serverip(u32_t addr1, u32_t addr2, u8_t setPos);
void dnsr_set_serverip_v6(ip6_addr_t *addr1, ip6_addr_t *addr2);
#endif
#if LWIP_DHCP_SERVER
void dhcpd_set_dongle_info(struct netif *netif);
#endif
#if LWIP_DHCP6_SERVER
void dhcp6d_set_dns6(ip6_addr_t *dns1, ip6_addr_t *dns2);
#endif

#if LWIP_IPSEC
err_t ipsec4_api_out_handle(struct pbuf *p, struct ip_addr *ipaddr, struct netif **inp, struct pbuf **p_new);
err_t ipsec4_api_in_handle(struct pbuf *p, struct netif *inp,struct pbuf **p_new);
err_t ipsec6_api_out_handle(struct pbuf *p, ip6_addr_t *ipaddr, struct netif **inp, struct pbuf **p_new);
err_t ipsec6_api_in_handle(struct pbuf *p, struct netif *inp, struct pbuf **p_new, u8_t nexth);
#endif

err_t ethernet_input(struct pbuf *p, struct netif *netif);
err_t pppnet_input(struct pbuf *p, struct netif *inp);
struct netif * dhcpd_ip_route_netif(void);

#endif
