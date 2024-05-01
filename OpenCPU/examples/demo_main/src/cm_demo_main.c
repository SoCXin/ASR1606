/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "stdio.h"
#include "stdlib.h"
#include "cm_demo_main.h"
#include "cm_demo_uart.h"
#include "cm_demo_asocket.h"
#include "cm_demo_fs.h"
#include "cm_fs.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_sim.h"
#include "cm_demo_i2c.h"
#include "cm_demo_adc.h"
#include "cm_demo_spi.h"
#include "cm_demo_lcd.h"
#include "cm_demo_gpio.h"
#include "cm_demo_keypad.h"
#include "cm_demo_pwm.h"
#include "cm_demo_pm.h"
#include "cm_demo_http.h"
#include "cm_demo_mqtt.h"
#include "cm_demo_ntp.h"
#include "cm_demo_fota.h"
#include "cm_demo_audio.h"
#include "cm_virt_at.h"
#include "cm_demo_virt_at.h"
#include "cm_rtc.h"
#include "cm_demo_lbs.h"
#include "cm_demo_tts.h"
#include "cm_modem.h"
#include "cm_demo_cJSON.h"
#include "cm_demo_sd.h"
#include "cm_demo_gnss.h"
#include "cm_demo_aliyun.h"
#include "cm_demo_wifiscan.h"
#include "cm_demo_camera.h"
#include "cm_demo_ftp.h"

#define SECOND_OF_DAY (24*60*60)

typedef struct cm_tm {
    int tm_sec; /* 秒 – 取值区间为[0,59] */   
    int tm_min; /* 分 - 取值区间为[0,59] */
    int tm_hour; /* 时 - 取值区间为[0,23] */
    int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
    int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    int tm_year; /* 年份，其值等于实际年份减去1900 */
}cm_tm_t;

osThreadId_t OC_APP_TaskHandle ;
osEventFlagsId_t cmd_task_flag;

static const char * weekday[] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
static const char DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

void oc_ring_cb(unsigned char *param) 
{  
    cm_demo_printf("oc_ring_cb:%s\n",param);
}

static void cm_sec_to_date(long lSec, cm_tm_t *tTime)
{
    unsigned short i,j,iDay;
    unsigned long lDay;

    lDay = lSec / SECOND_OF_DAY;    
    lSec = lSec % SECOND_OF_DAY;

    i = 1970;
    while(lDay > 365)
    {
        if(((i%4==0)&&(i%100!=0)) || (i%400==0))
        {
            lDay -= 366;
        }
        else
        {
            lDay -= 365;
        }
        i++;
    }
    if((lDay == 365) && !(((i%4==0)&&(i%100!=0)) || (i%400==0)))
    {
       lDay -= 365;
       i++;
    }
    tTime->tm_year = i;   
    for(j=0;j<12;j++)   
    {
        if((j==1) && (((i%4==0)&&(i%100!=0)) || (i%400==0)))
        {
            iDay = 29;
        }
        else
        {
            iDay = DayOfMon[j];
        }
        if(lDay >= iDay) lDay -= iDay;
        else break;
  }
    tTime->tm_mon = j+1;
    tTime->tm_mday = lDay+1;
    tTime->tm_hour = ((lSec / 3600))%24;//这里注意，世界时间已经加上北京时间差8，
    tTime->tm_min = (lSec % 3600) / 60;
    tTime->tm_sec = (lSec % 3600) % 60;
}

static uint8_t cm_time_to_weekday(cm_tm_t *t)
{
    uint32_t u32WeekDay = 0;
    uint32_t u32Year = t->tm_year;
    uint8_t u8Month = t->tm_mon;
    uint8_t u8Day = t->tm_mday;
    if(u8Month < 3U)
    {
        /*D = { [(23 x month) / 9] + day + 4 + year + [(year - 1) / 4] - [(year - 1) / 100] + [(year - 1) / 400] } mod 7*/
        u32WeekDay = (((23U * u8Month) / 9U) + u8Day + 4U + u32Year + ((u32Year - 1U) / 4U) - ((u32Year - 1U) / 100U) + ((u32Year - 1U) / 400U)) % 7U;
    }
    else
    {
        /*D = { [(23 x month) / 9] + day + 4 + year + [year / 4] - [year / 100] + [year / 400] - 2 } mod 7*/
        u32WeekDay = (((23U * u8Month) / 9U) + u8Day + 4U + u32Year + (u32Year / 4U) - (u32Year / 100U) + (u32Year / 400U) - 2U ) % 7U;
    }

    if (0U == u32WeekDay)
    {
        u32WeekDay = 7U;
    }

    return (uint8_t)u32WeekDay;
}

