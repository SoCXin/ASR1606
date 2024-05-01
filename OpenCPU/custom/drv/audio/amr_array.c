/**
 * @file amr_array.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "amr_array.h"
#include "cm_fs.h"
#include "drv_uart.h"
#include "stdio.h"
#include "string.h"

#define DBG_NAME "amr_array"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

static int load_amr_file(const unsigned char* amr, int length, const char* path)
{
    int ret = 0;
    int32_t fd = cm_fs_open(path, CM_FS_WB);
    ret = cm_fs_write(fd, amr, length);
    ret = cm_fs_close(fd);
    return ret;
}

void my_amr_load_files(void)
{
    int ret = 0;
    /*将要用到的音频文件载入文件系统*/
	ret += load_amr_file(amr_busy, ARRAY_SIZE(amr_busy), AMR_BUSY_PATH);
	ret += load_amr_file(amr_callend, ARRAY_SIZE(amr_callend), AMR_CALLEND_PATH);
	ret += load_amr_file(amr_callstart, ARRAY_SIZE(amr_callstart), AMR_CALLSTART_PATH);
	ret += load_amr_file(amr_connect, ARRAY_SIZE(amr_connect), AMR_CONNECT_PATH);
	ret += load_amr_file(amr_timeout, ARRAY_SIZE(amr_timeout), AMR_TIMEOUT_PATH);
    if(ret == 0)DBG_I("All amr write successed\r\n");
}