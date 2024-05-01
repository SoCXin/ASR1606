#ifndef __LWIP_CUS_STATS_H__
#define __LWIP_CUS_STATS_H__

#include "arch.h"
#include "netif.h"
#include "sys_arch.h"
#include "dialer_task.h"

/** define lwip_nwst_event, using hotbit format, for bit using also */
#define	LTE_ST_EVENT_CUTOFF_DATA        (0x1)
#define	LTE_ST_EVENT_REMINDER_DATA      (0x2)

typedef struct _ip_flow_stats
{
    u32_t   bytes_rx;
    u32_t   bytes_tx;
    u32_t   bytes_er;
    u32_t   ip_addr;
    struct _ip_flow_stats *next;
} ip_flow_stats;

typedef struct _net_flow_stats
{
    u32_t           bytes_rx;
    u32_t           bytes_tx;
    u32_t           bytes_er;
    struct netif    *netif;
    struct _net_flow_stats *next;
} net_flow_stats;

typedef struct _hw_flow_stats
{
    u32_t   bytes_rx;
    u32_t   bytes_tx;
    u32_t   bytes_er;
    u8_t    hw_addr[6];
    struct _hw_flow_stats *next;
} hw_flow_stats;

void ip_flow_stats_rx(u32_t ip, u32_t bytes);
void ip_flow_stats_tx(u32_t ip, u32_t bytes);
void ip_flow_stats_er(u32_t ip, u32_t bytes);
ip_flow_stats *get_ip_flow_stats(void);


/**************************************************************************
 *                        Define static inline function                   *
 **************************************************************************/
u32_t netif_get_usb_packets(void);
u32_t netif_get_uap_packets(void);
u32_t netif_get_all_packets(void);
void netif_set_tx_info(statisticsData *ptr, u32_t len);
void netif_set_rx_info(statisticsData *ptr, u32_t len);
void netif_set_tx_error_info(statisticsData *ptr, u32_t len);
void netif_set_rx_error_info(statisticsData *ptr, u32_t len);
void lte_data_statistic_process(u8_t simid);
void lte_data_statistic_reset(u8_t simid);
void lte_data_statistic_get_used(u8_t simid, u32_t* pused_MByte, u32_t* pused_Byte);
void lte_data_statistic_query_used(u8_t simid, u32_t* pused_MByte, u32_t* pused_Byte);
void lte_data_statistic_set_event(u8_t simid, int event);
void lte_data_statistic_clear_event(u8_t simid, int event);

#endif
