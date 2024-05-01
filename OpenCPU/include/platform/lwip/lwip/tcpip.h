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
#ifndef __LWIP_TCPIP_H__
#define __LWIP_TCPIP_H__

#include "opt.h"

#if !NO_SYS /* don't build if not configured for use in lwipopts.h */

#include "api_msg.h"
#include "netifapi.h"
#include "pbuf.h"
#include "api.h"
#include "sys.h"
#include "timers.h"
#include "netif.h"
#include "tcp.h"
#include "udp.h"

#ifdef __cplusplus
extern "C" {
#endif

#if LWIP_MPU_COMPATIBLE
/*@TBD*/
#else /* LWIP_MPU_COMPATIBLE */
#define API_VAR_REF(name)               name
#define API_VAR_DECLARE(type, name)     type name
#define API_VAR_ALLOC_EXT(type, pool, name, errorblock)
#define API_VAR_ALLOC(type, pool, name, errorval)
#define API_VAR_ALLOC_POOL(type, pool, name, errorval)
#define API_VAR_FREE(pool, name)
#define API_VAR_FREE_POOL(pool, name)
#define API_EXPR_REF(expr)              expr
#define API_EXPR_REF_SEM(expr)          API_EXPR_REF(expr)
#define API_EXPR_DEREF(expr)            (*(expr))
#define API_MSG_M_DEF(m)                *m
#define API_MSG_M_DEF_C(t, m)           const t * m
#endif /* LWIP_MPU_COMPATIBLE */


/** Define this to something that triggers a watchdog. This is called from
 * tcpip_thread after processing a message. */
#ifndef LWIP_TCPIP_THREAD_ALIVE
#define LWIP_TCPIP_THREAD_ALIVE()
#endif

#define SYS_MQ_USED 0

#if LWIP_TCPIP_CORE_LOCKING
/** The global semaphore to lock the stack. */
extern sys_mutex_t lock_tcpip_core;
#define LOCK_TCPIP_CORE()     sys_mutex_lock(&lock_tcpip_core)
#define UNLOCK_TCPIP_CORE()   sys_mutex_unlock(&lock_tcpip_core)
#ifdef LWIP_DEBUG
#define TCIP_APIMSG_SET_ERR(m, e) (m)->msg.err = e  /* catch functions that don't set err */
#else
#define TCIP_APIMSG_SET_ERR(m, e)
#endif
#define TCPIP_APIMSG_NOERR(m,f) do { \
  TCIP_APIMSG_SET_ERR(m, ERR_VAL); \
  LOCK_TCPIP_CORE(); \
  f(&((m)->msg)); \
  UNLOCK_TCPIP_CORE(); \
} while(0)
#define TCPIP_APIMSG(m,f,e)   do { \
  TCPIP_APIMSG_NOERR(m,f); \
  (e) = (m)->msg.err; \
} while(0)
#define TCPIP_APIMSG_ACK(m)
#define TCPIP_NETIFAPI(m)     tcpip_netifapi_lock(m)
#define TCPIP_NETIFAPI_ACK(m)
#else /* LWIP_TCPIP_CORE_LOCKING */
#define LOCK_TCPIP_CORE()
#define UNLOCK_TCPIP_CORE()
#define TCPIP_APIMSG_NOERR(m,f)      do { (m)->function = f; tcpip_apimsg(m); } while(0)
#define TCPIP_APIMSG(m,f,e)          do { (m)->function = f; (e) = tcpip_apimsg(m); } while(0)
#define TCPIP_APIMSG_ACK(m)          do { if (NULL == m) { \
                                            LWIP_DEBUGF(DIAG_ON_DEBUG, ("lwiperr: %s-%d, TCPIP_APIMSG_ACK error.", __FUNCTION__, __LINE__)); \
                                            break; \
                                          } \
                                          LWIP_DEBUGF(API_LIB_DEBUG, ("%s,signal,conn=%lx,msg=%lx,err=%d,semsig=%lx", __FUNCTION__, m->conn, m, m->err, &m->op_completed_sem));  \
                                          sys_sem_signal(&m->op_completed_sem); } while(0)

