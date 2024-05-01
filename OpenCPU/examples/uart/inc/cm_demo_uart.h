/*********************************************************
*  @file    cm_demo_uart.h
*  @brief   ML302 OpenCPU main header file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by CaoChi 2020/12/10
********************************************************/
#ifndef __CM_UART_DEMO_H__
#define __CM_UART_DEMO_H__
#include "cm_os.h"
#include "cm_common.h"

#define CM_PRT_INF(fmt,arg...)    cm_demo_printf("[CM_INF]line %d in %s:"fmt,__LINE__,__FILE__,##arg)
#define CM_PRT_WRN(fmt,arg...)    cm_demo_printf("[CM_WRN]line %d in %s:"fmt,__LINE__,__FILE__,##arg)
#define CM_PRT_ERR(fmt,arg...)    cm_demo_printf("[CM_ERR]line %d in %s:"fmt,__LINE__,__FILE__,##arg)

/**
 *  \brief 串口接收数据结构体
 */
typedef struct 
{
    unsigned char buf[1024];
    int len;
}cm_uart_recv_data_buf_t;
/**命令结构体*/
typedef struct cm_cmd {
    char * cmdstr;
    void (* cmdfunc)(unsigned char **,int);
} cm_cmd_t;

/**
 *  \brief 串口接收命令接口
 */
typedef struct 
{
    int cmd_execute;  //命令执行标志，1未被执行，0执行完成
    unsigned char * buf[20]; //最多接受20个参数
    int len;
}cm_uart_cmd_recv_t;

extern cm_uart_cmd_recv_t gstUartCmdRecv;

void cm_demo_uart(void);
void cm_demo_printf (char *str, ...);

void cm_test_uart_close(char **cmd, int len);

#endif
