/*************************************************************************
*filename: lwip_api.h
*description: support api to config or set lwip sub system, or get infomation from lwip sub system
*history:
*-------2018-11-02, change from lwip_stats.h to lwip_api.h
**************************************************************************/

#ifndef __LWIP_API_PROVIDER_H__
#define __LWIP_API_PROVIDER_H__

#include "opt.h"
#include "arch.h"
#include "pbuf.h"
#include "ip_nat.h"
#include "Dialer_Task.h"

typedef struct {
    struct netif * default_nw;
    struct netif * default_nw_ip6;
    struct netif * default_nw_ip6_old;

    /*set flag to control ul/dl lte path*/
    u8_t lte_DLpath_enable;  /*0: disable; 1, enable*/
    u8_t lte_ULpath_enable;  /*0: disable; 1, enable*/
    u8_t lwip_iot_test_mode; /*0: normal mode; 1, iot test mode*/
    u8_t lwip_platform_mode; /*0, for pcie dongle; 1, for mifi mode; 2, for bridge mode, based on dongle. */

    u8_t dongle_trigger_nat_flag; /*0: disable; 1, enable*/
	u8_t reserved;
    /*set flag to control ipv6 address ota config method, redm#9225*/
    u8_t ota_ip6addr_rsra_flag;  /*default 0, not do rs&ra, if ota ip6addr config, using this flag to control rs&ra*/
    u8_t ota_ip6addr_using_flag; /*default 0, update ip6addr, if ota ip6addr conflict with rs&ra process result*/

    /*only deal default ip6 nw check problem*/
    /*only in case: default_nw_ip6 changed*/
    void * default_nw_nd6_opt_pf;
    void * default_nw_nd6_opt_pf_old;
    u16_t default_nw_oldPfEnableFlg;    //old prefix exit: 0, for disable; 1, enable
    u16_t default_nw_oldPfRaTmrSet;     //old prefix send ra timer

	u16_t tcpdump_len;		/*lwip tcpdump copy length.*/
	u16_t pading16;
}lwip_system_ctrl_st;

typedef enum
{
  LWIP_NETIF_IF_STATUS_DISABLE   = 0, /* netif down or disable*/
  LWIP_NETIF_IF_STATUS_ACTIVE    = 1, /* netif up or active*/
} lwip_netif_if_status_e;

typedef enum
{
  LWIP_NETIF_IF_HOST_MODE   = 0,   /* netif host mode set, default set for it, config to lan, set by server ip*/
  LWIP_NETIF_IF_CLIENT_MODE = 1,   /* netif client mode set, will get ipconfig from uplink, and config to wan interface */
  LWIP_NETIF_IF_HDHCP_MODE  = 2,   /* netif host mode set, config to lan, but set by dhcp*/
} lwip_netif_if_mode_e;

/*Note: ipv4 and ipv6 both need config to same mode, host or client*/
/* but, can using different client config when in client mode. */
typedef enum
{
  LWIP_NETIF_IP_PROTOCOL_STATIC,     /* ipv4 client mode, need config ip by ifconfig */
  LWIP_NETIF_IP_PROTOCOL_DHCP,       /* ipv4 client mode, will get ip by dhcp */
  LWIP_NETIF_IP6_PROTOCOL_SLAAC,     /* ipv6 client mode, need config ip by ifconfig */
  LWIP_NETIF_IP6_PROTOCOL_DHCP6,     /* ipv6 client mode, will get ip by dhcp */
} lwip_netif_protocol_type_e;

/*only lwip internal call using inline, for outside call using function*/
extern lwip_system_ctrl_st lwip_system_ctrl;
static inline int lwip_get_lte_dlpath_switch(void) {return lwip_system_ctrl.lte_DLpath_enable;}
static inline int lwip_get_lte_ulpath_switch(void) {return lwip_system_ctrl.lte_ULpath_enable;}
static inline int lwip_get_dongle_trigger_nat(void) {return lwip_system_ctrl.dongle_trigger_nat_flag;}

/******************************
*function: lwip_get_netif_*****
*input:
*output:
*       NULL, no if exist
*       char *, netif if name
*******************************/
const char* lwip_get_netif_pc_ifname(void);
const char* lwip_get_netif_wifi_ifname(void);
const char* lwip_get_netif_usb_ifname(void);
const char* lwip_get_netif_uap_ifname(void);
const char* lwip_get_netif_eth_ifname(void);
const char* lwip_get_netif_lte_ifname(void);
const char* lwip_get_netif_lte6_ifname(void);
const char* lwip_get_default_nw_ifname(void);
const char* lwip_get_default_nw6_ifname(void);
struct netif * lwip_get_netif_pdp_if(char simid, char cid);
struct netif * lwip_get_netif_by_hostname(const char * ifname);

