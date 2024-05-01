#include "includes.h"
#include "drv_uart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "app.h"

static char txBuff0[256];
char rxBuff[256];
unsigned log_num = 0;

void u0_callback(void *param, uint32_t type)
{

	if(CM_UART_EVENT_TYPE_RX_ARRIVED & type){
    /* 接收到新的数据  */
        osSemaphoreRelease(u0_uart_sem);

    }else if(CM_UART_EVENT_TYPE_RX_OVERFLOW & type){
    /* 接收FIFO缓存溢出 */
    
    }
    
}

void uart_open(cm_uart_dev_e dev,int baudrate,void* callback)
{
    cm_uart_cfg_t config = 
    {
        CM_UART_BYTE_SIZE_8, 
        CM_UART_PARITY_NONE,
        CM_UART_STOP_BIT_ONE, 
        CM_UART_FLOW_CTRL_NONE, 
        baudrate
    };
    cm_uart_event_t event = 
    {
        CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,
        "NULL",
        callback
    };

    if(dev == CM_UART_DEV_0){
        cm_iomux_set_pin_func(CM_IOMUX_PIN_17,CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_18,CM_IOMUX_FUNC_FUNCTION1);
        event.event_param = "uart0";
    }else if(dev == CM_UART_DEV_1){
        cm_iomux_set_pin_func(CM_IOMUX_PIN_28,CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_29,CM_IOMUX_FUNC_FUNCTION1);
        event.event_param = "uart1";
    }
    else if(dev == CM_UART_DEV_2){
        cm_iomux_set_pin_func(CM_IOMUX_PIN_50,CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_51,CM_IOMUX_FUNC_FUNCTION1);
        event.event_param = "uart2";
    }

    cm_uart_register_event(dev,&event);

    cm_uart_open(dev,&config);
    
    if(dev == CM_UART_DEV_0){
        /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
        cm_iomux_set_pin_cmd(CM_IOMUX_PIN_17,CM_IOMUX_PINCMD1_LPMEDEG,CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
    }
}

void u0_printf(char *str, ...)
{
    va_list args;
    int len;
    
    if((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start (args,str);
    len = vsnprintf((char*)txBuff0,256,str,args);
    va_end(args);
    cm_uart_write(CM_UART_DEV_0,txBuff0,len,1000);
}

int u0_uart_read(char* data)
{
    int ret = EOF;
    memset(data, 0, strlen(data));
    ret = cm_uart_read(CM_UART_DEV_0, (void*)data, 256, 1000);
    return ret;
}


