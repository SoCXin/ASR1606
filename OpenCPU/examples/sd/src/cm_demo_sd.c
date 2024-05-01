/*********************************************************
 *  @file    cm_demo_sd.h
 *  @brief   OpenCPU SD header
 *  Copyright (c) 2022 China Mobile IOT.
 *  All rights reserved.
 *  created by WangPeng 2022/12/16
 ********************************************************/
#include "stdio.h"
#include "cm_sd.h"
#include "cm_demo_uart.h"

/**
 *  UART口SD功能调试使用示例
 *  注意：SD功能和UART0功能互斥，不可同时使用。
 *  注意：ML307A-DSLN/ML307A-GSLN模组支持SD功能。
 *  注意：若使用如下调试命令，需修改cm_demo_uart.c文件中的代码，将demo调试口由uart0修改为uart1，使用uart1作为调试口收发调试数据
 *  CM:SD:isformatted           //查询查询（SD卡）文件系统是否格式化
 *  CM:SD:format                //格式化（SD卡）文件系统
 *  CM:SD:getinfo               //获取文件系统信息
 *  CM:SD:write                 //write功能测试
 *  CM:SD:read                  //read功能测试
 *  CM:SD:seek                  //seek功能测试
 *  CM:SD:move                  //move功能测试
 *  CM:SD:delete                //delete功能测试
 */
