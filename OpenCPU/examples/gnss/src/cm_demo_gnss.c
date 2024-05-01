/*********************************************************
 *  @file    cm_demo_gnss.c
 *  @brief   OpenCPU GNSS示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include "stdlib.h"
#include "cm_demo_gnss.h"
#include "cm_gnss.h"
#include "cm_common.h"
#include "cm_mem.h"
#include "cm_demo_uart.h"
#include "cm_os.h"
#include "cm_sys.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Data
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Public Functions
****************************************************************************/
void cm_test_gnss_enable(unsigned char **cmd,int len)
{
    uint32_t nema_mask = 0xFFFF;
    int enable_param = 0;
    int gnss_type = 0;
    
    enable_param = atoi((char *)cmd[2]);
    gnss_type = atoi((char *)cmd[3]);
    
    if(enable_param == 1)
    {
        cm_gnss_config(CM_GNSS_CONFIG_TYPE_NMEA_MASK, &nema_mask);
        cm_gnss_open(gnss_type, CM_AGNSS_ENABLE);
    }
    else
    {
        cm_gnss_close();
    }
    
    return;
}


void cm_test_gnss_getlocateinfo(unsigned char **cmd,int len)
{
    cm_gnss_location_info_t *location_info = cm_malloc(sizeof(cm_gnss_location_info_t));
    
    memset(location_info,0,sizeof(cm_gnss_location_info_t));
    
    if(0 == cm_gnss_get_location_info(location_info))
    {
        cm_demo_printf("latitude=%lf\n",location_info->latitude);
        cm_demo_printf("longitude=%lf\n",location_info->longitude);
        cm_demo_printf("hdop=%lf\n",location_info->hdop);
        cm_demo_printf("altitude=%lf\n",location_info->altitude);
        cm_demo_printf("fix=%d\n",location_info->fix);
        cm_demo_printf("cog=%lf\n",location_info->cog);
        cm_demo_printf("spkm=%lf\n",location_info->spkm);
        cm_demo_printf("spkn=%lf\n",location_info->spkn);
        cm_demo_printf("nsat=%d\n",location_info->nsat);
    }
    else
    {
        cm_demo_printf("GNSS NO SINGAL\n");
    }
    cm_free(location_info);
}

void cm_gnss_nmea_cb(uint32_t type, const char *nmea, uint32_t len)
{
    cm_demo_printf("cm_test_gnss_getrawdata \n type = 0x%08x",type);
    cm_demo_printf("cm_test_gnss_getrawdata:%s,%d",nmea, len);
}

void cm_test_gnss_getrawdata(unsigned char **cmd,int len)
{
    uint32_t nmea_type = CM_GNSS_NAME_GGA|CM_GNSS_NAME_GSA|CM_GNSS_NAME_RMC|CM_GNSS_NAME_VTG|CM_GNSS_NAME_VTG|CM_GNSS_NAME_GLL|CM_GNSS_NAME_GSV;

    cm_demo_printf("nmea_type:0x%04x\n",nmea_type);
    cm_gnss_set_nmea_callback(cm_gnss_nmea_cb, nmea_type);
    cm_gnss_req_nmea();
}

void cm_gnss_sendrawdata_cb(const char *data, uint32_t len)
{
    cm_demo_printf("cm_gnss_sendrawdata_cb:%s,%d",data, len);
}

void cm_test_gnss_sendrawdata(unsigned char **cmd,int len)
{
    char senddata[64]= {0};
    uint8_t senddata_len = 0;

    senddata_len = strlen((char *)cmd[2]);
    memcpy(senddata, (char *)cmd[2], senddata_len);
    cm_demo_printf("cm_gnss_send_raw_data=%s\r\n",senddata);
    cm_gnss_send_raw_data(senddata, senddata_len, cm_gnss_sendrawdata_cb);
}


void cm_test_gnss_config(unsigned char **cmd,int len)
{
    int config_type = 0;
    int value = 0;

    config_type = atoi((char *)cmd[2]);
    value = atoi((char *)cmd[3]);
    cm_demo_printf("cm_gnss_config ret=:%d\r\n",cm_gnss_config(config_type, &value));
}


void cm_agnss_update_callback(cm_agnss_update_result_e mode, const char *update_time, uint32_t size)
{
    cm_demo_printf("mode=%d\r\n",mode);
    cm_demo_printf("update_time=%s\r\n",update_time);
}

void cm_test_agnss_update(unsigned char **cmd,int len)
{
    int agnss_cmd = 0;
    
    agnss_cmd = atoi((char *)cmd[2]);

    if(agnss_cmd == 1)
    {
        cm_agnss_data_start_update(cm_agnss_update_callback);
    }
    else if(agnss_cmd == 2)
    {
        cm_agnss_update_result_e agnss_mode = 0;
        char update_time[24] = {0};
        uint32_t data_size = 0;
        cm_agnss_state_query(&agnss_mode, update_time, sizeof(update_time),&data_size);
        cm_demo_printf("agnss_mode=%d\r\n",agnss_mode);
        cm_demo_printf("update_time=%s\r\n",update_time);
    }
}

void cm_test_gnss_reset(unsigned char **cmd,int len)
{
    cm_gnss_reset();
}

