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
#ifndef __LWIP_OPTS_NB_H__
#define __LWIP_OPTS_NB_H__

#define LWIP_VERSION_PLATFORM           	"crane-NBIOT"

#undef LWIP_CUSTOMER_HOOK
#define LWIP_CUSTOMER_HOOK              (0)
#undef LWIP_SOCKET_DEBUG_OP_FLAG
#define LWIP_SOCKET_DEBUG_OP_FLAG       (0)
#undef LWIP_NETCONN_DEBUG_OP_FLAG
#define LWIP_NETCONN_DEBUG_OP_FLAG      (0)
#undef LWIP_NETCONN_EVENT_OP_FLAG
#define LWIP_NETCONN_EVENT_OP_FLAG      (0)
#undef LWIP_APP_NAT_TABLE_FLAG
#define LWIP_APP_NAT_TABLE_FLAG         (0)
#undef LWIP_DEBUG
#define LWIP_DEBUG                      LWIP_DBG_OFF /*LWIP_DBG_ON or LWIP_DBG_OFF */
#undef LWIP_DEBUG_DISABLE_ALL
#define LWIP_DEBUG_DISABLE_ALL          (1)

#define DEFAULT_SPREAD_FACTOR           (2) 
#define DEFAULT_SPREAD_FACTOR_DOUBLE    (4) 

#define PC_USB_UL_BUF_SIZE          	(1692)
#define LWIP_IPNET_UL_HEADER_LEN    	(128)
#define UL_PMSG_SIZE                	(LWIP_IPNET_UL_HEADER_LEN + 2)

#define LWIP_RATE_TXACK_DEFAULT_SET         (22)
#define LWIP_RATE_TXDATA_DEFAULT_SET        (22)
#define LWIP_RATE_RXIMS_DEFAULT_SET         (32)
#define LWIP_RATE_RXFLOW_DEFAULT_SET        (16)
#define LWIP_RATE_RXDATA_DEFAULT_SET        (8)
#define LWIP_RATE_AP_EFAULT_SET             (32)

/*macro define for msg_size and mbox queue size, 
* mbox queue size setting need according to msg_size, 
* adjust mbox queue size, set smaller than msg_size
* avoid msg_pool reverse using, cause to memory flush
*/
#define ARP_MSG_SIZE                (32)
#define TX_MSG_SIZE                 (256)
#define PPP_TX_MSG_SIZE             (TX_MSG_SIZE / 2)
#define LTE_MSG_SIZE                (512)
#define LTE_IMS_MSG_SIZE            (256)
#define ACK_MSG_SIZE                (128)

#define LWIP_TCPIP_RATE     		(8)
#define LWIP_MQ_ARP_SIZE            (28)
#define LWIP_MQ_TX_SIZE        		(240)  //TX_MSG_SIZE (64)
#define LWIP_MQ_NW_SIZE        		(480) //LTE_MSG_SIZE (512)
#define LWIP_MQ_NW_IMS_SIZE         (240) //LTE_IMS_MSG_SIZE (256)
#define LWIP_MQ_ACK_SIZE    		(120) //ACK_MSG_SIZE (128)
#define LWIP_MQ_AP_SIZE       		(192)
#define LWIP_CPSR_OP_TICK_CNT   	(16)
#define LWIP_LOCK_PP_LEVEL          (6)

/*****************************************
*lwip macro define for different customer
*this macro need check before compiler for different customer
*default settings is for common 5M
******************************************/
#define LWIP_RA_DNS_OPT_IPV6ONLY        (0)  //this macro control ra dns option
#define LWIP_MAC_FILTER_FLG             (0)  //enable or disable mac filter for forward chain
#define LWIP_WAN_ACCESS_PORT_REDIRECT   (0)
#define LWIP_USING_TP_METHOD_ADJUST_PP  (0)
#define LWIP_DSIM_SUPPORT               (0)
#define LWIP_TCPIP_NO_LOCK              (1)
#define LWIP_BRIDGE_FASTPATH_DONGLE     (0)

#define IP_FILTER_EN			        (0)
#define LWIP_GETSET_CONN_OPT            (0)
#define LWIP_IP_RECOVERY_FLAG        	(0)
#define LWIP_DUMP_IP_HEADER             (0)
#define LWIP_REDIRECT					(0)

#define LWIP_IPSEC				        (0)
#define LWIP_TCPIP_MSG_DOUBLE_CHECK     (1)
#define LWIP_TCP_WRITE_STAGE_OPT        (1)
#define LWIP_SYS_PROTECT_USING_MUTEX    (0)
#define LWIP_SO_BINDTODEVICE            (1)

/***************************************/
/*fearture macro define*/
#define LWIP_NETIF_WF_UAP               (0)
#define LWIP_NETIF_BRIDGE               (0)
#define LWIP_NETIF_LTE_TEST             (0)
#define LWIP_NETIF_PC                   (0)
#define LWIP_NETIF_LTE                  (1)
#define LWIP_NETIF_PPP                  (0)
#define LWIP_NETIF_MBIM                 (0)
#define LWIP_NETIF_TUNE                 (0)