/*Note: for int return, 0: denote netif for network is ready; < 0: denote not ready.*/
int lwip_get_nw_status(const char* ifname);
int lwip_get_nw_status_ip6(const char* ifname);

/*Note: for int return, 0: denote success; < 0: denote failed.*/
int lwip_set_default_nw_ifname(const char * ifname );
int lwip_set_default_nw6_ifname(const char * ifname );

/******************************
*function: lwip_ifexist
*input:
*       char* netif_name, the name of netif, list as: cid_4, pc, wlan, local
*
*output:
*       0, not exist
*       1, exist
*******************************/
int lwip_ifexist(char* netif_name);

/******************************
*function: lwip_ifconfig
*input:
*       char* netif_name, the name of netif, list as: cid_4, pc, wlan, local
*       char* config_name, the name of config, list as: ip, gateway, netmask, dns1, dns2
*       char* config_value, the value string, if ip addr, like 8.8.8.8
*
*output:
*       0, if success full
*       -1, if failed
*******************************/
int lwip_ifconfig(char* netif_name, char* config_name, char* config_value);
int lwip_ifconfig_value(char* netif_name, char* config_name, int config_value);
int lwip_ifconfig_dongle(char* netif_name, char* config_name, char* config_value);
int lwip_dhcpd_ifconfig(char* if_name, char* config_name, char* config_value);
int lwip_dhcpd_dialconfig(char* if_name, char* config_name, char* config_value);

/******************************
*function: lwip_iflist
*input:
*       char* netif_name, the name of netif, list as: cid_4, pc, wlan, local
*       char* list_name, the name to list, list as: ip, gateway, netmask, dns1, dns2
*       char* list_value, the value string return, if ip addr, like 8.8.8.8
*
*output:
*       0, if success full
*       -1, if failed
*
*Note:
*       char* list value, is the return string, becareful the size for caller, the least size suggust to 64 bytes
*******************************/
int lwip_iflist(char* netif_name, char* list_name, char* list_value);
int lwip_iflist_value(char* netif_name, char* list_name, int* list_value);
int lwip_iflist_dongle(char* netif_name, char* list_name, char* list_value);
int lwip_dhcpd_iflist(char* if_name, char* list_name, char* list_value);

#if IS_LWIP_PLATFORM_NBIOT
void set_mifi_dns_name(char* str);
void lwip_manual_set_pmod(u8_t pmod);
void lwip_rndis_hotplug_link(void);
void lwip_do_rndis_hotplug_now(void);
void lwip_rndis_hotplug_break(void);
err_t pppnet_GetPacket_cb(u8_t *data, u32_t  len, pmsg* msg);
#endif

int lwip_set_tcpip_rate(char * rate_name, int rate_val); /*return 0, denote success*/
void lwip_set_tcpip_rate_default(void);
int lwip_ltemem_info_print(char * buf, int size_set);
int lwip_pbuf_info_print(char * buf, int size_set);
int lwip_print_tcpip_rate_all(char *buf, int size_set);
int lwip_print_tcpip_rate_default(char *buf, int size_set);
#if IP_NAT
int lwip_port_fwd_rule_add(port_fwd_list *item);
#endif
/**
* mtu: need set to what mtu value
* netif_name: which netif will config for this mtu, if set NULL, will config all netif pdp mtu
*/
void lwip_set_netif_pdp_mtu(u16_t mtu, char cid);
void lwip_set_netif_pdp_mtu_dsim(u16_t mtu, char *netif_name);
u16_t lwip_get_netif_pdp_mtu(void);
void lwip_set_dns_relay_port(u16_t value);
u16_t lwip_get_dns_relay_port(void);
void lwip_set_wan_access_port(u16_t value);
u16_t lwip_get_wan_access_port(void);
void lwip_set_wan_access_web_flag(int flag);
int lwip_get_wan_access_web_flag(void);
void lwip_set_wan_ping_access_flag(int flag);
int lwip_get_wan_ping_access_flag(void);
void lwip_set_mifi_flag(u8_t mode);
u8_t lwip_get_mifi_flag(void);
#if IS_LWIP_PLATFORM_CRANE
void lwip_set_mallocEx_free_rate(int rate);
int lwip_get_mallocEx_free_rate(void);
#endif
int lwip_get_ip_forward_flag(void);
void lwip_set_ip_forward_flag(int flag);
void lwip_set_usb_forward_flag(int flag);
int lwip_get_usb_forward_flag(void);
void lwip_set_wifi_forward_flag(int flag);
int lwip_get_wifi_forward_flag(void);
void lwip_set_app_input_flag(int flag);
int lwip_get_app_input_flag(void);
void lwip_set_loopif_input_flag(int flag);
int lwip_get_loopif_input_flag(void);
void lwip_set_send_ra_all_flag(int flag);
int lwip_get_send_ra_all_flag(void);
void lwip_set_check_192_168_dongle(int flag);
int lwip_get_check_192_168_dongle(void);