#define TCPIP_NETIFAPI(m)     tcpip_netifapi(m)
#define TCPIP_NETIFAPI_ACK(m) sys_sem_signal(&m->sem)
#endif /* LWIP_TCPIP_CORE_LOCKING */

/** Function prototype for the init_done function passed to tcpip_init */
typedef void (*tcpip_init_done_fn)(void *arg);
/** Function prototype for functions passed to tcpip_callback() */
typedef void (*tcpip_callback_fn)(void *ctx);
/** Function prototype for functions passed to tcpip_callback() */
typedef void (*tcpip_select_fn)(int event, void *ctx, void *arg);

/* Forward declarations */
struct tcpip_callback_msg;

void tcpip_init(tcpip_init_done_fn tcpip_init_done, void *arg);

#if LWIP_NETCONN
err_t tcpip_apimsg(struct api_msg *apimsg);
#endif /* LWIP_NETCONN */

err_t tcpip_input(struct pbuf *p, struct netif *inp);
err_t tcpip_input_tx_usb(struct pbuf *p, struct netif *inp);
err_t tcpip_input_rx(struct pbuf *p, struct netif *inp);
#if LWIP_NETIF_ETH
err_t tcpip_input_tx_eth(struct pbuf *p, struct netif *inp);
#endif
#if LWIP_NETIF_WF_UAP
err_t tcpip_input_tx_uap(struct pbuf *p, struct netif *inp);
err_t tcpip_input_ack_uap(struct pbuf *p, struct netif *inp);
#endif

#if LWIP_NETIF_API
err_t tcpip_netifapi(struct netifapi_msg *netifapimsg);
#if LWIP_TCPIP_CORE_LOCKING
err_t tcpip_netifapi_lock(struct netifapi_msg *netifapimsg);
#endif /* LWIP_TCPIP_CORE_LOCKING */
#endif /* LWIP_NETIF_API */

err_t tcpip_callback_with_block(tcpip_callback_fn function, void *ctx, u8_t block, const char *func, const int line);
#define tcpip_callback(f, ctx)                      tcpip_callback_with_block(f, ctx, 1, __FUNCTION__, __LINE__)
#define tcpip_callback_noblock(f, ctx)              tcpip_callback_with_block(f, ctx, 0, __FUNCTION__, __LINE__)

struct tcpip_callback_msg* tcpip_callbackmsg_new(tcpip_callback_fn function, void *ctx);
void   tcpip_callbackmsg_delete(struct tcpip_callback_msg* msg);
err_t  tcpip_trycallback(struct tcpip_callback_msg* msg);

#if 0
/* free pbufs or heap memory from another context without blocking */
err_t pbuf_free_callback(struct pbuf *p);
err_t mem_free_callback(void *m);
#endif

#if LWIP_TCPIP_TIMEOUT
err_t tcpip_timeout(u32_t msecs, sys_timeout_handler h, void *arg);
err_t tcpip_untimeout(sys_timeout_handler h, void *arg);
#endif /* LWIP_TCPIP_TIMEOUT */

enum tcpip_msg_type {
  TCPIP_MSG_INVALID = 0, /*for default, memset to 0*/
#if LWIP_NETCONN
  TCPIP_MSG_API,
#endif /* LWIP_NETCONN */
  TCPIP_MSG_INPKT,
#if LWIP_NETIF_API
  TCPIP_MSG_NETIFAPI,
#endif /* LWIP_NETIF_API */
#if LWIP_TCPIP_TIMEOUT
  TCPIP_MSG_TIMEOUT,
  TCPIP_MSG_UNTIMEOUT,
#endif /* LWIP_TCPIP_TIMEOUT */
  TCPIP_MSG_CALLBACK,
  TCPIP_MSG_CALLBACK_STATIC,
  TCPIP_MSG_PPP,
  TCPIP_MSG_MBIM
};

