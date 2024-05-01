/*********************************************************
 *  @file    cm_demo_uart.c
 *  @brief   OpenCPU UART示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/
#include "cm_uart.h"
#include "cm_demo_uart.h"
#include "cm_iomux.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "cm_os.h"
#include "cm_mem.h"
#include "cm_common.h"
#include "cm_sys.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define UART_BUF_LEN            1024

/****************************************************************************
 * Private Types
 ****************************************************************************/
static int rx_rev_len = 0;
static char rx_rev_data[UART_BUF_LEN] = {0};
static osThreadId_t OC_Uart_TaskHandle; //串口数据接收、解析任务Handle
static void* g_uart_sem = NULL;
extern osEventFlagsId_t cmd_task_flag;
cm_uart_cmd_recv_t gstUartCmdRecv = {0}; //串口命令缓冲区

static osMessageQueueId_t uart_event_queue = NULL;
static osThreadId_t uart_event_thread = NULL;
typedef struct{
    int msg_type;
} uart_event_msg_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void cm_demo_printf (char *str, ...);

static void cm_uart_recv_task(void *param);
static int __cm_cmd_engine(char * prefix,char * uart_buf,int* uart_buf_len,char **cmd_buf,int *cmd_len);
/****************************************************************************
 * Private Data
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void uart_event_task(void *arg)
{

    uart_event_msg_t msg = {0};

    while(1)
    {
        if(osMessageQueueGet(uart_event_queue, &msg, NULL, osWaitForever) == osOK)
        {
            //cm_log_printf(0, "uart event msg type = %d\n", msg.msg_type);
            if(CM_UART_EVENT_TYPE_RX_OVERFLOW & msg.msg_type)
            {
                cm_log_printf(0, "CM_UART_EVENT_TYPE_RX_OVERFLOW... ...");
                
            }

        }
    }
}

static int uart_event_task_create(void)
{
    if(uart_event_queue == NULL)
    {
        uart_event_queue = osMessageQueueNew(10, sizeof(uart_event_msg_t), NULL);
    }

    if(uart_event_thread == NULL)
    {
        osThreadAttr_t attr1 = {
            .name = "uart_event",
            .priority = UART_TASK_PRIORITY,
            .stack_size = 1024,
        };
        uart_event_thread = osThreadNew(uart_event_task, NULL, (const osThreadAttr_t*)&attr1);
    }

    return 0;
}

static int __cm_cmd_engine(char * prefix,char * uart_buf,int* uart_buf_len,char **cmd_buf,int *cmd_len)
{
    char *p1,*p2,*p3,*temp1,*temp2;
    uart_buf[*uart_buf_len] = 0;//结尾
    p1 = strstr(uart_buf,prefix);
    if(p1 == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    p2 = strstr(p1,"\r\n");
    if(p2 == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    *cmd_len = 0;
    p3 = strchr(p1,':');
    if(p3 == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    cmd_buf[(*cmd_len)++] = cm_malloc(p3-p1+1+1);
    memset(cmd_buf[*cmd_len-1],0,p3-p1+1);
    if(cmd_buf[*cmd_len -1] == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    memcpy(cmd_buf[*cmd_len-1],p1,(int)(p3-p1));
    temp1= p3;
    while(1)
    {
        if(*cmd_len >= 20)
        {
            *uart_buf_len = 0;
            for(int j = 0;j< *cmd_len;j++)
            {
                cm_free(cmd_buf[j]);
            }
            return -1;
        }
        temp2 = strchr(temp1+1,':');
        if(temp2 == 0)
            break;        
        if((temp2-temp1) < 1)
        {
             for(int j = 0;j< *cmd_len;j++)
            {
                cm_free(cmd_buf[j]);
            }
            *uart_buf_len = 0;
            return -1;
        }
        cmd_buf[(*cmd_len)++] = cm_malloc(temp2-temp1+1);
        if(cmd_buf[*cmd_len -1] == 0)
        {
             for(int j = 0;j< *cmd_len;j++)
            {
                cm_free(cmd_buf[j]);
            }
            *uart_buf_len = 0;
            return -1;
        }
        memset(cmd_buf[*cmd_len-1],0,temp2-temp1);
        memcpy(cmd_buf[*cmd_len-1],temp1+1,temp2-temp1-1);
        temp1 = temp2;            
    }
    if((p2-temp1-1) < 1)
    {
         for(int j = 0;j< *cmd_len;j++)
        {
            cm_free(cmd_buf[j]);
        }
        *uart_buf_len = 0;
        return -1;
    }
    
    cmd_buf[(*cmd_len)++] = cm_malloc(p2-temp1+1);
    memset(cmd_buf[*cmd_len-1],0,p2-temp1);
    memcpy(cmd_buf[*cmd_len-1],temp1+1,p2-1-temp1);
    *uart_buf_len = 0;
    return 0;
}

/* 回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源，建议以信号量或消息队列形式控制其他线程执行任务 */
static void cm_serial_uart_callback(void *param, uint32_t type)
{
    uart_event_msg_t msg = {0};
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        osSemaphoreRelease(g_uart_sem);
        
    }
    
    if(CM_UART_EVENT_TYPE_RX_OVERFLOW & type)
    {
        msg.msg_type = type;
        if(uart_event_queue != NULL)//向队列发送数据
        {
            osMessageQueuePut(uart_event_queue, &msg, 0, 0);
        }
    }

}