cm_cmd_t cmd_vector[] = {
    {"FWRITE", cm_test_write_file},     //CM:FWRITE
    {"FREAD", cm_test_read_file},       //CM:FREAD
    {"FMOVE", cm_test_move_file},       //CM:FMOVE
    {"FDELETE", cm_test_delete_file},   //CM:FDELETE
    {"GPIO_READ", cm_test_gpio_read},//CM:GPIO_READ:NUM:iomuxpin:iomuxfun
    {"GPIO_WRITE", cm_test_gpio_write},//CM:GPIO_WRITE:NUM:iomuxpin:iomuxfun:value
    {"GPIO_IRQ", cm_test_gpio_irq},//CM:GPIO_IRQ:NUM:iomuxpin:iomuxfun
    {"GPIO_DEINIT", cm_test_gpio_deinit},//CM:GPIO_DEINIT:NUM
    {"PM_REBOOT", cm_test_pm_reboot},//CM:PM_REBOOT
    {"PM_POWEROFF", cm_test_pm_poweroff},//CM:PM_POWEROFF
    {"PM_REASON", cm_test_pm_power_on_reason},//CM:PM_REASON
    {"PM_SLEEP", cm_test_sleep_or_wakeup_enable},//CM:PM_SLEEP
    {"PM_POWERKEY", cm_test_pm_powerkey},//CM:PM_POWERKEY
    {"KEYPAD", cm_test_keypad},//CM:KEYPAD:5:0:1:2:3:4:5:0:1:2:3:4  备注：5x5的键盘
    {"PWM_OPEN", cm_test_pwm_start},//CM:PWM_OPEN:dev:period:period_h
    {"PWM_CLOSE", cm_test_pwm_close},//CM:PWM_CLOSE:dev   
//  {"LBS", cm_test_lbs_start},//CM:LBS
    {"HTTP", cm_test_http},             //CM:HTTP:operation
    {"MQTT", cm_test_mqtt},               //CM:MQTT:...
    {"ASOCKET", cm_test_asocket_cmd}, //CM:ASOCKET:...
    {"VIRT_AT", cm_virt_at_test},       //CM:VIRT_AT:AT+xxxx
    {"PIN_CMD", cm_test_pin_cmd},
    {"NTP", cm_test_ntp},               //CM:NTP:operation
    {"I2C", cm_test_i2c},               //CM:I2C
    {"ADC", cm_test_adc},               //CM:ADC
    {"SPI", cm_test_spi},               //CM:SPI
    {"LCD", cm_test_lcd},               //CM:LCD
    {"FOTA", cm_test_fota},             //CM:FOTA:operation
    {"TTS", cm_test_tts},               //CM:TTS:operation
    {"cJSON", cm_test_cJSON},           //CM:cJSON
    {"FTP", cm_test_ftp},               //CM:FTP
//    {"ALIYUN", cm_test_aliyun},         //CM:ALIYUN
//    {"WIFISCAN", cm_test_wifiscan},     //CM:WIFISCAN:operation
//    {"ALIYUNOTA", cm_aliyun_ota_test},    //CM:ALIYUNOTA
#if 0                                   //仅GSLN支持GNSS
    {"GNSS_ENABLE"       , cm_test_gnss_enable},       //CM:GNSS_ENABLE
    {"GNSS_LOCATE"       , cm_test_gnss_getlocateinfo},       //CM:GNSS_LOCATE
    {"GNSS_GETRAW"       , cm_test_gnss_getrawdata},       //CM:GNSS_GETRAW
    {"GNSS_SENDRAW"       , cm_test_gnss_sendrawdata},       //CM:GNSS_SENDRAW
    {"GNSS_CONFIG"       , cm_test_gnss_config},       //CM:GNSS_CONFIG:config_type:value
    {"AGNSS_CMD"       , cm_test_agnss_update},       //CM:AGNSS_CMD:cmd
    {"GNSS_RESET"       , cm_test_gnss_reset},       //CM:GNSS_RESET
#endif

#if 0                                   //仅8M版本（DSLN GSLN）支持audio
    {"AUDIO_PLAY", cm_test_audio_play},     //CM:AUDIO_PLAY:operation:param
    {"AUDIO_RECORD", cm_test_audio_record}, //CM:AUDIO_RECORD:operation:param
#endif
//    {"SD", cm_test_sd},                     //CM:SD:operation
#if 0                                   //仅8M版本支持
    {"CAM_PRE", cm_test_camera_preview},
    {"CAM_CAP", cm_test_camera_capture},
    {"QR_DECODE", cm_test_qr_decode}, //若要编译或使用二维码解析功能，需在third-party/third-party.mk中打开zbar编译
#endif
};

