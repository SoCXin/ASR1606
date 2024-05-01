/*********************************************************
*  @file    cm_demo_main.h
*  @brief   ML302 OpenCPU DEMO MAIN  header
*  Copyright (c) 2021 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2021/03/18
********************************************************/
#ifndef __CM_DEMO_MAIN_H__
#define __CM_DEMO_MAIN_H__
#include "cm_os.h"
#include "string.h"

extern void cm_printf(unsigned int atHandle, const char *str, ...);
#define cm_log(fmt, args...)                      \
    do                                            \
    {                                             \
        RTI_LOG("[ cmlog ]" fmt, ##args); \
    } while(0) 


#endif