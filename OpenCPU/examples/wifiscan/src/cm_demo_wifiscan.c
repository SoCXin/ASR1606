#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cm_wifiscan.h"
#include "cm_demo_uart.h"

#define STR_LEN(s) (sizeof(s) - 1)
#define STR_ITEM(s) (s), STR_LEN(s)

static void __NumToHex(char *inData, int inLen, char *outData)
{
    int i=0;
    char tmpBuf[10];

	memset(tmpBuf, 0, sizeof(tmpBuf));

    if (inLen > 0)
    {
		for ( i = 0; i < inLen; i++ )
		{
		        sprintf( tmpBuf, "%02X", inData[i] );
                        strcat( outData, tmpBuf );
		}
    }
}

static void __wifiscan_callback_demo(cm_wifi_scan_info_t *param, void *user_param)
{
    cm_demo_printf("__wifiscan_callback_demo %s\n", user_param);
    char macBuf[50];
    
    for(int i = 0; i < param->bssid_number; i++)
    {
        memset(macBuf,0,sizeof(macBuf));
        __NumToHex((char*)param->channel_cell_list[i].bssid, sizeof(param->channel_cell_list[i].bssid), macBuf);
        cm_demo_printf("%d, %s, %d, %d\n", i+1, macBuf, param->channel_cell_list[i].rssi, param->channel_cell_list[i].channel_number);
    }
}

void cm_test_wifiscan(unsigned char **cmd, int len)
{
    const char *sub_cmd = (const char *)cmd[2];
    if (strncmp(sub_cmd, STR_ITEM("CFG")) == 0)
    {
        /* CM:WIFISCAN:CFG */
        uint8_t round = 3;
        uint8_t max = 10;
        uint8_t time_out = 30;
        uint8_t priority = CM_WIFI_SCAN_WIFI_HIGH;

        if(cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_ROUND, &round))
        {
            cm_demo_printf("wifiscan cfg round err\n");
        }

        if(cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_MAX_COUNT, &max))
        {
            cm_demo_printf("wifiscan cfg max err\n");
        }

        if(cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_TIMEOUT, &time_out))
        {
            cm_demo_printf("wifiscan cfg time_out err\n");
        }
        
        if(cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_PRIORITY, &priority))
        {
            cm_demo_printf("wifiscan cfg priority err\n");
        }
    }
    else if (strncmp(sub_cmd, STR_ITEM("START")) == 0)
    {
        /* CM:WIFISCAN:START */
        if(cm_wifiscan_start((cm_wifiscan_callback_t)__wifiscan_callback_demo, "wifiscan"))
        {
            cm_demo_printf("wifiscan start err\n");
        }
    }
    else if (strncmp(sub_cmd, STR_ITEM("STOP")) == 0)
    {
        /* CM:WIFISCAN:STOP */
        if(cm_wifiscan_stop())
        {
            cm_demo_printf("wifiscan stop err\n");
        }
    }
    else if (strncmp(sub_cmd, STR_ITEM("QUERY")) == 0)
    {
        /* CM:WIFISCAN:QUERY */
        cm_wifi_scan_info_t *param = NULL;
        if(cm_wifiscan_query(&param))
        {
            cm_demo_printf("wifiscan query err\n");
        }

        char macBuf[50];

        for(int i = 0; i < param->bssid_number; i++)
        {
            memset(macBuf,0,sizeof(macBuf));
            __NumToHex((char*)param->channel_cell_list[i].bssid, sizeof(param->channel_cell_list[i].bssid), macBuf);
            cm_demo_printf("%d, %s, %d, %d\n", i+1, macBuf, param->channel_cell_list[i].rssi, param->channel_cell_list[i].channel_number);
        }
    }
}
