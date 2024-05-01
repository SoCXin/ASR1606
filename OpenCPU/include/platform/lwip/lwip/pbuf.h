/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef __LWIP_PBUF_H__
#define __LWIP_PBUF_H__

#include "opt.h"
#include "err.h"
#include "mem.h"
#include "sys_arch.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Currently, the pbuf_custom code is only needed for one specific configuration
 * of IP_FRAG */
//#define LWIP_SUPPORT_CUSTOM_PBUF (IP_FRAG && !IP_FRAG_USES_STATIC_BUF && !LWIP_NETIF_TX_SINGLE_PBUF)
#define LWIP_SUPPORT_CUSTOM_PBUF 1

/* @todo: We need a mechanism to prevent wasting memory in every pbuf
   (TCP vs. UDP, IPv4 vs. IPv6: UDP/IPv4 packets may waste up to 28 bytes) */

#define PBUF_TRANSPORT_HLEN 20
#if LWIP_IPV6
#define PBUF_IP_HLEN        40
#define PBUF_IP6_HLEN       40
#define PBUF_IP4_HLEN       20
#else
#define PBUF_IP_HLEN        20
#endif

typedef enum {
  PBUF_TRANSPORT = 0,
  PBUF_IP,
  PBUF_LINK,
  PBUF_RAW,

  /*extend setting for asr*/
  PBUF_RNDIS,
  PBUF_ECM,
  PBUF_WIFI,
  PBUF_MBIM,
  PBUF_PPP,
  PBUF_EXTEND,
  PBUF_PMSG
} pbuf_layer;

typedef enum {
  PBUF_RAM, /* pbuf data is stored in RAM */
  PBUF_ROM, /* pbuf data is stored in ROM */
  PBUF_REF, /* pbuf comes from the pbuf pool */
  PBUF_POOL, /* pbuf payload refers to RAM */
  PBUF_MEM  /* pbuf data is stored in LTE MEM */
} pbuf_type;

typedef struct lte_tx_hdr_tag
{
    u16_t   length;
    u16_t   dataOffset;
    u16_t   totalLength;
    u8_t   *npduData;
}lte_tx_hdr;

typedef struct lte_tx_list_tag
{
    u8_t                     epsBearerId;
    lte_tx_hdr               npdu;
    struct lte_tx_list_tag  *next;
}lte_tx_list;

#ifndef PBUF_PMSG_STRUCT_DEF
#define PBUF_PMSG_STRUCT_DEF
typedef struct _pmsg pmsg;
#define PMSG_SIZE   (sizeof(pmsg))

/** Function prototype for functions passed to tcpip_callback() */
typedef void* (*alloc_fn)(size_t size);
typedef void (*free_fn)(pmsg* msg);
struct _pmsg{
    free_fn free;                               /* Free callback function.  */
    void*   phdr;                               /* Buf header.              */
    UINT16  plen;                               /* Buf total length.        */
    UINT16  rnum;                               /* Buf ref num.             */
    UINT16  type;                               /* recorder memory type. */
    UINT16  layer;                              /* recorder memory layer. */
#if 0
    int     pmsgpos;                            /* recorder operator posion for debug */
    UINT32  pmsgtick;                           /* recorder operator ticks when happened */
#endif
}__attribute__((packed));

typedef struct _lte_pkt_ lte_pkt;
struct _lte_pkt_
{
    u8_t*       buf;
    u32_t       len;
    lte_pkt*    next;
    u32_t       o_cid;
    u8_t        areaid;
    u8_t        cid;
    u8_t        reserverd0;
    u8_t        reserverd1;
    u32_t       serviceid;
}__attribute__((packed));

typedef struct _lte_pkt_embms_ lte_pkt_embms;
struct _lte_pkt_embms_
{
    u8_t*       buf;
    u32_t       len;
    lte_pkt_embms*    next;
    u32_t       o_cid;
    u8_t        areaid;
    u8_t        cid;
    u8_t        reserverd0;
    u8_t        reserverd1;
    u32_t       serviceid;
}__attribute__((packed));
#endif

/** indicates this packet's data should be immediately passed to the application */
#define PBUF_FLAG_PUSH      0x01U
/** indicates this is a custom pbuf: pbuf_free and pbuf_header handle such a
    a pbuf differently */
