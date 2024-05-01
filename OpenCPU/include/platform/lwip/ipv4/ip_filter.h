#ifndef __IP_FILTER_H__
#define __IP_FILTER_H__

#include "opt.h"
#include "pbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ip_info_t
{
    u16_t   src_port;       // src port,    0 - invalid
    u16_t   dst_port;       // dst port,    0 - invalid
    u32_t   src_ip;         // src ip,      0 - invalid
    u32_t   dst_ip;         // dst ip,      0 - invalid

}ip_info_t;


typedef struct _ip_elt_t
{
    u16_t   port_start;     // port start,  0 - invalid
    u16_t   port_end;       // port end,    0 - invalid
    u32_t   ip_addr;        // ip address,  0 - invalid
    u32_t   net_mask;       // ip net mask, 0 - invalid

}ip_elt_t;


typedef struct _ip_filter_t
{
    u8_t        flag;       // 0 - disable, 1 - enable
    u8_t        type;       // 1 - tcp, 2 - udp, 3 - tcp and udp

    ip_elt_t    src;        // source
    ip_elt_t    dst;        // destination

}ip_filter_t;


typedef struct _ip_filter_param_list
{
    unsigned char   enable_disable;     //0:disable 1:enable other:invalid
    unsigned int    src_ip;             //0: invalid
    unsigned char   net_mask1;          //0: invalid 24/32 mode
    unsigned short  src_port_start;     //0: invalid
    unsigned short  src_port_end;       //0: invalid
    unsigned int    des_ip;             //0: invalid
    unsigned char   net_mask2;          //0: invalid 24/32 mode
    unsigned short  des_port_start;     //0: invalid
    unsigned short  des_port_end;       //0: invalid
    unsigned char   protocol;           /*0: invalid,1:TCP,2:UDP,3:BOTH TCP/UDP,4:ICMP */
    struct _ip_filter_param_list *next;
}ip_filter_param_list;

void ip_filter_set_rules_ex(ip_filter_param_list* list);
void ip_filter_mode_set_ex(int mode);
int ip_filter_ip_layer_icmp(struct pbuf *p);

#ifdef __cplusplus
}
#endif

#endif /* __IP_FILTER_H__ */

