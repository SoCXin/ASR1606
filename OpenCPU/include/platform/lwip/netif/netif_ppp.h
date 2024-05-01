#ifndef _NETIF_PPP_HDR_H_
#define _NETIF_PPP_HDR_H_

#include "netif.h"
#include "pbuf.h"
#include "err.h"
#include "lwipopts.h"

#if (LWIP_NETIF_PPP || LWIP_NETIF_MBIM)
void netif_ppp_init(void);
void netif_ppp_remove(void);
void netif_ppp_reset_ip(u32_t ipaddr);
int pppnet_downlink_bypass(struct netif * netif, struct pbuf * p);
int pppnet_downlink_bypass_ip6(struct netif * netif, struct pbuf * p);
err_t pppnet_input(struct pbuf *p, struct netif *inp);
err_t pppnet_GetPacket_cb(u8_t *data, u32_t  len, pmsg* msg);
#else

#endif

#endif