#define PBUF_FLAG_IS_CUSTOM 0x02U
/** indicates this pbuf is UDP multicast to be looped back */
#define PBUF_FLAG_MCASTLOOP 0x04U
/** indicates this pbuf was received as link-level broadcast */
#define PBUF_FLAG_LLBCAST   0x08U
/** indicates this pbuf was received as link-level multicast */
#define PBUF_FLAG_LLMCAST   0x10U
/** indicates this pbuf includes a TCP FIN flag */
#define PBUF_FLAG_TCP_FIN   0x20U

#define PBUF_FLAG_STATIC    0x40U
#define PBUF_FLAG_ACK       0x80U
#define PBUF_FLAG_FLOW      0x0100U
#define PBUF_FLAG_LT1       0x0200U
#define PBUF_FLAG_LT2       0x0400U
#define PBUF_FLAG_LTEMEM    0x0800U
#define PBUF_FLAG_FREE      0x1000U
#define PBUF_FLAG_URG       0x2000U
#define PBUF_FLAG_SYN       0x4000U
#define PBUF_FLAG_IPSEC     0x10000U
#define PBUF_FLAG_IPSEC_TUN 0x20000U

/** indicates this pbuf if for pptp or gre */
#define PBUF_FLAG_PPTP_GRE      0x100000U
/** indicates this pbuf nat done already */
#define PBUF_FLAG_NAT_DONE      0x200000U
/** indicates this pbuf if assembled from self appication */
#define PBUF_FLAG_APP_DATA      0x400000U
/** indicates this pbuf if do tcp dns transfor */
#define PBUF_FLAG_DNS_TRANSFER  0x800000U
/** indicates this pbuf if do tcp dns transfor */
#define PBUF_FLAG_BRIDGE        0x1000000U
/** indicates this pbuf if ipv4 packet */
#define PBUF_FLAG_IP4_PACKET    0x2000000U
/** indicates this pbuf if ipv6 packet */
#define PBUF_FLAG_IP6_PACKET    0x4000000U

struct pbuf {
  /** next pbuf in singly linked pbuf chain */
  struct pbuf *next;

  /** pointer to the actual data in the buffer */
  void *payload;

  /**
   * total length of this buffer and all next buffers in chain
   * belonging to the same packet.
   *
   * For non-queue packet chains this is the invariant:
   * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
   */
  u16_t tot_len;

  /** length of this buffer */
  u16_t len;

  /** pointer to the mac header in the buffer */
  void *pkt_mac;

  /** pointer to pmsg */
  pmsg *pkt_msg;

  /** pbuf_type as u8_t instead of enum to save space */
  u16_t type;

  /**
   * the reference count always equals the number of pointers
   * that refer to this pbuf. This can be pointers from an application,
   * the stack itself, or pbuf->next pointers from a chain.
   */
  u8_t ref;

  /** For imcoming packets, this contains the input netif's netif->num*/
  u8_t ifnum;

  /** misc flags */
  u32_t flags;

  /** extend data */
  u32_t flowid;
  s32_t sockid;
  u32_t crtick;
};

#if LWIP_SUPPORT_CUSTOM_PBUF
/** Prototype for a function to free a custom pbuf */
typedef void (*pbuf_free_custom_fn)(struct pbuf *p);

/** A custom pbuf: like a pbuf, but following a function pointer to free it. */
struct pbuf_custom {
  /** The actual pbuf */
  struct pbuf pbuf;
  /** This function is called when pbuf_free deallocates this pbuf(_custom) */
  pbuf_free_custom_fn custom_free_function;
};
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */

#if LWIP_TCP && TCP_QUEUE_OOSEQ
/** Define this to 0 to prevent freeing ooseq pbufs when the PBUF_POOL is empty */
#ifndef PBUF_POOL_FREE_OOSEQ
#define PBUF_POOL_FREE_OOSEQ 1
#endif /* PBUF_POOL_FREE_OOSEQ */
#if NO_SYS && PBUF_POOL_FREE_OOSEQ
extern volatile u8_t pbuf_free_ooseq_pending;
void pbuf_free_ooseq();
/** When not using sys_check_timeouts(), call PBUF_CHECK_FREE_OOSEQ()
    at regular intervals from main level to check if ooseq pbufs need to be
    freed! */
#define PBUF_CHECK_FREE_OOSEQ() do { if(pbuf_free_ooseq_pending) { \
  /* pbuf_alloc() reported PBUF_POOL to be empty -> try to free some \
     ooseq queued pbufs now */ \
  pbuf_free_ooseq(); }}while(0)
#endif /* NO_SYS && PBUF_POOL_FREE_OOSEQ*/
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ */

