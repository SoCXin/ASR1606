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
#ifndef __LWIP_API_H__
#define __LWIP_API_H__

#include "opt.h"

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */

#include <stddef.h> /* for size_t */
#include <limits.h>

#include "netbuf.h"
#include "sys.h"
#include "ip_addr.h"
#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SSIZE_MAX       INT_MAX
#define LWIP_UINT32_MAX 0xffffffff

/* Throughout this file, IP addresses and port numbers are expected to be in
 * the same byte order as in the corresponding pcb.
 */

/* Flags for netconn_write (u8_t) */
#define NETCONN_NOFLAG      0x00
#define NETCONN_NOCOPY      0x00 /* Only for source code compatibility */
#define NETCONN_COPY        0x01
#define NETCONN_MORE        0x02
#define NETCONN_DONTBLOCK   0x04
#define NETCONN_NOAUTORCVD  0x08 /* prevent netconn_recv_data_tcp() from updating the tcp window - must be done manually via netconn_tcp_recvd() */
#define NETCONN_NOFIN       0x10 /* upper layer already received data, leave FIN in queue until called again */

/* Flags for struct netconn.flags (u8_t) */
/** TCP: when data passed to netconn_write doesn't fit into the send buffer,
    this temporarily stores whether to wake up the original application task
    if data couldn't be sent in the first try. */
#define NETCONN_FLAG_WRITE_DELAYED            0x01
/** Should this netconn avoid blocking? */
#define NETCONN_FLAG_NON_BLOCKING             0x02
/** Was the last connect action a non-blocking one? */
#define NETCONN_FLAG_IN_NONBLOCKING_CONNECT   0x04
/** If this is set, a TCP netconn must call netconn_recved() to update
    the TCP receive window (done automatically if not set). */
#define NETCONN_FLAG_NO_AUTO_RECVED           0x08
/** If a nonblocking write has been rejected before, poll_tcp needs to
    check if the netconn is writable again */
#define NETCONN_FLAG_CHECK_WRITESPACE         0x10
#if LWIP_IPV6
/** If this flag is set then only IPv6 communication is allowed on the
    netconn. As per RFC#3493 this features defaults to OFF allowing
    dual-stack usage by default. */
#define NETCONN_FLAG_IPV6_V6ONLY              0x20
#endif /* LWIP_IPV6 */
/** If this is set, a TCP netconn have OOB data inline
    only one byte now. */
#define NETCONN_FLAG_OOBINLINE                0x40

/* Helpers to process several netconn_types by the same code */
#define NETCONNTYPE_GROUP(t)         ((t)&0xF0)
#define NETCONNTYPE_DATAGRAM(t)      ((t)&0xE0)
#if LWIP_IPV6
#define NETCONN_TYPE_IPV6            0x08
#define NETCONNTYPE_ISIPV6(t)        ((t)&0x08)
#define NETCONNTYPE_ISUDPLITE(t)     (((t)&0xF7) == NETCONN_UDPLITE)
#define NETCONNTYPE_ISUDPNOCHKSUM(t) (((t)&0xF7) == NETCONN_UDPNOCHKSUM)
#else /* LWIP_IPV6 */
#define NETCONNTYPE_ISUDPLITE(t)     ((t) == NETCONN_UDPLITE)
#define NETCONNTYPE_ISUDPNOCHKSUM(t) ((t) == NETCONN_UDPNOCHKSUM)
#endif /* LWIP_IPV6 */

/** Protocol family and type of the netconn */
enum netconn_type {
  NETCONN_INVALID     = 0,
  /* NETCONN_TCP Group */
  NETCONN_TCP         = 0x10,
#if LWIP_IPV6
  NETCONN_TCP_IPV6    = NETCONN_TCP | NETCONN_TYPE_IPV6 /* 0x18 */,
#endif /* LWIP_IPV6 */
  /* NETCONN_UDP Group */
  NETCONN_UDP         = 0x20,
  NETCONN_UDPLITE     = 0x21,
  NETCONN_UDPNOCHKSUM = 0x22,
#if LWIP_IPV6
  NETCONN_UDP_IPV6         = NETCONN_UDP | NETCONN_TYPE_IPV6 /* 0x28 */,
  NETCONN_UDPLITE_IPV6     = NETCONN_UDPLITE | NETCONN_TYPE_IPV6 /* 0x29 */,
  NETCONN_UDPNOCHKSUM_IPV6 = NETCONN_UDPNOCHKSUM | NETCONN_TYPE_IPV6 /* 0x2a */,
#endif /* LWIP_IPV6 */
  /* NETCONN_RAW Group */
  NETCONN_RAW         = 0x40
#if LWIP_IPV6
  ,
  NETCONN_RAW_IPV6    = NETCONN_RAW | NETCONN_TYPE_IPV6 /* 0x48 */
#endif /* LWIP_IPV6 */
};