/*api for control rs&ra process*/
void lwip_set_ota_ip6addr_rsra_flag(int flag);
int lwip_get_ota_ip6addr_rsra_flag(void);
void lwip_set_ota_ip6addr_using_flag(int flag);
int lwip_get_ota_ip6addr_using_flag(void);
void lwip_set_rs_retry_counter(int cnt);
void lwip_set_rs_backoff(u8_t * backoff_cfg, int size_n);
void lwip_set_rs_backoff_default(void);
void lwip_get_rs_backoff(u8_t * backoff_read, int size_n);

void lwip_set_dns_api_cache_flag(int flag);
int lwip_get_dns_api_cache_flag(void);
void lwip_set_mac_filter_flag(int flag);
int lwip_get_mac_filter_flag(void);
void lwip_set_dnsr_hijack_flag(int flag);
int lwip_get_dnsr_hijack_flag(void);
void lwip_set_man_set_dnsr_flag(int flag);
int lwip_get_man_set_dnsr_flag(void);
void lwip_set_solinger_sp1_flag(int flag);
int lwip_get_solinger_sp1_flag(void);
void lwip_set_arp_mask_match_flag(int value);
int lwip_get_arp_mask_match_flag(void);
void lwip_set_dhcpd_server_flag(int value);
int lwip_get_dhcpd_server_flag(void);
void lwip_set_reuseaddr_sp1_flag(int flag);
int lwip_get_reuseaddr_sp1_flag(void);
void lwip_set_dhcpd_local_dns_flag(int value);
int lwip_get_dhcpd_local_dns_flag(void);
void lwip_set_ra_dns_opt_flag(int value);
int lwip_get_ra_dns_opt_flag(void);
void lwip_set_dhcpd_gw_flag(int value);
int lwip_get_dhcpd_gw_flag(void);
void lwip_set_rndis_hotplug_flag(int value);
int lwip_get_rndis_hotplug_flag(void);
void lwip_set_adjust_pp_flag(int flag);
int lwip_get_adjust_pp_flag(void);
void lwip_set_nat_hash_dump_flag(int flag);
int lwip_get_nat_hash_dump_flag(void);

void lwip_set_tcp_max_rtx(int max_rtx);
void lwip_set_tcp_syn_max_rtx(int max_rtx);
int lwip_get_tcp_max_rtx(void);
int lwip_get_tcp_syn_max_rtx(void);

/*Note: tcp_rcv_win, bytes number config*/
/*if return 0, denote success ; return other , denote failed.*/
int lwip_set_tcp_rcv_win(u32_t value);
u32_t lwip_get_tcp_rcv_win(void);
int lwip_set_tcp_rcv_scale(u8_t value);
u8_t lwip_get_tcp_rcv_scale(void);

/*Note: tcp_snd_win, bytes number config*/
/*if return 0, denote success ; return other , denote failed.*/
int lwip_set_tcp_snd_win(u32_t value);
u32_t lwip_get_tcp_snd_win(void);

/* Note: tcp msl set, using microsecond unit. */
/* minvalue is 1000ms; */
/* default for tcp_msl_val= 60000ms */
void lwip_set_tcp_msl_val(u32_t value);
u32_t lwip_get_tcp_msl_val(void);

/*Note: value, using ms unit. and 500ms one step; minvalue is 500ms*/
/*for RTO = value / 500, will round the value config.*/
/*default value set to 1000 ms.*/
void lwip_set_tcp_rto_update_value(int value);
int lwip_get_tcp_rto_update_value(void);

/*Note: flag set to 0 or 1, 0 for disable; 1 for enable*/
/*default set to 0, disable this function.*/
void lwip_set_tcp_rto_update_flag(int flag);
int lwip_get_tcp_rto_update_flag(void);

/*Note: value, using ms unit. and 500ms one step; minvalue is 500ms*/
void lwip_set_tcp_rto_init(u32_t value);
void lwip_get_tcp_rto_init(u32_t * pvalue);