struct tcpip_msg {
  enum tcpip_msg_type type;
  sys_sem_t *sem;
  union {
#if LWIP_NETCONN
    struct api_msg *apimsg;
#endif /* LWIP_NETCONN */
#if LWIP_NETIF_API
    struct netifapi_msg *netifapimsg;
#endif /* LWIP_NETIF_API */
    struct {
      struct pbuf *p;
      struct netif *netif;
    } inp;
    struct {
      tcpip_callback_fn function;
      void *ctx;
    } cb;
#if LWIP_TCPIP_TIMEOUT
    struct {
      u32_t msecs;
      sys_timeout_handler h;
      void *arg;
    } tmo;
#endif /* LWIP_TCPIP_TIMEOUT */
  } msg;

#if 0 //!SYS_MQ_USED
  lwip_mq_node node;
#endif

};

typedef enum
{
    TCPIP_MBOX_ACK_TYPE = 0,
    TCPIP_MBOX_TX_TYPE,
    TCPIP_MBOX_RX_TYPE,
    TCPIP_MBOX_AP_TYPE,
    TCPIP_MBOX_RX_IMS_TYPE,
    TCPIP_MBOX_RX_ACK_TYPE,
    TCPIP_MBOX_ARP_TYPE,
    TCPIP_MBOX_ARP_TYPE_END
} tcpip_mbox_type_enums;

struct _lwip_mq_node_
{
    void            *msg;
    struct _lwip_mq_node_  *next;
};
typedef struct _lwip_mq_node_ lwip_mq_node;

struct _lwip_mq_
{
    unsigned int    *head;
    unsigned int    *tail;
    int             count;
    int             size;
    int             head_cnt;
    int             tail_cnt;
};
typedef struct _lwip_mq_ lwip_mq;

#if SYS_MQ_USED
    typedef sys_mbox_t mq_t;
#else
    typedef lwip_mq* mq_t;
#endif

struct _tcp_ip_mbox_
{
    mq_t        mbox_ack; // PRIORITY 0
    mq_t        mbox_tx; // PRIORITY 1
    mq_t        mbox_rx; // PRIORITY 2
    mq_t        mbox_ap; // PRIORITY 3
    mq_t        mbox_rx_ims;
    mq_t        mbox_arp;

    OSFlagRef   mbox_flg;
};
typedef struct _tcp_ip_mbox_ tcp_ip_mbox;

err_t lwip_tcpip_post(tcp_ip_mbox *tcp_ip_mbox_p, msg_t msg, int type, int timeout);

/*define for tcpip man trigger*/
#define TCPIP_MAN_TRIGGER_TYPE_NULL            (0x0000)
#define TCPIP_MAN_TRIGGER_TYPE_TIMER           (0x0001)
#define TCPIP_MAN_TRIGGER_TYPE_CREAT           (0x0002)
#define TCPIP_MAN_TRIGGER_TYPE_ONECE           (0x0003)
#define TCPIP_MAN_TRIGGER_TYPE_KILL            (0x0004)

#if IS_LWIP_PLATFORM_C4M
#define TCPIP_MAN_TRIGGER_CTRL_TABLE_SIZE       16
#define TCPIP_MAN_TRIGGER_NAME_LENGTH           16
#define TCPIP_MAN_TRIGGER_MSGQ_SIZE             32
#else
#define TCPIP_MAN_TRIGGER_CTRL_TABLE_SIZE       32
#define TCPIP_MAN_TRIGGER_NAME_LENGTH           16
#define TCPIP_MAN_TRIGGER_MSGQ_SIZE             64
#endif

#define TCPIP_MAN_TRIGGER_BIT_DEFAULT_SET       0x0000
#define TCPIP_MAN_TRIGGER_BIT_SPECIAL_DEAL      0x0001
#define TCPIP_MAN_TRIGGER_BIT_SUPPORT_UPDATE    0x0002
#define TCPIP_MAN_TRIGGER_BIT_POST_TCPIP_DONE   0x0004
#define TCPIP_MAN_TRIGGER_BIT_TCPFLOW_LOOP      0x0008
#define TCPIP_MAN_TRIGGER_BIT_CALLBACK_LOOP     0x0010