/** Current state of the netconn. Non-TCP netconns are always
 * in state NETCONN_NONE! */
enum netconn_state {
  NETCONN_NONE,
  NETCONN_WRITE,
  NETCONN_LISTEN,
  NETCONN_CONNECT,
  NETCONN_CLOSE
};

/** Use to inform the callback function about changes */
enum netconn_evt {
  NETCONN_EVT_RCVPLUS,
  NETCONN_EVT_RCVMINUS,
  NETCONN_EVT_SENDPLUS,
  NETCONN_EVT_SENDMINUS,
  NETCONN_EVT_CONNECTED,
  NETCONN_EVT_ACCEPTPLUS,
  NETCONN_EVT_ERROR_CLSD,
  NETCONN_EVT_ERROR_RST,
  NETCONN_EVT_ERROR_ABRT,
  NETCONN_EVT_CLOSE_WAIT,
  NETCONN_EVT_SENDACKED,
  NETCONN_EVT_CLOSE_NORMAL,
  NETCONN_EVT_ERROR
};

#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
/** Used for netconn_join_leave_group() */
enum netconn_igmp {
  NETCONN_JOIN,
  NETCONN_LEAVE
};
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */

/* forward-declare some structs to avoid to include their headers */
struct ip_pcb;
struct tcp_pcb;
struct udp_pcb;
struct raw_pcb;
struct netconn;
struct api_msg_msg;

/** A callback prototype to inform about events for a netconn */
typedef void (* netconn_callback)(struct netconn *, enum netconn_evt, u16_t len);

/** A netconn descriptor */
struct netconn {
  /** type of the netconn (TCP, UDP or RAW) */
  enum netconn_type type;
  /** current state of the netconn */
  enum netconn_state state;
  /** the lwIP internal protocol control block */
  union {
    struct ip_pcb  *ip;
    struct tcp_pcb *tcp;
    struct udp_pcb *udp;
    struct raw_pcb *raw;
  } pcb;
  /** the lwIP internal protocol control block, only for tcp refer */
  union {
    struct ip_pcb  *ip;
    struct tcp_pcb *tcp;
  } pcb_ref;
  /** the last error this netconn had */
  err_t last_err;

  /** mbox where received packets are stored until they are fetched
      by the netconn application thread (can grow quite big) */
  sys_mbox_t recvmbox;
#if LWIP_TCP
  /** mbox where new connections are stored until processed
      by the application thread */
  sys_mbox_t acceptmbox;
#endif /* LWIP_TCP */
  /** only used for socket layer */
#if LWIP_SOCKET
  int socket;
#endif /* LWIP_SOCKET */
#if LWIP_SO_SNDTIMEO
  /** timeout to wait for sending data (which means enqueueing data for sending
      in internal buffers) */
  s32_t send_timeout;
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVTIMEO
  /** timeout to wait for new data to be received
      (or connections to arrive for listening netconns) */
  int recv_timeout;
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVBUF
  /** maximum amount of bytes queued in recvmbox
      not used for TCP: adjust TCP_WND instead! */
  int recv_bufsize;
  /** number of bytes currently in recvmbox to be received,
      tested against recv_bufsize to limit bytes on recvmbox
      for UDP and RAW, used for FIONREAD */
  int recv_avail;
#endif /* LWIP_SO_RCVBUF */
#if LWIP_SO_LINGER
     /** values <0 mean linger is disabled, values > 0 are seconds to linger */
  s16_t linger;
#endif /* LWIP_SO_LINGER */
  /** flags holding more netconn-internal state, see NETCONN_FLAG_* defines */
  u8_t flags;
  /** oob_data restore, if flag of bit NETCONN_FLAG_OOBINLINE define */
  u8_t oob_data;
#if LWIP_TCP
  /** TCP: when data passed to netconn_write doesn't fit into the send buffer,
      this temporarily stores the message.
      Also used during connect and close. */
  struct api_msg_msg *current_msg;
#endif /* LWIP_TCP */
  /** A callback function that is informed about events for this netconn */
  netconn_callback callback;

#if LWIP_MALLOC_NETCONN_STATIC
  /** flag, mark this structure been using ,will using static pool malloc*/
  int using_flag;
#endif

#if LWIP_NETCONN_DEBUG_OP_FLAG
  /** add debug recorder for netconn operation */
  int debug_op;
  u32_t debug_line;
  u32_t debug_tick;
#endif

#if LWIP_NETCONN_EVENT_OP_FLAG
  int event_op;
  u32_t event_line;
  u32_t event_tick;
#endif
};