/* Initializes the pbuf module. This call is empty for now, but may not be in future. */
#define pbuf_init()
#define SIZEOF_STRUCT_PBUF        			LWIP_MEM_ALIGN_SIZE(sizeof(struct pbuf))
#define pbuf_get_crtick_diff(pbuf)          (U32_DIFF(OSAGetTicks(), (pbuf)->crtick))
#define pbuf_inherit_flags(pd, ps, flag)   ((pd)->flags |= ((ps)->flags & (flag)))
#define pbuf_get_ifnum(pbuf)                ((pbuf)->ifnum)
#define pbuf_set_ifnum(pbuf, val)           ((pbuf)->ifnum = (val))
#define pbuf_get_sockid(pbuf)                ((pbuf)->sockid)
#define pbuf_set_sockid(pbuf, val)           ((pbuf)->sockid = (val))
#define pbuf_set_flowid(pbuf, val)           ((pbuf)->flowid = (val))

/* macro function define here */
#define pbuf_is_static(pbuf)               (((pbuf)->flags & PBUF_FLAG_STATIC) ? 1 : 0)
#define pbuf_is_lte_part1(pbuf)            (((pbuf)->flags & PBUF_FLAG_LT1) ? 1 : 0)
#define pbuf_is_lte_part2(pbuf)            (((pbuf)->flags & PBUF_FLAG_LT2) ? 1 : 0)
#define pbuf_is_LTE_MEM(pbuf)              (((pbuf)->flags & PBUF_FLAG_LTEMEM) ? 1 : 0)
#define pbuf_clear_LTE_MEM(pbuf)           ((pbuf)->flags &= ~PBUF_FLAG_LTEMEM)

#define pbuf_is_for_app(pbuf)              (((pbuf)->flags & PBUF_FLAG_APP_DATA) ? 1 : 0)
#define pbuf_set_for_app(pbuf)             ((pbuf)->flags |= PBUF_FLAG_APP_DATA)

#define pbuf_is_for_bridge(pbuf)           (((pbuf)->flags & PBUF_FLAG_BRIDGE) ? 1 : 0)
#define pbuf_set_for_bridge(pbuf)          ((pbuf)->flags |= PBUF_FLAG_BRIDGE)

#define pbuf_is_pptp_gre(pbuf)             (((pbuf)->flags & PBUF_FLAG_PPTP_GRE) ? 1 : 0)
#define pbuf_set_pptp_gre(pbuf)            ((pbuf)->flags |= PBUF_FLAG_PPTP_GRE)

#define pbuf_is_for_ipsec(pbuf)            (((pbuf)->flags & PBUF_FLAG_IPSEC) ? 1 : 0)
#define pbuf_set_for_ipsec(pbuf)           ((pbuf)->flags |= PBUF_FLAG_IPSEC)
#define pbuf_clear_ipsec(pbuf)             ((pbuf)->flags &= ~PBUF_FLAG_IPSEC)

#define pbuf_is_for_ipsectun(pbuf)            (((pbuf)->flags & PBUF_FLAG_IPSEC_TUN) ? 1 : 0)
#define pbuf_set_for_ipsectun(pbuf)           ((pbuf)->flags |= PBUF_FLAG_IPSEC_TUN)
#define pbuf_clear_ipsectun(pbuf)             ((pbuf)->flags &= ~PBUF_FLAG_IPSEC_TUN)

#define pbuf_is_tcp_push(pbuf)             (((pbuf)->flags & PBUF_FLAG_PUSH) ? 1 : 0)
#define pbuf_set_tcp_push(pbuf)            ((pbuf)->flags |= PBUF_FLAG_PUSH)

#define pbuf_is_tcp_syn(pbuf)              (((pbuf)->flags & PBUF_FLAG_SYN) ? 1 : 0)
#define pbuf_set_tcp_syn(pbuf)             ((pbuf)->flags |= PBUF_FLAG_SYN)

#define pbuf_is_tcp_urg(pbuf)              (((pbuf)->flags & PBUF_FLAG_URG) ? 1 : 0)
#define pbuf_set_tcp_urg(pbuf)             ((pbuf)->flags |= PBUF_FLAG_URG)

#define pbuf_is_ack_flag(pbuf)             (((pbuf)->flags & PBUF_FLAG_ACK) ? 1 : 0)
#define pbuf_set_ack_flag(pbuf)            ((pbuf)->flags |= PBUF_FLAG_ACK)