struct _tcpip_man_trigger_msg
{
    u16_t flag;   /*flag setting, follow tcpip_mantrigger hot bit*/
    u16_t delay;  /*delay times, ms as unit*/
    void * cb_arg; /*callback arg*/
    tcpip_callback_fn callback; /*callback function*/
    struct pbuf   *pbuf; /* pbuf record for loop deal; */
    struct netif *netif; /* netif record for loop deal; */
};
typedef struct _tcpip_man_trigger_msg tcpip_man_trigger_msg_t;

struct _tcpip_man_trigger_ctl
{
#if 0
    /** pointer to next in linked list */
    struct _tcpip_man_trigger_ctl *next;
#endif
    u16_t event;
    u16_t flag;        /* flag, like keepalive, need set special deal flag bit hot */
    u16_t ref;         /* refer value for this trigger, only subtract to 0, can be remove */
    u16_t counter;     /* counter */

    u16_t timer_shift; /* tick as unit, shift time = timer_shift * 5ms */
    u16_t timer_inter; /* tick as unit, interval time = timer_shift * 5ms */
    u16_t trigger_max; /* max counter set */
    u16_t trigger_rsvd;

    tcpip_man_trigger_msg_t * trigger_msg;

	OSATimerRef timerRef;
    char name [TCPIP_MAN_TRIGGER_NAME_LENGTH];     /*trigger name*/
};
typedef struct _tcpip_man_trigger_ctl tcpip_man_trigger_ctl_t;


/*Note:
** name size < 16
** timer_shift, tick value, one tick is 5ms, so config 2, is shift 10ms
** timer_inter, tick value
** trigger_max, max trigger occur counter
** if trigger_max = 0, denote this is the special trigger, need another stop check function
*/
err_t tcpip_man_trigger_setApi(char* name, u32_t timer_shift, u32_t timer_inter, u32_t trigger_max, u16_t flag);
err_t tcpip_man_trigger_bringup(u32_t trigger_max, const char* func);
err_t tcpip_man_trigger_loopup(char* name, const char *func,
                                        tcpip_man_trigger_msg_t * trigger_msg);
err_t tcpip_man_trigger_assert(const char *func, u32_t delay);
err_t tcpip_man_trigger_kill(const char *func, char* name);

/*Note: name size < 16*/
err_t tcpip_man_trigger_remove(char* name);

#if LWIP_TCPFLOW_FALG
/*define for lwip flow control design*/
#if IS_LWIP_PLATFORM_C4M
#define TCPIP_FLOW_CTRL_PCB_SIZE       4
#define TCPIP_FLOW_CTRL_MSG_SIZE       128
#else
#define TCPIP_FLOW_CTRL_PCB_SIZE       8
#define TCPIP_FLOW_CTRL_MSG_SIZE       256
#endif

/*flow control pcb hot bit setting*/
#define TCPIP_FLOW_CTRL_FLAG_REG_INIT           0x0001
#define TCPIP_FLOW_CTRL_FLAG_REG_FLOWID         0x0002
#define TCPIP_FLOW_CTRL_FLAG_REG_SOCKET         0x0004
#define TCPIP_FLOW_CTRL_FLAG_REG_NETIF          0x0008
#define TCPIP_FLOW_CTRL_FLAG_REG_NAT            0x0010
#define TCPIP_FLOW_CTRL_FLAG_REG_TFT            0x0020
#define TCPIP_FLOW_CTRL_FLAG_REG_PBUF           0x0040

