#include <stdlib.h>
#include <stdio.h>
#include "cm_sys.h"
#include "cm_virt_at.h"
#include "cm_common.h"
#include "cm_demo_uart.h"
#define  OC_RESP_OK                 (0)     //  0  OC_RESP_OK               正确返回
#define  CM_RESP_CME_ERROR          (1)     //  1  CM_RESP_CME_ERROR        命令错误
#define  CM_RESP_INVALID_PARAM      (2)     //  2  CM_RESP_INVALID_PARAM    参数无效
#define  CM_RESP_TIME_OUT           (3)     //  3  CM_RESP_TIME_OUT         请求超时
#define  CM_RESP_SYS_ERROR          (4)     //  4  CM_RESP_SYS_ERROR        系统错误
int get_len =0;
int send_len = 0;
unsigned char at_data[256]={0};  

void oc_resp_cb(void * param) 
{  
    if(atoi(param) == OC_RESP_OK )
    {        
        get_len = cm_virt_at_get(at_data,sizeof(at_data));
    }
    else if(atoi(param) == CM_RESP_CME_ERROR)
    {
        get_len = cm_virt_at_get(at_data,sizeof(at_data));
    }
    else if(atoi(param) == CM_RESP_INVALID_PARAM)
    {
        get_len = cm_virt_at_get(at_data,sizeof(at_data));
    }
    else if(atoi(param) == CM_RESP_TIME_OUT)
    {
        get_len = cm_virt_at_get(at_data,sizeof(at_data));
    }
    else if(atoi(param) == CM_RESP_SYS_ERROR)
    {
        get_len = cm_virt_at_get(at_data,sizeof(at_data));
    }
    else
    {
        cm_demo_printf("Unknown error\n");
    }
    cm_virt_at_deinit(); 
}
int cm_virt_at_test(unsigned char **cmd,int len)
{
    char operation[20] = {0};
    sprintf(operation, "%s", cmd[2]);
    char at_operation[25] = {0};
    snprintf(at_operation,sizeof(at_operation),"%s\r\n",operation);        
    if(cm_virt_at_init(oc_resp_cb)!=0)
    {
       cm_demo_printf("cm_virt_at_init  err");   
        return -1;
    }
    send_len = cm_virt_at_send((uint8_t *)at_operation,sizeof(at_operation));
    if(send_len < 0)
    {
        cm_demo_printf("cm_virt_at_send err");
        cm_virt_at_deinit();    
        return -1;
    }
    osDelay(2);
    cm_demo_printf("data:%s  send_len:%d get_len:%d\n", at_data,send_len,get_len); 
    return 0;
}