#define pbuf_is_flow_flag(pbuf)            (((pbuf)->flags & PBUF_FLAG_FLOW) ? 1 : 0)
#define pbuf_set_flow_flag(pbuf)           ((pbuf)->flags |= PBUF_FLAG_FLOW)

#define pbuf_is_nat_done(pbuf)             (((pbuf)->flags & PBUF_FLAG_NAT_DONE) ? 1 : 0)
#define pbuf_set_nat_done(pbuf)            ((pbuf)->flags |= PBUF_FLAG_NAT_DONE)

#define pbuf_is_dns_transfer(pbuf)         (((pbuf)->flags & PBUF_FLAG_DNS_TRANSFER) ? 1 : 0)
#define pbuf_set_dns_transfer(pbuf)        ((pbuf)->flags |= PBUF_FLAG_DNS_TRANSFER)

#define pbuf_is_ip4_packet(pbuf)         (((pbuf)->flags & PBUF_FLAG_IP4_PACKET) ? 1 : 0)
#define pbuf_set_ip4_packet(pbuf)        ((pbuf)->flags |= PBUF_FLAG_IP4_PACKET)

#define pbuf_is_ip6_packet(pbuf)         (((pbuf)->flags & PBUF_FLAG_IP6_PACKET) ? 1 : 0)
#define pbuf_set_ip6_packet(pbuf)        ((pbuf)->flags |= PBUF_FLAG_IP6_PACKET)

#if 0
#if MEMP_OVERFLOW_CHECK
struct pbuf *
pbuf_alloc_fn(
	pbuf_layer layer,
	u16_t length,
	pbuf_type type,
	const char* file,
	const int line);
#define pbuf_alloc(layer, length, type) \
		pbuf_alloc_fn((layer), (length), (type), __FUNCTION__, __LINE__)
#else
struct pbuf *pbuf_alloc(pbuf_layer l, u16_t length, pbuf_type type);
#endif
#endif

struct pbuf *
pbuf_alloc_fn( pbuf_layer layer, u16_t length, pbuf_type type, const char* func_name, const int line);
#define pbuf_alloc(layer, length, type)     pbuf_alloc_fn((layer), (length), (type), __FUNCTION__, __LINE__)

#if 0
u8_t pbuf_free(struct pbuf *p);
#endif

u8_t
pbuf_free_fn(struct pbuf *p, const char* func_name, const int line);
#define pbuf_free(p)                        pbuf_free_fn((p), __FUNCTION__, __LINE__)


#if LWIP_SUPPORT_CUSTOM_PBUF
struct pbuf *pbuf_alloced_custom(pbuf_layer l, u16_t length, pbuf_type type,
                                 struct pbuf_custom *p, void *payload_mem,
                                 u16_t payload_mem_len);
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */
void pbuf_realloc(struct pbuf *p, u16_t size);
u8_t pbuf_remove_header(struct pbuf *p, size_t header_size_decrement);
u8_t pbuf_add_header(struct pbuf *p, size_t header_size_increment);
u8_t pbuf_add_header_force(struct pbuf *p, size_t header_size_increment);
u8_t pbuf_header(struct pbuf *p, s16_t header_size);
u8_t pbuf_header_force(struct pbuf *p, s16_t header_size_increment);
void pbuf_ref(struct pbuf *p);
u8_t pbuf_clen(struct pbuf *p);
void pbuf_cat(struct pbuf *head, struct pbuf *tail);
void pbuf_chain(struct pbuf *head, struct pbuf *tail);
struct pbuf *pbuf_dechain(struct pbuf *p);
err_t pbuf_copy(struct pbuf *p_to, struct pbuf *p_from);
u16_t pbuf_copy_partial(struct pbuf *p, void *dataptr, u16_t len, u16_t offset);
err_t pbuf_take(struct pbuf *buf, const void *dataptr, u16_t len);
struct pbuf *pbuf_coalesce(struct pbuf *p, pbuf_layer layer);
#if LWIP_CHECKSUM_ON_COPY
err_t pbuf_fill_chksum(struct pbuf *p, u16_t start_offset, const void *dataptr,
                       u16_t len, u16_t *chksum);
#endif /* LWIP_CHECKSUM_ON_COPY */
#if LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
void pbuf_split_64k(struct pbuf *p, struct pbuf **rest);
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