void iccid_resp_cb(void * param) 
{
    unsigned char at_data[256]={0};
    if(atoi(param) ==  0)
    {        
        cm_virt_at_get(at_data,sizeof(at_data));
        cm_demo_printf("%s\n", at_data);        
    }
    else if(atoi(param) == 1)
    {
        cm_virt_at_get(at_data,sizeof(at_data));
        cm_demo_printf("*ICCID:%s\n", at_data);  
    }
}
void cm_demo_iccid(void)
{
    if(cm_virt_at_init(iccid_resp_cb)!=0)
    {
       cm_demo_printf("cm_virt_at_init  err");   
        return;
    }
    if(cm_virt_at_send((uint8_t *)"AT*ICCID?\r\n",strlen("AT*ICCID?\r\n")) < 0)
    {
        cm_demo_printf("cm_virt_at_send err");   
        return;
    }
    osDelay(500);
    cm_virt_at_deinit();     
}
 void  my_appimg_enter(char *param)
{
    cm_demo_uart();
    cm_uart_cmd_recv_t *pUartCmdRecv_t = &gstUartCmdRecv;
    char buf[CM_VER_LEN] = {0};
    cm_tm_t t;
    int i;
    int ret;
    int pdp_time_out=0;
    cm_fs_system_info_t info = {0, 0};
    cm_heap_stats_t stats = {0};
    cm_demo_printf("\n\n\n\n\n\n\n\n\n\n");
    cm_demo_printf("CM OpenCPU Starts\n");
    cm_sys_get_cm_ver(buf, CM_VER_LEN);
    cm_demo_printf("SDK VERSION:%s\n", buf);
    cm_fs_getinfo(&info);
    cm_mem_get_heap_stats(&stats);
    cm_demo_printf("fs total:%d,remain:%d\n", info.total_size, info.free_size);
    cm_demo_printf("heap total:%d,remain:%d\n",stats.total_size,stats.free);

    cm_demo_printf("waiting for network...\n");
    while(1)
    {
        if(pdp_time_out>10)
        {
            cm_demo_printf("network timeout\n");
            break;
        }
        if(cm_modem_get_pdp_state(1) == 1)
        {
            cm_demo_printf("network ready\n");        
            break;
        }
        osDelay(200);   
        pdp_time_out++;
    }

    osDelay(200);
    cm_virt_at_urc_reg((cm_at_urc_callback)oc_ring_cb);
    cm_sec_to_date((long)(cm_rtc_get_current_time() + cm_rtc_get_timezone() * 60 * 60), &t);
    cm_demo_printf("Now:%d-%d-%d:%d:%d:%d,%s\n", t.tm_year, t.tm_mon , t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,weekday[cm_time_to_weekday(&t)-1]);

    memset(buf, 0, CM_VER_LEN);
    ret = cm_sys_get_sn(buf);
    if (ret == 0)
    {
        cm_demo_printf("SN:%s\n", buf);
    }
    else
    {
        cm_demo_printf("SN ERROR\n");
    }

    memset(buf, 0, CM_VER_LEN);
    ret = cm_sys_get_imei(buf);
    if (ret == 0)
    {
        cm_demo_printf("IMEI:%s\n", buf);
    }
    else
    {
        cm_demo_printf("IMEI ERROR\n");
    }

    memset(buf, 0, CM_VER_LEN);
    ret = cm_sim_get_imsi(buf);
    if (ret == 0)
    {
        cm_demo_printf("IMSI:%s\n", buf);
    }
    else
    {
        cm_demo_printf("IMSI ERROR\n");
    }

    /* 获取ICCID示例，注意虚拟AT通道传入的指令为"AT*ICCID?\r\n" */
    cm_demo_iccid();

	while(1)
	{
	    cm_demo_printf("\nplease input cmds:\n");
        pUartCmdRecv_t->cmd_execute = 0;
        osEventFlagsWait(cmd_task_flag, 0x00000001U, osFlagsWaitAny, osWaitForever);

        if ((pUartCmdRecv_t->len < 2))
        {
            cm_demo_printf("CMD NOT DEFINE\n");
        }
        else
        {
            for (i = 0; i < (sizeof(cmd_vector) / sizeof(cm_cmd_t)); i++)
            {
                if (NULL == pUartCmdRecv_t->buf[1])
                {
                    break;
                }
                if (strcmp((const char *)pUartCmdRecv_t->buf[1], cmd_vector[i].cmdstr) == 0)
                {
                    (*(cmd_vector[i].cmdfunc))(pUartCmdRecv_t->buf, pUartCmdRecv_t->len);
                    cm_demo_printf("OK\n");
                    break;
                }
            }
            if (i >= (sizeof(cmd_vector) / sizeof(cm_cmd_t)))
            {
                cm_demo_printf("CMD NOT DEFINE\n");
            }
        }
        for (i = 0; i < pUartCmdRecv_t->len; i++)
        {
            if (NULL != pUartCmdRecv_t->buf[i])
            {
                cm_free(pUartCmdRecv_t->buf[i]);
                pUartCmdRecv_t->buf[i] = NULL;
            }
        }
	}
   
   
}

int cm_opencpu_entry(char *param)
{
    cmd_task_flag = osEventFlagsNew(0);
    if (cmd_task_flag == 0)
    {
        cm_log_printf(0,"event flag init failed");
        return -1;
    }

	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "main_task";
	app_task_attr.stack_size = 4096 * 2;
	app_task_attr.priority = osPriorityNormal;
	
    OC_APP_TaskHandle = osThreadNew((osThreadFunc_t)my_appimg_enter,0,&app_task_attr);
	cm_log_printf(0, "task:%d\n",strlen((const char *)OC_APP_TaskHandle));
	
	
    return 0;
}

