/*********************************************************
 *  @file    cm_demo_fota.c
 *  @brief   OpenCPU FOTA示例
 *  Copyright (c) 2022 China Mobile IOT.
 *  All rights reserved.
 *  created by WangPeng 2022/8/29
 ********************************************************/

#include "stdio.h"
#include "string.h"
#include "cm_fota.h"
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_fs.h"
#include "cm_demo_uart.h"


static void __cm_fota_cb(cm_fota_error_e error)
{
    cm_demo_printf("[FOTA] error code is %d\n", error); 
}

/**
 *  UART口FOTA功能调试使用示例
 *  CM:FOTA:FULLPACKAGE_HTTP             //HTTP FOTA完整包升级方式，配置HTTP服务器信息（仅为示例，配置的url不可使用）
 *  CM:FOTA:FULLPACKAGE_FTP              //FTP FOTA完整包升级方式，配置FTP服务器信息（仅为示例，配置的url不可使用）
 *  CM:FOTA:UPGRADE                      //升级，须在完成配置后使用
 */
void cm_test_fota(unsigned char **cmd,int len)
{
    unsigned char operation[20] = {0};
    sprintf((char *)operation, "%s", cmd[2]);
    int ret = 0;

    if (0 == strcmp((const char *)operation, "FULLPACKAGE_HTTP"))          //HTTP FOTA完整包升级方式，配置FOTA信息
    {
        cm_fota_set_ota_plan(CM_FOTA_ASR_PLAN_MINI_INTEGRATE);
        cm_fota_res_callback_register((cm_fota_result_callback)__cm_fota_cb);
        ret = cm_fota_set_url("http://xxx.com:8080/system_patch.bin");          //仅作为示例，url不可使用

        if (0 != ret){
            cm_log_printf(0, "cm_fota_set_url() error");
        }
    }
    else if (0 == strcmp((const char *)operation, "FULLPACKAGE_FTP"))          //FTP FOTA完整包升级方式，配置FOTA信息
    {
        cm_fota_set_ota_plan(CM_FOTA_ASR_PLAN_MINI_INTEGRATE);
        cm_fota_res_callback_register((cm_fota_result_callback)__cm_fota_cb);
        ret = cm_fota_set_url("ftp://xxx.com:23/system_patch.bin");             //仅作为示例，url不可使用

        if (0 != ret){
            cm_log_printf(0, "cm_fota_set_url() error");
        }

        ret = cm_fota_set_auth("user", "password");

        if (0 != ret){
            cm_log_printf(0, "cm_fota_set_auth() error");
        }
    }
    else if (0 == strcmp((const char *)operation, "UPGRADE"))                 //启动升级
    {
        cm_fs_system_info_t fs_info = {0, 0};
        cm_fs_getinfo(&fs_info);

        /* 文件系统剩余空间为0时不建议执行升级，关键文件系统操作可能会失败 */
        if (0 == fs_info.free_size)
        {
            cm_log_printf(0, "insufficient space left in the file system");
            return;
        }

        cm_fota_info_t info = {0};
        cm_fota_read_config(&info);

        if (CM_FOTA_TYPE_HTTP_HTTPS == info.fixed_info.fota_mode)
        {
            cm_log_printf(0, "HTTP server [%s]", info.fixed_info.url);
        }
        else if (CM_FOTA_TYPE_FTP == info.fixed_info.fota_mode)
        {
            cm_log_printf(0, "FTP server [%s] [%s] [%s]", info.fixed_info.url, info.fixed_info.username, info.fixed_info.passwd);
        }
        else
        {
            cm_log_printf(0, "HTTP server error");
            return;
        }

        osDelay(200);

        cm_fota_exec_upgrade();
    }
    else
    {
        cm_demo_printf("[FOTA] Illegal operation\n"); 
    }
    return;
}
