/**
 * @file         cm_demo_ntp.c
 * @brief        NTP接口
 * @copyright    Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author       WangPeng
 * @date         2021/07/26
 *
 */

#include "stdio.h"
#include "cm_ntp.h"
#include "cm_demo_ntp.h"
#include "cm_demo_uart.h"

static void __cm_ntp_cb(cm_ntp_event_e event, void *event_param, void *cb_param)
{
    if (CM_NTP_EVENT_SYNC_OK == event)
    {
        cm_demo_printf("[NTP][%s] get net time %s\n", (char *)cb_param, event_param); 
    }
    else if (CM_NTP_EVENT_SETTIME_FAIL == event)
    {
        cm_demo_printf("[NTP][%s] get net time %s OK, but set time fail\n", (char *)cb_param, event_param); 
    }
    else
    {
        cm_demo_printf("[NTP][%s] error\n", (char *)cb_param); 
    }
}

/**
 *  UART口NTP功能功能调试使用示例
 *  CM:NTP:GETNETTIME                           //仅获取网络时间，不同步至本地时间
 *  CM:NTP:SYNCHTIME                            //获取网络时间并将网络时间与本地时间同步
 */
void cm_test_ntp(unsigned char **cmd,int len)
{
    unsigned char operation[20] = {0};
    sprintf((char *)operation, "%s", cmd[2]);

    if (0 == strcmp((const char *)operation, "GETNETTIME"))
    {
        uint16_t port = 123;
        uint32_t timeout = 6000;
        uint32_t dns_priority = 1;
        bool set_rtc = 0;

        cm_ntp_set_cfg(CM_NTP_CFG_SERVER, "cn.ntp.org.cn"); 
        cm_ntp_set_cfg(CM_NTP_CFG_PORT, &port);
        cm_ntp_set_cfg(CM_NTP_CFG_TIMEOUT, &timeout);
        cm_ntp_set_cfg(CM_NTP_CFG_DNS, &dns_priority);
        cm_ntp_set_cfg(CM_NTP_CFG_SET_RTC, &set_rtc);
        cm_ntp_set_cfg(CM_NTP_CFG_CB, __cm_ntp_cb);
        cm_ntp_set_cfg(CM_NTP_CFG_CB_PARAM, "GETNETTIME");

        if (0 != cm_ntp_sync())
        {
            cm_demo_printf("[NTP]cm_ntp_sync() fail\n");
        }
    }
    else if (0 == strcmp((const char *)operation, "SYNCHTIME"))
    {
        uint16_t port = 123;
        uint32_t timeout = 6000;
        uint32_t dns_priority = 1;
        bool set_rtc = 1;

        cm_ntp_set_cfg(CM_NTP_CFG_SERVER, "cn.ntp.org.cn"); 
        cm_ntp_set_cfg(CM_NTP_CFG_PORT, &port);
        cm_ntp_set_cfg(CM_NTP_CFG_TIMEOUT, &timeout);
        cm_ntp_set_cfg(CM_NTP_CFG_DNS, &dns_priority);
        cm_ntp_set_cfg(CM_NTP_CFG_SET_RTC, &set_rtc);
        cm_ntp_set_cfg(CM_NTP_CFG_CB, __cm_ntp_cb);
        cm_ntp_set_cfg(CM_NTP_CFG_CB_PARAM, "SYNCHTIME");

        if (0 != cm_ntp_sync())
        {
            cm_demo_printf("[NTP]cm_ntp_sync() fail\n");
        }
    }
    else
    {
        cm_demo_printf("[NTP] Illegal operation\n"); 
    }
}