/** This vector type is passed to @ref netconn_write_vectors_partly to send
 * multiple buffers at once.
 * ATTENTION: This type has to directly map struct iovec since one is casted
 *            into the other!
 */
struct netvector {
  /** pointer to the application buffer that contains the data to send */
  const void *ptr;
  /** size of the application data to send */
  size_t len;
};

/*redmine#33649, xujingquan, 20210519, begin*/
struct api_event_msg {
  struct netconn * conn;
  struct netbuf * nbuf;
  int event;
  int size;
};
/*redmine#33649, xujingquan, 20210519, end*/

#if LWIP_NETCONN_EVENT_OP_FLAG
/** Register an Network connection event */
#define API_EVENT(c,e,l,cat3) if (c->callback) {         \
                          LWIP_DIAG(API_MSG_DEBUG, cat3, "LWIP_API_EVENT, %s-%d, conn=%lx, evt=%d, len=%u", __FUNCTION__, __LINE__, c, e, l); \
                            c->event_op = e; \
                            c->event_line = __LINE__; \
                            c->event_tick = (OSAGetTicks()); \
                            (*c->callback)(c, e, l); \
                         }

/** Register an Network connection event, with pbuf debug info */
#define API_EVENT_2(c,e,l,buf,pbuf,cat3) if (c->callback) {         \
                          LWIP_DIAG(API_MSG_DEBUG, cat3, "LWIP_API_EVENT, %s-%d, conn=%lx, evt=%d, len=%u, buf=%lx, pbuf=%lx", __FUNCTION__, __LINE__, c, e, l, buf, pbuf); \
                            c->event_op = e; \
                            c->event_line = __LINE__; \
                            c->event_tick = (OSAGetTicks()); \
                            (*c->callback)(c, e, l); \
                         }
#else
/** Register an Network connection event */
#define API_EVENT(c,e,l,cat3) if (c->callback) {         \
                          LWIP_DIAG(API_MSG_DEBUG, cat3, "LWIP_API_EVENT, %s-%d, conn=%lx, evt=%d, len=%u", __FUNCTION__, __LINE__, c, e, l); \
                            (*c->callback)(c, e, l); \
                         }

/** Register an Network connection event, with pbuf debug info */
#define API_EVENT_2(c,e,l,buf,pbuf,cat3) if (c->callback) {         \
                          LWIP_DIAG(API_MSG_DEBUG, cat3, "LWIP_API_EVENT, %s-%d, conn=%lx, evt=%d, len=%u, buf=%lx, pbuf=%lx", __FUNCTION__, __LINE__, c, e, l, buf, pbuf); \
                            (*c->callback)(c, e, l); \
                         }
#endif

/** Set conn->last_err to err but don't overwrite fatal errors */
#define NETCONN_SET_SAFE_ERR(conn, err) do { \
  SYS_ARCH_DECL_PROTECT(lev); \
  SYS_ARCH_PROTECT(lev); \
  if (!ERR_IS_FATAL((conn)->last_err)) { \
    (conn)->last_err = err; \
  } \
  SYS_ARCH_UNPROTECT(lev); \
} while(0);

enum lwip_netconn_debug_op_e {
    LWIP_NETCONN_DEBUG_OP_BEGIN  = 0x0,

