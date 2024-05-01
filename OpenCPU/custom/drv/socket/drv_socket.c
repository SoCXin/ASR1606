/**
 * @file tcp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "drv_socket.h"
#include "drv_uart.h"
// #include "bsp_socket.h"
// #include "bsp_call.h"


#define DBG_NAME "tcp"

Device_Par MyDevice_Par;
TCP_CB MyTCP_CB;

char recv_buf[324+8];

/* 异步socket消息回调(执行环境为eloop线程) */
static void __on_asocket_event(int sock, cm_asocket_event_e event, void *user_param)
{
    switch (event)
    {
    // responses
    case CM_ASOCKET_EV_CONNECT_OK: {
        /*直接赋值为1清除其他位*/
        // MySocket_CB.status = 1;
        DBG_I("sock(%d) connect_ok\r\n", sock);
        break;
    }
    case CM_ASOCKET_EV_CONNECT_FAIL: {
        DBG_I("sock(%d) connect_fail\r\n", sock);
        break;
    }

    // indications
    case CM_ASOCKET_EV_RECV_IND: {
        /* 取得获取接收缓存中可读的数据长度 */
        int recv_avail = 0;
        cm_asocket_ioctl(MyTCP_CB.sock, FIONREAD, &recv_avail);

        /* 接收数据 */
        memset(recv_buf, 0, 324+8);
        int ret = cm_asocket_recv(MyTCP_CB.sock, recv_buf, sizeof(recv_buf), 0);
        if (ret > 0){
            // my_socket_recv_handle(recv_buf);
            // DBG_I("sock(%d) recv_ind: recv_avail=%d, recv_len=%d, data=%x %x %x %x %x %x %x %x %x\r\n"
            //         , MyTCP_CB.sock, recv_avail, ret,
            //         recv_buf[0],recv_buf[1],recv_buf[2],recv_buf[3],recv_buf[4],recv_buf[5],recv_buf[6],recv_buf[7],recv_buf[8]);
        }
        else
        {
            DBG_I("sock(%d) recv_ind error(%d)\r\n", MyTCP_CB.sock, errno);

            if (ENOTCONN == errno)
            {
                /* Connection closed */
                /*全部状态清空*/
                // MySocket_CB.status = 0;
                // my_netled_status_sw(0);
                DBG_I("sock(%d) recv_ind: Connection closed\r\n", MyTCP_CB.sock);
            }
        }
        break;
    }
    case CM_ASOCKET_EV_SEND_IND:
        // DBG_I("sock(%d) send_ind\r\n", MyTCP_CB.sock);
        break;
    case CM_ASOCKET_EV_ACCEPT_IND:
        // DBG_I("sock(%d) accept_ind\r\n", MyTCP_CB.sock);
        break;
    case CM_ASOCKET_EV_ERROR_IND: {
        /* 获取socket错误码 */
        int sock_error = 0;
        socklen_t opt_len = sizeof(sock_error);
        cm_asocket_getsockopt(MyTCP_CB.sock, SOL_SOCKET, SO_ERROR, &sock_error, &opt_len);
        DBG_I("sock(%d) error_ind: sock_error(%d)\r\n", MyTCP_CB.sock, sock_error);
        if (ECONNABORTED == sock_error)
        {
            /* Connection aborted */
            DBG_I("sock(%d) error_ind: Connection aborted\r\n", MyTCP_CB.sock);
        }
        else if (ECONNRESET == sock_error)
        {
            /* Connection reset */
            DBG_I("sock(%d) error_ind: Connection reset\r\n", MyTCP_CB.sock);
        }
        else if (ENOTCONN == sock_error)
        {
            /* Connection closed */
            DBG_I("sock(%d) error_ind: Connection closed\r\n", MyTCP_CB.sock);
        }
        break;
    }
    default:
        break;
    }
}

int my_device_par()
{
    int ret = 0;
    /*获取IMEI*/
    while ((ret = cm_sys_get_imei(MyDevice_Par.imei))!=0){
        DBG_W("Get device IMEI failed: %d\r\n",ret);
        osDelayMs(10);
    }
    DBG_I("Device IMEI: %s\r\n",MyDevice_Par.imei);
    /*获取SN*/
    while ((ret = cm_sys_get_sn(MyDevice_Par.sn))!=0){
        DBG_W("Get device SN failed: %d\r\n",ret);
        osDelayMs(10);
    }
    DBG_I("Device SN  : %s\r\n",MyDevice_Par.sn);
    /*获取SIM卡IMSI*/
    while ((ret = cm_sim_get_imsi(MyDevice_Par.imsi))!=0){
        DBG_W("Get SIM IMSI failed: %d\r\n",ret);
        osDelayMs(2000);
    }
    DBG_I("SIM IMSI  : %s\r\n",MyDevice_Par.imsi);
    return ret;
}

// int my_tcp_init(int* socket, const char* addr, uint16_t port, void* socket_callback)
// {
//     int ret = 0;
//     struct sockaddr_in server_addr = {
//         .sin_addr.s_addr = inet_addr(addr),
//         .sin_family = AF_INET,
//         .sin_port = htons(port)
//     };
//     /* TCP */
//     *socket = cm_asocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP, socket_callback, NULL);
//     if(*socket==-1)DBG_E("cm_asocket_open failed\r\n");

//     /*开启TCP连接*/
//     ret = cm_asocket_connect(*socket, (const struct sockaddr *)&server_addr, sizeof(server_addr));
//     if(ret < 0){
//         if (EINPROGRESS == errno){
//             DBG_I("rsock(%d) open request success, wait connect...\r\n", *socket);
//             ret = 0;
//         }else{
//             DBG_E("rsock(%d) connect %s:%d failed(%d)\r\n", *socket, addr, port, errno);
//         }
//     }

//     return ret;
// }

int my_tcp_init(void)
{
    int ret = 0;
    MyTCP_CB.sock = -1;
    struct sockaddr_in server_addr = {
        .sin_addr.s_addr = inet_addr(MY_TCP_ADDR),
        .sin_family = AF_INET,
        .sin_port = htons(MY_TCP_PORT)
    };
    /* TCP */
    MyTCP_CB.sock = cm_asocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP, __on_asocket_event, NULL);
    if(MyTCP_CB.sock==-1)DBG_E("cm_asocket_open failed\r\n");

    /*开启TCP连接*/
    ret = cm_asocket_connect(MyTCP_CB.sock, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret < 0){
        if (EINPROGRESS == errno){
            DBG_I("rsock(%d) open request success, wait connect...\r\n", MyTCP_CB.sock);
            ret = 0;
        }else{
            DBG_E("rsock(%d) connect %s:%d failed(%d)\r\n", MyTCP_CB.sock, MY_TCP_ADDR, MY_TCP_PORT,errno);
        }
    }
    return ret;
}

int my_socket_reconnect(void)
{
    int ret = 0;
    // struct sockaddr_in server_addr = {
    //     .sin_addr.s_addr = inet_addr(MY_TCP_ADDR),
    //     .sin_family = AF_INET,
    //     .sin_port = htons(MY_TCP_PORT)
    // };
    // /*开启TCP连接*/
    // ret = cm_asocket_connect(MyTCP_CB.sock, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    // if(ret < 0){
    //     if (EINPROGRESS == errno){
    //         ret = 0;
    //     }
    // }
    // ret = my_tcp_init();
    return ret;
}

int my_socket_send(int sockect, const void *dataptr, size_t size)
{
    int ret = 0;
    ret = cm_asocket_send(sockect, dataptr, size, 0);
    return ret;
}