/*flow control pcb event hot bit setting*/
#define TCPIP_FLOW_CTRL_EVENT_TCP_WIN_FULL      0x0001
#define TCPIP_FLOW_CTRL_EVENT_TCP_WIN_EMPTY     0x0002
#define TCPIP_FLOW_CTRL_EVENT_TCP_PCB_FREE      0x0004
#define TCPIP_FLOW_CTRL_EVENT_CLEAR_SO_FLOW     0x0008
#define TCPIP_FLOW_CTRL_EVENT_TCP_PCB_EXIST     0x0010

struct _tcpip_flow_control_msg {
  enum tcpip_msg_type type;
  struct pbuf *pbuf;
  struct netif *netif;
};
typedef struct _tcpip_flow_control_msg tcpip_flow_control_msg;

struct _tcpip_flow_context_msg {
  struct pbuf *pbuf;
  struct tcp_pcb *tcp;
};
typedef struct _tcpip_flow_context_msg tcpip_flow_control_ctx;

struct _tcpip_flow_control_pcb
{
    u16_t flags;         /* flag, default set to 0 for initial status. */
    u16_t event;         /* event, event hot bit set. */
    u32_t flowid;        /* refer value for this trigger, only subtract to 0, can be remove */
    s16_t sockid;        /* socket id */
    s16_t pcbidx;        /* record flow pcb index*/
    mq_t  mbox_flow;     /* use flow mbox */
#if LWIP_TCP_FLOW_USING_STATIC
    s32_t msgidx;        /* record msg index*/
    tcpip_flow_control_msg *msg_head; /* record msg head pointer*/
#endif
    struct pbuf *pbuf;   /* record the latest pbuf will do flow control*/
    struct tcp_pcb *tcp; /* record tcp pcb*/
};
typedef struct _tcpip_flow_control_pcb tcpip_flow_control_pcb;

void tcpip_flowctrl_pcb_initial(void);
err_t tcpip_flowctrl_pcb_post_msg(struct pbuf * p, struct netif * inp, int timeout);
err_t tcpip_flowctrl_pcb_fetch_msg(msg_t* msg);
void tcpip_flowctrl_pcb_msg_size(int * count);
void tcpip_flowctrl_pcb_reset(tcpip_flow_control_pcb * flow_pcb);
tcpip_flow_control_pcb * tcpip_flowctrl_pcb_select(void);
tcpip_flow_control_pcb * tcpip_flowctrl_pcb_select_empty(void);
tcpip_flow_control_pcb * tcpip_flowctrl_pcb_select_tcppcb(struct tcp_pcb *tcp);
tcpip_flow_control_pcb * tcpip_flowctrl_pcb_select_flowid(u32_t flowid);
void tcpip_flowctrl_pcb_reg_pcb(struct tcp_pcb * pcb);
void tcpip_flowctrl_pcb_reg_pbuf(struct pbuf *p, struct netif * inp);
void tcpip_flowctrl_pcb_deal_event(u16_t event, void *ctx);
s32_t tcpip_flowctrl_pcb_query_event(u16_t event, void *ctx);
#endif

/*****************************************/
/*function defined for lwip simple iptables*/
/****************************************/
typedef enum
{
    TCPIP_IPTABLES_PREROUTING = 0,
    TCPIP_IPTABLES_IP_INPUT,
    TCPIP_IPTABLES_FORWARD,
    TCPIP_IPTABLES_IP_OUTPUT,
    TCPIP_IPTABLES_POSTROUTING,
    TCPIP_IPTABLES_HOOK_END
} tcpip_iptables_hook_enums;

typedef enum
{
    TCPIP_IPTABLES_DROP = 0,
    TCPIP_IPTABLES_PASS,
    TCPIP_IPTABLES_POLICY_END
} tcpip_iptables_policy_enums;

