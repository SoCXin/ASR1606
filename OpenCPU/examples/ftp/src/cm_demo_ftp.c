/**
 * @file         cm_demo_ftp.c
 * @brief        FTP示例
 * @copyright    Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author       ShiMingRui
 * @date         2021/05/19
 *
 */

#include <stdio.h> 
#include "string.h"
#include "cm_ftp.h"
#include "cm_demo_ftp.h"
#include "cm_demo_uart.h"

static cm_ftp_file_data_t file_data_test;

static int32_t port = 21;
static char hostname[16] = "";
static char username[16] = "";
static char password[16] = "";
static char dir[32] = "/ml307a_test_dir";
static char home_dir[32] = "/home/uftp";
static char file[32] = "ml307a_ftp_test7.txt";
static char new_file[32] = "ml307a_ftp_test8.txt";
static char get_dir[256];
static char data[32] = "1234567890";
static char out_data[32];

/**
 *  FTP功能调试使用示例，注意使用前需先设置port，hostname，username，password
 *
 */
void cm_test_ftp(unsigned char **cmd,int len)
{
    int32_t handle;
    int32_t ret;
    int32_t find_fd1;

    cm_ftp_state_e state;

    cm_ftp_config_t config;
    config.url = (uint8_t *)hostname;
    config.port = port;
    config.username = (uint8_t *)username;
    config.passwd = (uint8_t *)password;
    config.rsptimeout = 10*1000;//10秒
    config.cid = 1;

    handle = cm_ftp_open(&config);
    if(handle < 0)
    {
        cm_demo_printf("[%s] ftp open fail\n",__func__);
    }

    ret = cm_ftp_create_dir(handle, dir);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp create dir fail\n",__func__);
    }

    ret = cm_ftp_set_current_dir(handle, dir);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp change dir fail\n",__func__);
    }

    ret = cm_ftp_get_current_dir(handle, get_dir);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp change dir fail\n",__func__);
    }
    cm_demo_printf("[%s] current_dir=[%s]\n",__func__, get_dir);

    ret = cm_ftp_set_current_dir(handle, home_dir);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp change dir fail\n",__func__);
    }
    
    ret = cm_ftp_remove_dir(handle, dir);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp remove dir fail\n",__func__);
    }

    ret = cm_ftp_put_file(handle, 0, file, (uint8_t*)data, strlen(data));
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp put 0 fail\n",__func__);
    }

    ret = cm_ftp_put_file(handle, 1, file, (uint8_t*)data, strlen(data));
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp put 1 fail\n",__func__);
    }

    ret = cm_ftp_get_file(handle, 0, file, 0, (uint8_t*)out_data, 20);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp get fail\n",__func__);
    }
    out_data[ret] = '\0';
    cm_demo_printf("[%s] len=%d data=[%s]\n",__func__, ret, out_data);

    ret = cm_ftp_rename_file(handle, file, new_file);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp rename fail\n",__func__);
    }

    ret = cm_ftp_delete_file(handle, new_file);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp del file fail\n",__func__);
    }  

    find_fd1 = cm_ftp_find_first(handle, "/", &file_data_test);
    if(find_fd1 < 0)
    {
        cm_demo_printf("[%s] ftp find first fail\n",__func__);
    }
    cm_demo_printf("[%s,%d] name=%s,size=%d,time=%d,attr=%d,permision=%d\n",__func__, __LINE__, 
         file_data_test.file_name, file_data_test.file_size, file_data_test.file_modify_time,
         file_data_test.file_attr, file_data_test.file_permision);

    do{
        find_fd1 = cm_ftp_find_next(handle, find_fd1, &file_data_test);
        cm_demo_printf("[%s,%d] name=%s,size=%d,time=%d,attr=%d,permision=%d\n",__func__, __LINE__, 
         file_data_test.file_name, file_data_test.file_size, file_data_test.file_modify_time,
         file_data_test.file_attr, file_data_test.file_permision);
    }while(find_fd1 >= 0);

    ret = cm_ftp_find_close(handle, find_fd1);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp find close fail\n",__func__);
    }

    state = cm_ftp_get_state(handle);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp get state fail\n",__func__);
    }
    cm_demo_printf("[%s] ftp state=%d\n",__func__, state);

    ret = cm_ftp_close(handle);
    if(ret < 0)
    {
        cm_demo_printf("[%s] ftp close fail\n",__func__);
    }
    return;
}
