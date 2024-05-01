#ifndef __TFT_H__
#define __TFT_H__

#include "lwipopts.h"

#if IS_LWIP_PLATFORM_CRANE
#include "pdpdef.h"  /*define TftInfoList_CM*/
#else
#include "Dialer_Task.h"
#include "teldef.h"

typedef struct TftInfoList_st   TftInfoList_CM;
#endif

int tft_new_tft(TftInfoList_CM *);
int tft_ulrate_control(u8_t protocol, u8_t epsid, struct pbuf *p, u8_t* psendFlag);
int tft_route_from_default(struct pbuf *p, struct netif *inp);
int tft_route_from_default_ip6(struct pbuf *p, struct netif *inp);
err_t tft_route_from_app(struct pbuf *p, struct netif *inp);
err_t tft_route_from_app_ip6(struct pbuf *p, struct netif *inp);
#if 0
struct netif* tft_route_from_local(struct pbuf *p, u32_t src_ip);
struct netif* tft_route_from_local_ip6(struct pbuf *p, ip6_addr_t *src);
#endif

#endif