void lwip_set_tcp_backoff(u8_t * backoff_cfg, int size_n); /*max size=13*/
/*Note: restore the default table config { 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7};*/
void lwip_set_tcp_backoff_default(void);
void lwip_get_tcp_backoff(u8_t * backoff_read, int size_n); /*max size=13*/

void lwip_set_tcp_sync_backoff(u8_t * backoff_cfg, int size_n); /*max size=13*/
/*Note: { 1, 1, 1, 3, 3, 6, 12, 24, 48, 96, 120, 120, 120} as default; s is unit*/
void lwip_set_tcp_sync_backoff_default(void);
void lwip_get_tcp_sync_backoff(u8_t * backoff_read, int size_n); /*max size=13*/

void lwip_set_tcp_persist_backoff(u8_t * backoff_cfg, int size_n); /*max size=7*/
void lwip_set_tcp_persist_backoff_default(void); /*restore the default table config { 3, 6, 12, 24, 48, 96, 120 }*/
void lwip_get_tcp_persist_backoff(u8_t * backoff_read, int size_n); /*max size=7*/

u32_t lwip_get_dns_max_wait_time(void);
void lwip_set_dns_max_retries(u32_t value);  /*max value is 8, default set to 4*/
u32_t lwip_get_dns_max_retries(void);  /*max value is 8, default set to 4*/
void lwip_set_dns_wait_tmr(u8_t * wait_cfg, int size_n); 
void lwip_set_dns_wait_tmr_default(void);  /* defaul set to { 2, 2, 4, 8, 8, 8, 8, 8}; unit is s*/
void lwip_get_dns_wait_tmr(u8_t * wait_read, int size_n);
void lwip_set_dns_bind_port(u16_t port);
u16_t lwip_get_dns_bind_port(void);
void lwip_set_ip_reass_maxage(u8_t value); /*unit: second*/
u8_t lwip_get_ip_reass_maxage(void);
void lwip_set_ip_default_ttl(u8_t value);
u8_t lwip_get_ip_default_ttl(void);

void lwip_system_ctrl_init(void);
void * lwip_system_ctrl_get_opt_pf(void);
void * lwip_system_ctrl_get_opt_pf_old(void);
u16_t lwip_system_ctrl_default_pfCfg(u8_t opMode, struct netif * inp, void * nd6_prefix_opt);
void lwip_print_version_info(void);

void lwip_set_temp_flag1(int flag);
int lwip_get_temp_flag1(void);
void lwip_set_temp_flag2(int flag);
int lwip_get_temp_flag2(void);
void lwip_set_temp_flag3(int flag);
int lwip_get_temp_flag3(void);
void lwip_set_dongle_detect_cnt(int flag);
int lwip_get_dongle_detect_cnt(void);

/*********************************/
/*  api for control lwip_system_ctrl switch*/
/*********************************/
void lwip_set_lte_path_switch(int flag);
void lwip_set_lte_dlpath_switch(int flag);
void lwip_set_lte_ulpath_switch(int flag);
void lwip_set_dongle_directly_switch(int flag);
int lwip_get_lte_path_switch(void);
int lwip_get_dongle_directly_switch(void);
void lwip_set_dongle_trigger_nat(int flag);

#if 0
int lwip_man_trigger_app_query(void);
void lwip_man_trigger_app_enable(void);
void lwip_man_trigger_app_remove(void);
#endif

/******************************
*function: lwip_set_dns_relay_server
*input:
*       char* dns_name, the name of dns relay server, list as: dnsr_1, dnsr_2
*       char* config_value, the value string, if ip addr, like 8.8.8.8
*output:
*       0, if success full
*       -1, if failed
********************************/
int lwip_set_dns_relay_server(char* dnsr_name, char* config_value);

/******************************
*function: lwip_get_dns_relay_server
*input:
*       char* dns_name, the name of dns relay server, list as: dnsr_1, dnsr_2
*       char* list_value, the value string return, if ip addr, like 8.8.8.8
*output:
*       0, if success full
*       -1, if failed
********************************/
int lwip_get_dns_relay_server(char* dnsr_name, char* list_value);

/******************************
*function: lwip_reset_dns_relay_server
*description: reset dns relay server by default netif dns settings
*input:
*       void
*output:
*       0, if success full
*       -1, if failed
********************************/
int lwip_reset_dns_relay_server(void);

/******************************
*function: lwip_lte_require_check
*input:
*output:
*       0, no need lte data access require
*       1, need lte data access
*******************************/
int lwip_lte_require_check(void);