#define LWIP_CHKSUM_ALGORITHM           3
#define MEM_ALIGNMENT                   4
#define MEMP_NUM_RAW_PCB                (8 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define MEMP_NUM_UDP_PCB                (32 / DEFAULT_SPREAD_FACTOR)
#define MEMP_NUM_TCP_PCB                (64 / DEFAULT_SPREAD_FACTOR)
#define MEMP_NUM_TCP_PCB_LISTEN         (32 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define MEMP_NUM_TCP_SEG                (64 / DEFAULT_SPREAD_FACTOR)
#define MEMP_NUM_REASSDATA              (32 / DEFAULT_SPREAD_FACTOR)
#define MEMP_NUM_ARP_QUEUE              (16 / DEFAULT_SPREAD_FACTOR)
#define MEMP_NUM_NETDB                  (16 * 4  / DEFAULT_SPREAD_FACTOR) /*support get multi-addr answers, DNS_MAX_ANS_NUM_GET = 4*/
#define MEMP_NUM_NETBUF                 (512 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define MEMP_NUM_NETCONN                (128 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define MEMP_NUM_SYS_TIMEOUT            (128 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define MEMP_NUM_PBUF                   (512)
#define MEMP_NUM_TCPIP_MSG_API          (512 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define MEMP_NUM_TCPIP_MSG_INPKT        (512 / DEFAULT_SPREAD_FACTOR_DOUBLE)
#define PBUF_POOL_SIZE                  (64)
#define ARP_TABLE_SIZE                  (16 * 2)
#define IP_REASS_MAX_PBUFS              (64 / DEFAULT_SPREAD_FACTOR)

#define DEFAULT_THREAD_STACKSIZE        (4096)
#define DEFAULT_THREAD_PRIO             90
#define TCPIP_THREAD_STACKSIZE          (4096)
#define TCPIP_THREAD_PRIO               80
#define DNSR_THREAD_PRIO                TCPIP_THREAD_PRIO
#define DHCPD_THREAD_PRIO               TCPIP_THREAD_PRIO
#define MANTRIGGER_THREAD_PRIO          TCPIP_THREAD_PRIO

#define TCPIP_MBOX_SIZE                 (512) /*no use*/
#define DEFAULT_RAW_RECVMBOX_SIZE       (256 / DEFAULT_SPREAD_FACTOR)
#define DEFAULT_UDP_RECVMBOX_SIZE       (256 / DEFAULT_SPREAD_FACTOR)
#define DEFAULT_TCP_RECVMBOX_SIZE       (256 / DEFAULT_SPREAD_FACTOR)
#define DEFAULT_ACCEPTMBOX_SIZE         (256 / DEFAULT_SPREAD_FACTOR)
#define MTU_MAX_SET                     1500            //fixed to 1500
#define TCP_MSS                         1460

#define LWIP_IPV6                       1
#define LWIP_IPV6_MLD                   0
#define LWIP_IPV6_FORWARD               0
#define LWIP_IPV6_FRAG                  1

#define LWIP_RAW                        1
#define IP_FORWARD                      0
#define LWIP_DHCP                       0
#define LWIP_IGMP                       0
#define LWIP_DNS                        1
#define LWIP_NETIF_API                  0
#define ARP_QUEUEING                    0
#define IP_FRAG_USES_STATIC_BUF         1
#define LWIP_DHCP_SERVER			    0
#define ETHARP_SUPPORT_STATIC_ENTRIES   0
#define LWIP_IPV6_DHCP6                 0
#define LWIP_DHCP6_SERVER			    0
#define LWIP_ARP                        0

#define LWIP_CHECKSUM_ON_COPY           1
#define CHECKSUM_CHECK_IP               0
#define CHECKSUM_CHECK_UDP              0
#define CHECKSUM_GEN_UDP                1

#define TCP_DNS_TURN_EN                 0
#define TCP_MSS_ADJUST_EN               0
#define UDP_ECHO_SVR                    0
#define TCP_ECHO_SVR                    0
#define LWIP_DNS_RELAY                  0
#define LWIP_NETIF_MULTI_PDP            1
#define IP_NAT						    0
#define IP_NAT_FTP_ALG                  0
#define IP_NAT_FRAG_EN                  0
#define LWIP_SIP_ALG			        0
#define LWIP_PPTP_ALG			        0
#define LWIP_BIH_46			        	0

#define TCP_LISTEN_BACKLOG              1
#define ERRNO                           1

#define NO_SYS                          0
#define LWIP_STATS                      0
#define ETHARP_TRUST_IP_MAC             0
#define LWIP_PROVIDE_ERRNO	            1
#define LWIP_COMPAT_MUTEX               1

#if 0
#define MEM_USE_POOLS                   1
#define MEMP_USE_CUSTOM_POOLS           1
#define MEM_LIBC_MALLOC                 0
#define MEMP_MEM_MALLOC                 0
#define SYS_LIGHTWEIGHT_PROT            1
#define MEMP_OVERFLOW_CHECK             0
#endif
#define MEM_USE_POOLS                   0
#define MEMP_USE_CUSTOM_POOLS           0
#define MEM_LIBC_MALLOC                 1
#define MEMP_MEM_MALLOC                 1
#define SYS_LIGHTWEIGHT_PROT            1
#define MEMP_OVERFLOW_CHECK             0

#define LWIP_NO_STATIC                  0 //close lwip static flag, default set is disable; open static
#define LWIP_DNS_API_ERROR_TURN         1

#endif /* __LWIP_OPTS_H__ */