static void cm_uart_recv_task(void *param)
{
    int temp_len = 0;

    cm_uart_cmd_recv_t* pstUartCmdRecv = &gstUartCmdRecv;
    while (1)
    {
        if (g_uart_sem != NULL)
        {
            osSemaphoreAcquire(g_uart_sem, osWaitForever);//阻塞
        }
        if (rx_rev_len < UART_BUF_LEN)
        {
            temp_len = cm_uart_read(OPENCPU_MAIN_URAT, (void*)&rx_rev_data[rx_rev_len], UART_BUF_LEN - rx_rev_len, 1000);
            rx_rev_len += temp_len;
        }
        if (g_uart_sem != NULL && (strstr(rx_rev_data,"\r\n")))
        {
    
            //处理收到数据事件
            if(gstUartCmdRecv.cmd_execute==0)
    {
                gstUartCmdRecv.cmd_execute ++;
                if (rx_rev_len != 0)
                {
                    if(pstUartCmdRecv->cmd_execute != 0)
        {             
            //OC指令参数的格式是否正确
            char *trai_colon_pz = NULL;
            
                        trai_colon_pz = strrchr((const char * )rx_rev_data, ':');
            
            if((trai_colon_pz != NULL)&&((*(trai_colon_pz+1) == '\0') || (*(trai_colon_pz+1) == '\r') ||(*(trai_colon_pz+1) == '\n')))
            {
                            cm_demo_printf("formate error\n");
                            memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                            rx_rev_len = 0;
                pstUartCmdRecv->cmd_execute = 0;
                continue;
            }
            
            //解析命令，如果未检测到正确的命令格式，则丢弃本次数据
                        if(__cm_cmd_engine("CM",rx_rev_data,&rx_rev_len,(char **)(pstUartCmdRecv->buf),&pstUartCmdRecv->len) == -1)
            {
                            cm_demo_printf("CM CMD error\n");
                            memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                            rx_rev_len = 0;
                pstUartCmdRecv->cmd_execute = 0;
            }
            else
            {
                //清空数据缓冲，并提示主任务开始执行命令
                            memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                            rx_rev_len = 0;
                osEventFlagsSet(cmd_task_flag, 0x00000001U);  
            }
                    }	
                }
            }
            else
            {
                memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                rx_rev_len = 0;
                cm_demo_printf("Uart busy\n");
            }
        }
        
        
    }
    
}



/****************************************************************************
 * Public Functions
 ****************************************************************************/
void cm_demo_printf (char *str, ...)
{
    static char s[600]; //This needs to be large enough to store the string TODO Change magic number
    va_list args;
    int len;
    
    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start (args, str);
    len = vsnprintf ((char*)s, 600, str, args);
    va_end (args);
    cm_uart_write(OPENCPU_MAIN_URAT, s, len, 1000);
}

/* 若要修改串口引脚请在cm_common.h中修改宏定义 */
void cm_demo_uart(void)
{

    int32_t ret = -1;
    cm_uart_cfg_t config = 
    {
        CM_UART_BYTE_SIZE_8, 
        CM_UART_PARITY_NONE,
        CM_UART_STOP_BIT_ONE, 
        CM_UART_FLOW_CTRL_NONE, 
        CM_UART_BAUDRATE_9600,
        0
    };
    cm_uart_event_t event = 
    {
        CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,
        "uart0",
        cm_serial_uart_callback
    };

    cm_log_printf(0, "uart NUM = %d demo start... ...", OPENCPU_MAIN_URAT);
    
    cm_iomux_set_pin_func(OPENCPU_TEST_UARTTX_IOMUX);
    cm_iomux_set_pin_func(OPENCPU_TEST_UARTRX_IOMUX);
    
    ret = cm_uart_register_event(OPENCPU_MAIN_URAT, &event);
    
    if(ret != RET_SUCCESS)
    {
        cm_log_printf(0, "uart register event err,ret=%d\n", ret);
        return;
    }
    ret = cm_uart_open(OPENCPU_MAIN_URAT, &config);
    
    if(ret != RET_SUCCESS)
    {
        cm_log_printf(0, "uart init err,ret=%d\n", ret);
        return;
    }

#if CM_OPENCPU_MODEL_ML302A
    if(OPENCPU_MAIN_URAT == CM_UART_DEV_1)
    {
        /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
        cm_iomux_set_pin_cmd(OPENCPU_UART_WEKEUP_PIN, CM_IOMUX_PINCMD1_LPMEDEG, CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
    }
#else
    if(OPENCPU_MAIN_URAT == CM_UART_DEV_0)
    {
        /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
        cm_iomux_set_pin_cmd(OPENCPU_UART_WEKEUP_PIN, CM_IOMUX_PINCMD1_LPMEDEG, CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
    }
#endif

    cm_log_printf(0, "cm_uart_register_event start... ...\n");
    
    osThreadAttr_t uart_task_attr = {0};
    uart_task_attr.name = "uart_task";
    uart_task_attr.stack_size = 2048;
    uart_task_attr.priority= UART_TASK_PRIORITY;
    gstUartCmdRecv.cmd_execute = 0;
    if(g_uart_sem == NULL)
        g_uart_sem = osSemaphoreNew(1, 0, NULL);

    OC_Uart_TaskHandle = osThreadNew(cm_uart_recv_task,0,&uart_task_attr);

    uart_event_task_create();
}

void cm_test_uart_close(char **cmd, int len)
{
    cm_uart_dev_e dev = atoi(cmd[2]);
    
    if(0 == cm_uart_close(dev))
    {
        cm_demo_printf("uart%d close is ok\n", dev);
    }
    else
    {
        cm_demo_printf("uart%d close is error\n", dev);
    }
}