/*************************************************
* API function for lwip self call define here
*************************************************/
int lwip_do_dns_query(char *netifname, char *hostname, int ipv4v6, void *result);
int lwip_do_dns_query_ex(char *netifname, char *hostname, char *servname, int ipv4v6, void *result);
int lwip_manul_adjust_pp(int index);
void lwip_manual_set_pmod(u8_t pmod);
void lwip_do_rndis_hotplug_now(void);
void lwip_do_rndis_hotplug(void);
void lwip_rndis_hotplug_break(void);
void lwip_rndis_hotplug_link(void);
int lwip_check_forward_for_pbuf(struct pbuf* p);

void lwip_debug_dumppbufs(char *prefix, unsigned char *data, int offs, int length);
int lwip_set_debug_setting(char *modu_name, u8_t onoff, u8_t print_level);
int lwip_get_debug_setting_by_name(char *modu_name, u8_t *onoff, u8_t *print_level);
void lwip_disable_all_debug_switch(void);
void lwip_set_default_debug_setting(void);
void lwip_enable_all_debug_switch(void);
void lwip_enable_tcp_debug_switch(void);
void lwip_enable_iptrace_debug_switch(void);

void lwip_iot_test_mode_env(u8_t iot_mode);
int lwip_get_iot_test_mode(void);

void lwip_set_iot_test_mode(u8_t value);
void lwip_psm_get_wrapper(char *para, char *para_value, int *max_size);
void lwip_psm_set_wrapper(char *para, char *para_value);
int lwip_get_val_from_flash(char *para);

char * lwip_build_netif_pdp_ifname(char simid, char cid);
int lwip_select_netif_ready(int event, char *netifname, u32_t mstimeout);
int lwip_select_default_nw_ready(int event, u32_t mstimeout);

#if !LWIP_NO_STATIC
/*************************************************
* API function for lwip data statics call define here
*************************************************/
/* return speed in Bytes per second */
u32_t lwip_nwst_get_current_speed(void);
/* return used data in MBytes + Bytes */
void lwip_nwst_get_used_data(u8_t simid, u32_t* pused_MByte, u32_t* pused_Byte);
void lwip_nwst_clear_used_data(u8_t simid);
/** note: cutoff_data, unit is Mbyte , if config 0, denote disable cutoff_data check */
void lwip_nwst_set_cutoff_data(u8_t simid, u32_t cutoff_data);
/** note: reminder_data, unit is Mbyte , if config 0, denote disable reminder_data check */
void lwip_nwst_set_reminder_data(u8_t simid, u32_t reminder_data);
/** note: nwst_callback_fn cb, if config NULL, denote disable callback */
void lwip_nwst_set_callback(u8_t simid, nwst_callback_fn cb);
void lwip_nwst_get_cutoff_data(u8_t simid, u32_t* cutoff_data);
void lwip_nwst_get_reminder_data(u8_t simid, u32_t* reminder_data);

#if 0 /*disable update flash, jingquan, 20190913*/
/** note: flash update parameter set and get api */
/** default set: enable flag = 1, update time = 10m, update thread mbyte = 100M*/
void lwip_nwst_set_update_flash_flag(int flag);
int lwip_nwst_get_update_flash_flag(void);
void lwip_nwst_set_update_flash_time(int minitue);
int lwip_nwst_get_update_flash_time(void);
void lwip_nwst_set_update_flash_mbyte(int mbyte);
int lwip_nwst_get_update_flash_mbyte(void);
#endif

#endif

/*
function: lwip_set_mac_nwif_table
input:
    char *mac, 	format as XX:XX:XX:XX:XX:XX or XX-XX-XX-XX-XX-XX 
    struct netif * nwif, need call lwip_get_netif_pdp_if to get;
    int * flag, set and write back for IP type;

for flag, now use to denote ipv4v6:
#define IP_TYPE_IPV4V6			0x3
#define IP_TYPE_IPV4			0x1
#define IP_TYPE_IPV6			0x2
#define IP_TYPE_LOCAL			0x4
after config table, will write back the config flag;

for flag, special deal:
if flag = 0, remove this netif setting for mac-apn paire.
*/
err_t lwip_set_mac_nwif_table(char *mac, struct netif * nwif, int * flag);

/*func: lwip_down_mac_nwif_table
without mac input, this api will find mac-apn for all who matched nwif config.
this api will auto called when apn netif remove.
for customer, can use this api to remove all mac-apn for matched nwif

for flag, now use to denote ipv4v6:
#define IP_TYPE_IPV4V6			0x3
#define IP_TYPE_IPV4			0x1
#define IP_TYPE_IPV6			0x2
#define IP_TYPE_LOCAL			0x4
*/
void lwip_down_mac_nwif_table(struct netif * nwif, int flag);


#endif