void cm_test_sd(unsigned char **cmd,int len)
{
    unsigned char operation[20] = {0};
    sprintf((char *)operation, "%s", cmd[2]);
    int ret = 0;

    if (0 == strcmp((const char *)operation, "isformatted"))
    {
        ret = cm_sd_is_formatted("D:");
        cm_demo_printf("[SD] cm_sd_is_formatted() ret:%d\n", ret);
    }
    else if (0 == strcmp((const char *)operation, "format"))
    {
        /* 因软件驱动特性，cm_sd_format()接口应在执行SD文件操作前调用。若需要获取SD文件系统的关键数据后格式化SD卡，应在进行完SD文件系统操作并重启模组后格式化SD卡 */
        ret = cm_sd_format("D:");
        cm_demo_printf("[SD] cm_sd_format() ret:%d\n", ret);
    }
    else if (0 == strcmp((const char *)operation, "getinfo"))
    {
        cm_sd_system_info_t info = {};
        ret = cm_sd_getinfo("D:", &info);
        cm_demo_printf("[SD] cm_sd_getinfo() ret:%d total:%llu free:%llu\n", ret, info.total_size, info.free_size);
    }
    else if (0 == strcmp((const char *)operation, "write"))
    {
        ret = cm_sd_mkdir("D:\\TEST");
        cm_demo_printf("[SD] cm_sd_mkdir() ret:%d\n", ret);
        ret = cm_sd_set_currentdir("D:\\TEST");
        cm_demo_printf("[SD] cm_sd_set_currentdir() ret:%d\n", ret);

        char currentdir[256] = {};

        ret = cm_sd_get_currentdir(currentdir, 256);
        cm_demo_printf("[SD] cm_sd_get_currentdir() ret:%d %s\n", ret, currentdir);

        int32_t fd = cm_sd_fopen("D:\\TEST\\test1.c", "wc");
        cm_demo_printf("[SD] cm_sd_fopen() ret:%d\n", fd);
        ret = cm_sd_fwrite(fd, "123456780", strlen("123456780"));
        cm_demo_printf("[SD] cm_sd_write() ret:%d\n", ret);
        ret = cm_sd_fclose(fd);
        cm_demo_printf("[SD] cm_sd_fclose() ret:%d\n", ret);
    }
    else if (0 == strcmp((const char *)operation, "read"))
    {
        if (true == cm_sd_exist("D:\\TEST\\test1.c"))
        {
            cm_demo_printf("[SD] test1.c\n");
            int32_t filesize = cm_sd_filesize("D:\\TEST\\test1.c");
            cm_demo_printf("[SD] cm_sd_filesize() ret:%d\n", filesize);

            if (0 < filesize)
            {
                char buff[20] = {};
                int32_t fd = cm_sd_fopen("D:\\TEST\\test1.c", "r");
                cm_demo_printf("[SD] cm_sd_fopen() ret:%d\n", fd);
                cm_sd_fread(fd, buff, filesize);
                cm_demo_printf("[SD] cm_sd_fread() ret:%d, buff is %s\n", ret, buff);
                ret = cm_sd_fclose(fd);
                cm_demo_printf("[SD] cm_sd_fclose() ret:%d\n", ret);
            }
        }
        else if (true == cm_sd_exist("D:\\TEST\\test2.c"))
        {
            cm_demo_printf("[SD] test2.c\n");
            int32_t filesize = cm_sd_filesize("D:\\TEST\\test2.c");
            cm_demo_printf("[SD] cm_sd_filesize() ret:%d\n", filesize);

            if (0 < filesize)
            {
                char buff[20] = {};
                int32_t fd = cm_sd_fopen("D:\\TEST\\test2.c", "r");
                cm_demo_printf("[SD] cm_sd_fopen() ret:%d\n", fd);
                cm_sd_fread(fd, buff, filesize);
                cm_demo_printf("[SD] cm_sd_fread() ret:%d, buff is %s\n", ret, buff);
                ret = cm_sd_fclose(fd);
                cm_demo_printf("[SD] cm_sd_fclose() ret:%d\n", ret);
            }
        }
        else
        {
            cm_demo_printf("[SD] no file\n");
        }
    }
    else if (0 == strcmp((const char *)operation, "seek"))
    {
        if (true == cm_sd_exist("D:\\TEST\\test1.c"))
        {
            cm_demo_printf("[SD] test1.c\n");
            int32_t fd = cm_sd_fopen("D:\\TEST\\test1.c", "w");
            cm_demo_printf("[SD] cm_sd_fopen() ret:%d\n", fd);
            ret = cm_sd_fseek(fd, 1, CM_SD_SEEK_SET);
            ret = cm_sd_fwrite(fd, "ABC", strlen("ABC"));
            cm_demo_printf("[SD] cm_sd_write() ret:%d\n", ret);
            ret = cm_sd_fseek(fd, 2, CM_SD_SEEK_CUR);
            ret = cm_sd_fwrite(fd, "DE", strlen("DE"));
            cm_demo_printf("[SD] cm_sd_write() ret:%d\n", ret);
            ret = cm_sd_fseek(fd, 0, CM_SD_SEEK_END);
            ret = cm_sd_fwrite(fd, "F", strlen("F"));
            cm_demo_printf("[SD] cm_sd_write() ret:%d\n", ret);
            ret = cm_sd_fclose(fd);
            cm_demo_printf("[SD] cm_sd_fclose() ret:%d\n", ret);
        }
        else if (true == cm_sd_exist("D:\\TEST\\test2.c"))
        {
            cm_demo_printf("[SD] test2.c\n");
            
        }
        else
        {
            cm_demo_printf("[SD] no file\n");
        }
    }
    else if (0 == strcmp((const char *)operation, "move"))
    {
        ret = cm_sd_fmove("D:\\TEST\\test1.c", "D:\\TEST\\test2.c");
        cm_demo_printf("[SD] cm_sd_move() ret:%d\n", ret);
    }
    else if (0 == strcmp((const char *)operation, "delete"))
    {
        char currentdir[256] = {};

        if (true == cm_sd_exist("D:\\TEST\\test1.c"))
        {
            cm_demo_printf("[SD] test1.c\n");
            ret = cm_sd_fdelete("D:\\TEST\\test1.c");
            cm_demo_printf("[SD] cm_sd_delete() ret:%d\n", ret);
            ret = cm_sd_mkdir("D:\\other");
            cm_demo_printf("[SD] cm_sd_mkdir() ret:%d\n", ret);
            ret = cm_sd_set_currentdir("D:\\other");
            cm_demo_printf("[SD] cm_sd_set_currentdir() ret:%d\n", ret);
            ret = cm_sd_get_currentdir(currentdir, 256);
            cm_demo_printf("[SD] cm_sd_get_currentdir() ret:%d %s\n", ret, currentdir);
            ret = cm_sd_rmdir("D:\\TEST");
            cm_demo_printf("[SD] cm_sd_rmdir() ret:%d\n", ret);
        }
        else if (true == cm_sd_exist("D:\\TEST\\test2.c"))
        {
            cm_demo_printf("[SD] test2.c\n");
            ret = cm_sd_fdelete("D:\\TEST\\test2.c");
            cm_demo_printf("[SD] cm_sd_delete() ret:%d\n", ret);
            ret = cm_sd_mkdir("D:\\other");
            cm_demo_printf("[SD] cm_sd_mkdir() ret:%d\n", ret);
            ret = cm_sd_set_currentdir("D:\\other");
            cm_demo_printf("[SD] cm_sd_set_currentdir() ret:%d\n", ret);
            ret = cm_sd_get_currentdir(currentdir, 256);
            cm_demo_printf("[SD] cm_sd_get_currentdir() ret:%d %s\n", ret, currentdir);
            ret = cm_sd_rmdir("D:\\TEST");
            cm_demo_printf("[SD] cm_sd_rmdir() ret:%d\n", ret);
        }
        else
        {
            cm_demo_printf("[SD] no file\n");
        }
    }
    else
    {
        cm_demo_printf("[SD] Illegal operation\n"); 
    }
    return;
}