    LWIP_NETCONN_DEBUG_OP_TCPIP_POST,
    LWIP_NETCONN_DEBUG_OP_TCPIP_RSP,

    LWIP_NETCONN_DEBUG_OP_NEW_ENTER,
    LWIP_NETCONN_DEBUG_OP_NEW_FINISH,

    LWIP_NETCONN_DEBUG_OP_RECV_ENTER = 0x80,
    LWIP_NETCONN_DEBUG_OP_RECV_RETURN,
    LWIP_NETCONN_DEBUG_OP_RECV_CALL_REC,
    LWIP_NETCONN_DEBUG_OP_RECV_PEND_MBOX,
    LWIP_NETCONN_DEBUG_OP_RECV_FINISH,

    LWIP_NETCONN_DEBUG_OP_END
};

#if LWIP_NETCONN_DEBUG_OP_FLAG
#define LWIP_DEBUG_HOOK_REC_NETCONN_OP(conn, e)  do { \
  if (conn) { \
    conn->debug_op = (e); \
    conn->debug_line = __LINE__; \
    conn->debug_tick = (OSAGetTicks()); \
  } \
} while (0)
#else
#define LWIP_DEBUG_HOOK_REC_NETCONN_OP(conn, e)
#endif

/* Network connection functions: */
#define netconn_new(t)                  netconn_new_with_proto_and_callback(t, 0, NULL)
#define netconn_new_with_callback(t, c) netconn_new_with_proto_and_callback(t, 0, c)

#if LWIP_MALLOC_NETCONN_STATIC
void memp_free_netconn(struct netconn* conn);
struct netconn* memp_malloc_netconn(void);
#endif

struct netconn *netconn_new_with_proto_and_callback(enum netconn_type t, u8_t proto,
                                             netconn_callback callback);
err_t   netconn_delete(struct netconn *conn);
err_t   netconn_delete_tcp(struct netconn *conn);

/** Get the type of a netconn (as enum netconn_type). */
#define netconn_type(conn) (conn->type)

err_t   netconn_getaddr(struct netconn *conn, ip_addr_t *addr,
                        u16_t *port, u8_t local);
#define netconn_peer(c,i,p) netconn_getaddr(c,i,p,0)
#define netconn_addr(c,i,p) netconn_getaddr(c,i,p,1)

err_t   netconn_bind(struct netconn *conn, ip_addr_t *addr, u16_t port);
err_t   netconn_connect(struct netconn *conn, ip_addr_t *addr, u16_t port);
err_t   netconn_disconnect (struct netconn *conn);
err_t   netconn_listen_with_backlog(struct netconn *conn, u8_t backlog);
#define netconn_listen(conn) netconn_listen_with_backlog(conn, TCP_DEFAULT_LISTEN_BACKLOG)
err_t   netconn_accept(struct netconn *conn, struct netconn **new_conn);
err_t   netconn_recv(struct netconn *conn, struct netbuf **new_buf);
err_t   netconn_recv_tcp_pbuf(struct netconn *conn, struct pbuf **new_buf);
err_t   netconn_recv_udp_raw_netbuf(struct netconn *conn, struct netbuf **new_buf);
err_t   netconn_recv_udp_raw_netbuf_flags(struct netconn *conn, struct netbuf **new_buf, u8_t apiflags);
void    netconn_recved(struct netconn *conn, u32_t length);
err_t   netconn_sendto(struct netconn *conn, struct netbuf *buf, ip_addr_t *addr, u16_t port);
err_t   netconn_send(struct netconn *conn, struct netbuf *buf);
err_t   netconn_write_partly(struct netconn *conn, const void *dataptr, size_t size,
                             u8_t apiflags, size_t *bytes_written);
err_t   netconn_write_vectors_partly(struct netconn *conn, struct netvector *vectors, u16_t vectorcnt,
                             u8_t apiflags, size_t *bytes_written);
err_t   netconn_write(struct netconn *conn, const void *dataptr, size_t size, u8_t apiflags);
err_t   netconn_close(struct netconn *conn);
err_t   netconn_shutdown(struct netconn *conn, u8_t shut_rx, u8_t shut_tx);

