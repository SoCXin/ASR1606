/**
 * @file tcp.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __DRV__SOCKET_H
#define __DRV__SOCKET_H

#include "includes.h"
#include "cm_asocket.h"
#include "cm_sim.h"
#include "sockets.h"

#define MY_TCP_ADDR "60.205.170.65"
#define MY_TCP_PORT 8107

typedef struct
{
    /*设备自身IMEI*/
    char imei[15];
    /*设备SN码*/
    char sn[20];
    /*SIM卡imsi*/
    char imsi[15];
}Device_Par;

typedef struct
{
    /*异步socket描述符*/
    int sock;
    /*tcp配置参数*/
    struct sockaddr_in server_addr;
}TCP_CB;

extern Device_Par MyDevice_Par;
extern TCP_CB MyTCP_CB;

/**
 * @brief 获取设备参数
 * 
 * @return int = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 */
int my_device_par(void);

/**
 * @brief TCP初始化函数
 * 
 * @return int = 0 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 */
// int my_tcp_init(void);

/**
 * @brief 套接字发送函数
 * 
 * @param dataptr 
 * @param size 
 * @return int > 0 - 成功(发送长度) \n
 *    < 0 - 失败, 返回值为错误码
 */
int my_socket_send(int sockect, const void *dataptr, size_t size);

/**
 * @brief socket重连函数
 * 
 * @return int 
 */
int my_socket_reconnect(void);

// int my_tcp_init(int* socket, const char* addr, uint16_t port, void* socket_callback);
int my_tcp_init(void);

#endif // !__DRV__SOCKET_H
