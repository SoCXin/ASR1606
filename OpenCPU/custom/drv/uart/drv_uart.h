#ifndef __DRV_UART_H
#define __DRV_UART_H

#include "cm_uart.h"
#include "cm_iomux.h"

#define DEBUG //开启DBG
#define COLR_LOG_MODE //颜色日志输出模式
extern unsigned log_num;
extern char rxBuff[256];

#ifdef COLR_LOG_MODE
    #define DBG_UART    u0_printf
	#define COLR_SET(dis_mode,fwd_clor,bak_clor) \
	DBG_UART("\033[%d;%d;%dm",dis_mode,fwd_clor,bak_clor)
#else
	#define COLR_SET(dis_mode,fwd_clor,bak_clor) (NULL)
#endif

/*
x:背景色
y:前景色
z:特殊效果
*/
#define DBG_LOG(x,y,z,str,...) {COLR_SET(x,y,z);DBG_UART(str,##__VA_ARGS__);COLR_SET(0,0,0);}
/*
 *  I:INFO W:WARN E:ERR F:FLAG
 */

#ifdef DEBUG

    #define DBG_I(str,...) {COLR_SET(34,34,1);DBG_UART("[%05d] I/%s: ",log_num++,DBG_NAME);DBG_UART(str,##__VA_ARGS__);COLR_SET(0,0,0);}
    #define DBG_W(str,...) {COLR_SET(33,33,1);DBG_UART("[%05d] W/%s: ",log_num++,DBG_NAME);DBG_UART(str,##__VA_ARGS__);COLR_SET(0,0,0);}
    #define DBG_E(str,...) {COLR_SET(31,31,1);DBG_UART("[%05d] E/%s: ",log_num++,DBG_NAME);DBG_UART(str,##__VA_ARGS__);COLR_SET(0,0,0);}
    #define DBG_F(str,...) {COLR_SET(36,36,1);DBG_UART("[%05d] F/%s: ",log_num++,DBG_NAME);DBG_UART(str,##__VA_ARGS__);COLR_SET(0,0,0);}
#else
    #define DBG_I(str,...) while(0);
    #define DBG_W(str,...) while(0);
    #define DBG_E(str,...) while(0);
    #define DBG_F(str,...) while(0);
#endif // DEBUG

/**
 * @brief 打开串口设备
 * 
 * @param dev 串口设备号
 * @param baudrate 波特率
 * @param callback 回调函数
 */
void uart_open(cm_uart_dev_e dev,int baudrate,void* callback);

/**
 * @brief 串口0回调函数
 * 
 * @param param 参数
 * @param type 事件类型
 */
void u0_callback(void *param, uint32_t type);

/**
 * @brief 串口0 Printf函数
 * 
 * @param str 
 * @param ... 
 */
void u0_printf(char *str, ...);

/**
 * @brief 串口0 Read函数
 * 
 * @param data 
 * @return int 
 */
int u0_uart_read(char* data);

#endif // !__DRV_UART_H