/*hotbit operate flag defined*/
/*8bit as one part, split to 4 part*/
#define TCPIP_IPTABLES_MANGLE_MSS               0x0001UL
#define TCPIP_IPTABLES_MANGLE_TCP_DNS           0x0002UL
#define TCPIP_IPTABLES_MANGLE_FTP_ALG           0x0004UL
#define TCPIP_IPTABLES_NAT_SNAT                 0x0010UL
#define TCPIP_IPTABLES_NAT_DNAT                 0x0020UL
#define TCPIP_IPTABLES_ICMP_FILTER              0x0100UL
#define TCPIP_IPTABLES_IP6_DONGLENAT            0x0200UL

void tcpip_simple_iptables(struct netif *inp, struct pbuf **ppbuf, int ipType, int hook, u32_t flags, int *op);

/*****************************************/
/*function defined for lwip query event */
/****************************************/
#if IS_LWIP_PLATFORM_C4M
#define TCPIP_SELECT_EVENT_MSG_POOL_SIZE         (8)
#else
#define TCPIP_SELECT_EVENT_MSG_POOL_SIZE         (32)
#endif

/*flow control pcb status setting*/
#define TCPIP_SELECT_FLAG_SELECT_INIT     0x0000
#define TCPIP_SELECT_FLAG_SELECT_ONCE     0x0001    /*only do once if select event happen*/
#define TCPIP_SELECT_FLAG_SELECT_ALWAYS   0x0002    /*alway do select evet, untill unreg by caller*/

/*flow control event hot bit setting*/
#define TCPIP_SELECT_NETIF_IPV4_READY     0x0001    /*netif ipv4 config or update event*/
#define TCPIP_SELECT_NETIF_IPV6_READY     0x0002    /*netif ipv6 config or update event*/
#define TCPIP_SELECT_DEFAULT_IPV4_READY   0x0004    /*default netif ipv4 config or update event*/
#define TCPIP_SELECT_DEFAULT_IPV6_READY   0x0008    /*default netif ipv6 config or update event*/

struct _tcpip_select_event_pcb
{
    u16_t flag;   /*flag setting*/
    u16_t event;  /*event set*/

    /*tcpip select context set*/
    struct netif *netif;

    /*callback func and input args set*/
    void *cb_arg;
    tcpip_select_fn cb_fun;

    /*add debug recorder*/
    u32_t debug_tick;
    sys_thread_t debug_thread;
};
typedef struct _tcpip_select_event_pcb tcpip_select_event_pcb;

/*func: tcpip_select_event_select
* description: use tcpip_select_event_select() get select id.
* output:  >= 0, select_id
*          < 0, error number
*/
int tcpip_select_event_select(void);

/*func: tcpip_select_event_reg
* description: use tcpip_select_event_reg() register tcpip_select.
* input: int select_id, get from tcpip_select_event_select();
*        u16_t flag, config TCPIP_SELECT_FLAG_SELECT_ONCE or TCPIP_SELECT_FLAG_SELECT_ALWAYS
*        u16_t event, config event hotbit, can combine it.
*        void *ctx, check context, for netif event select, need config netif pointer(struct netif *).
*        void *arg, arg for callback set by caller.
*        tcpip_select_fn cb, callback set by caller.
* output: 0, success
*         < 0, error number.
*/
int tcpip_select_event_reg(int select_id, u16_t flag, u16_t event,
                                     void *ctx, void *arg, tcpip_select_fn cb);

/*func: tcpip_select_event_unreg
* description: use tcpip_select_event_unreg() unreg select pcb.
* output: 0, success
*         < 0, error number.
*/
int tcpip_select_event_unreg(int select_id);

/*func: tcpip_select_event_hook
* description: process function only called by lwip.
*/
void tcpip_select_event_hook(u16_t event, void * ctx);

/*func: tcpip_select_event_init
* description: initial function only called by lwip.
*/
void tcpip_select_event_init(void);


#if LWIP_IP_RECOVERY_FLAG
void tcpip_ip_recovery_process(struct netif *inp, struct pbuf *p, int ipType, int hook);
#endif
u8_t lwip_get_ul_non_cpy(void);

#ifdef __cplusplus
}
#endif

#endif /* !NO_SYS */

#endif /* __LWIP_TCPIP_H__ */
