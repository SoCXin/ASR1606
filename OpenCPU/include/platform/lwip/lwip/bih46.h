#ifndef __LWIP_BIH46_H__
#define __LWIP_BIH46_H__

#if LWIP_BIH_46

#define BIH_IP4_AA           172
#define BIH_IP4_BB           21
#define BIH_IP4_C1           112
#define BIH_IP4_C2           254
#define BIH_IP4_D1           1
#define BIH_IP4_D2           254

#define BIH_IPV6_ADDR2      0x32571235

#define IS_BIH_IP4_ADDR(addr) (((addr) & 0xFFFF) == (BIH_IP4_AA | BIH_IP4_BB << 8))
#define IS_BIH_IP6_ADDR(addr2, addr3) (((addr2) == BIH_IPV6_ADDR2) && \
                                        (((addr3) & 0xFFFF) == 0xA8C0))

//u8_t bih_get_enflg();
u16_t bih_get_query_type(u8_t *buf, int len);
void bih_set_query_type(u8_t *buf, int len, u16_t type);
int bih_dns_ip6_to_ip4(u8_t *buf, int *len);
int bih_ip4_to_ip6(struct pbuf *src, struct pbuf **dst, ip6_addr_t **ip6_out);
int bih_ip6_to_ip4(struct pbuf *src, struct pbuf **dst, ip_addr_t *ip4_out);

extern u8_t g_bih_en;

static inline u8_t bih_get_enflg(void)
{
    return g_bih_en;
}

static inline void bih_set_enflg(u8_t flg)
{
    g_bih_en = flg;
}

#endif

#endif /* __LWIP_BIH46_H__ */