u8_t pbuf_get_at(struct pbuf* p, u16_t offset);
u8_t* pbuf_get_ptr(struct pbuf* p, u16_t offset);
u16_t pbuf_memcmp(struct pbuf* p, u16_t offset, const void* s2, u16_t n);
u16_t pbuf_memfind(struct pbuf* p, const void* mem, u16_t mem_len, u16_t start_offset);
u16_t pbuf_strstr(struct pbuf* p, const char* substr);
u32_t pbuf_gen_flowid(struct pbuf *p, void *phdr, int uldl);

/*func: pmsg_free_fun*/
/*free func match malloc, alloc, MALLOC*/
static inline void pmsg_free_fun(pmsg *msg)
{
    if (!msg) {return;}
    free(msg->phdr);
#if 0
    LWIP_DIAG(MEM_DEBUG, lwip_pbuf_363, "lwipmem: %s, msg=%lx,->phdr=%lx,->plen=%d,tick=%lu",
        					                    __FUNCTION__, msg, msg->phdr, msg->plen, OSAGetTicks());
#endif
}

/*func: pmsg_mem_free*/
/*free func match mem_malloc*/
static inline void pmsg_mem_free(pmsg *msg)
{
    if (!msg) {return;}
    mem_free(msg->phdr);
#if 0
    LWIP_DIAG(MEM_DEBUG, lwip_pbuf_365, "lwipmem: %s, msg=%lx,->phdr=%lx,->plen=%d,tick=%lu",
        					                    __FUNCTION__, msg, msg->phdr, msg->plen, OSAGetTicks());
#endif
}

static inline void pmsg_mem_free_64A(pmsg *msg)
{
    if (!msg) {return;}
    mem_free_64A(msg->phdr);
#if 0
    LWIP_DIAG(MEM_DEBUG, lwip_pbuf_368, "lwipmem: %s, msg=%lx,->phdr=%lx,->plen=%d,tick=%lu",
                                                __FUNCTION__, msg, msg->phdr, msg->plen, OSAGetTicks());
#endif
}


/*func: pmsg_pmsg_set*/
/*key parameter is msg, fn, phdr*/
/*some time, len and type no care, can set 0*/
static inline void pmsg_pmsg_set(pmsg *msg, free_fn fn, void *phdr, int plen, int type)
{
    if(msg) {
        msg->free = fn;
        msg->phdr = phdr;
        msg->plen = plen;
        msg->type = type;
    }
}

static inline void pmsg_pmsg_copy(pmsg *msg_dest, pmsg* msg_src)
{
    if(msg_dest && msg_src) {
        msg_dest->free = msg_src->free;
        msg_dest->phdr = msg_src->phdr;
        msg_dest->plen = msg_src->plen;
        msg_dest->type = msg_src->type;
    }
}

static inline void pmsg_pbuf_set_mem(struct pbuf* p, void* pkt_hdr, void*pkt_mac, pmsg* pkt_msg)
{
    if(p->type == PBUF_MEM) {
        p->payload = pkt_hdr; // ip hdr or mac hdr
        p->pkt_mac = pkt_mac; // mac hdr
        p->pkt_msg = pkt_msg; // msg ptr

#if 0
        /*special deal for lte mem*/
        if(pkt_msg && pkt_msg->type == PBUF_MEM) {
            p->flags |= PBUF_FLAG_LTEMEM;
        }
#endif
    }
}

static inline void pmsg_pbuf_free_mem(struct pbuf* p)
{
    if(p->type == PBUF_MEM
        && p->pkt_msg
        && p->pkt_msg->free) {
        p->pkt_msg->free(p->pkt_msg);
        p->pkt_msg = NULL;
    }
}

void* lwip_in_buf_alloc(size_t size, pbuf_type type, pbuf_layer layer, alloc_fn alloc_in, free_fn free_in);
void lwip_in_buf_free(void* buf, pbuf_layer layer);
pmsg* lwip_in_buf_pmsg(void* buf, pbuf_layer layer);
void* lwip_out_buf_alloc(size_t size, pbuf_type type, pbuf_layer layer, alloc_fn alloc_in, free_fn free_in);
void lwip_out_buf_free(void* buf, pbuf_layer layer);
pmsg* lwip_out_buf_pmsg(void* buf, pbuf_layer layer);
u8_t* lwip_wifi_in_buf_alloc(size_t size);
u8_t* lwip_wifi_in_buf_pmsg(u8_t* buf);

struct pbuf * lte_pbuf_reshape(struct pbuf * p, int copy_flag, int p_free_flag);
struct pbuf * lte_pbuf_reshape_cpy(struct pbuf * p);

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_PBUF_H__ */