#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
err_t   netconn_join_leave_group(struct netconn *conn, ip_addr_t *multiaddr,
                                 ip_addr_t *netif_addr, enum netconn_igmp join_or_leave);
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */
#if LWIP_DNS
err_t   netconn_gethostbyname(const char *name, ip_addr_t *addr);
err_t   netconn_gethostbyname_withmsg(const char *name, ip_addr_t *addr, void *callback_arg);
#endif /* LWIP_DNS */
#if LWIP_IPV6

#define netconn_bind_ip6(conn, ip6addr, port) (NETCONNTYPE_ISIPV6((conn)->type) ? \
        netconn_bind(conn, ip6_2_ip(ip6addr), port) : ERR_VAL)
#define netconn_connect_ip6(conn, ip6addr, port) (NETCONNTYPE_ISIPV6((conn)->type) ? \
        netconn_connect(conn, ip6_2_ip(ip6addr), port) : ERR_VAL)
#define netconn_sendto_ip6(conn, buf, ip6addr, port) (NETCONNTYPE_ISIPV6((conn)->type) ? \
        netconn_sendto(conn, buf, ip6_2_ip(ip6addr), port) : ERR_VAL)
#if LWIP_IPV6_MLD
#define netconn_join_leave_group_ip6(conn, multiaddr, srcaddr, join_or_leave) (NETCONNTYPE_ISIPV6((conn)->type) ? \
        netconn_join_leave_group(conn, ip6_2_ip(multiaddr), ip6_2_ip(srcaddr), join_or_leave) :\
        ERR_VAL)
#endif /* LWIP_IPV6_MLD*/
#endif /* LWIP_IPV6 */

#define netconn_err(conn)               ((conn)->last_err)
#define netconn_recv_bufsize(conn)      ((conn)->recv_bufsize)

/** Set the blocking status of netconn calls (@todo: write/send is missing) */
#define netconn_set_nonblocking(conn, val)  do { if(val) { \
  (conn)->flags |= NETCONN_FLAG_NON_BLOCKING; \
} else { \
  (conn)->flags &= ~ NETCONN_FLAG_NON_BLOCKING; }} while(0)
/** Get the blocking status of netconn calls (@todo: write/send is missing) */
#define netconn_is_nonblocking(conn)        (((conn)->flags & NETCONN_FLAG_NON_BLOCKING) != 0)

/** TCP: Set the no-auto-recved status of netconn calls (see NETCONN_FLAG_NO_AUTO_RECVED) */
#define netconn_set_noautorecved(conn, val)  do { if(val) { \
  (conn)->flags |= NETCONN_FLAG_NO_AUTO_RECVED; \
} else { \
  (conn)->flags &= ~ NETCONN_FLAG_NO_AUTO_RECVED; }} while(0)
/** TCP: Get the no-auto-recved status of netconn calls (see NETCONN_FLAG_NO_AUTO_RECVED) */
#define netconn_get_noautorecved(conn)        (((conn)->flags & NETCONN_FLAG_NO_AUTO_RECVED) != 0)

#if LWIP_SO_SNDTIMEO
/** Set the send timeout in milliseconds */
#define netconn_set_sendtimeout(conn, timeout)      ((conn)->send_timeout = (timeout))
/** Get the send timeout in milliseconds */
#define netconn_get_sendtimeout(conn)               ((conn)->send_timeout)
#endif /* LWIP_SO_SNDTIMEO */
#if LWIP_SO_RCVTIMEO
/** Set the receive timeout in milliseconds */
#define netconn_set_recvtimeout(conn, timeout)      ((conn)->recv_timeout = (timeout))
/** Get the receive timeout in milliseconds */
#define netconn_get_recvtimeout(conn)               ((conn)->recv_timeout)
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVBUF
/** Set the receive buffer in bytes */
#define netconn_set_recvbufsize(conn, recvbufsize)  ((conn)->recv_bufsize = (recvbufsize))
/** Get the receive buffer in bytes */
#define netconn_get_recvbufsize(conn)               ((conn)->recv_bufsize)
#endif /* LWIP_SO_RCVBUF*/

#ifdef __cplusplus
}
#endif

#endif /* LWIP_NETCONN */

#endif /* __LWIP_API_H__ */
