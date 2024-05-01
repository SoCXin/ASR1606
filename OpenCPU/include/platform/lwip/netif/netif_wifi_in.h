#ifndef __NETIF_WIFI_IN_H__
#define __NETIF_WIFI_IN_H__

#if LWIP_NETIF_WF_UAP
#include "netif.h"

#if 0
int netif_recv_from_wifi(UINT8 *data, UINT32 len, UINT8 *buf);
int netif_wifi_status(int status);
int netif_wifi_init(int ip_addr, char* mac_addr);
int netif_send_to_wifi_complete(UINT8 *data);
#endif

#endif

#endif /* __NETIF_WIFI_IN_H__ */

