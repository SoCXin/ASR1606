/*********************************************************
*  @file    cm_main.h
*  @brief   ML302 OpenCPU main header file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/08
********************************************************/
#ifndef __CM_MAIN_H__
#define __CM_MAIN_H__

#include "cm_os.h"
#include "cm_mem.h"
#include "sys\types.h"
#include <string.h>
#include "cm_sys.h"
#include "stdint.h"
#include "stdint-gcc.h"
#include "stdio.h"
#include "string.h"

#define FlagSET(Flag,bit)	(Flag |= (bit))		//Flag置位
#define FlagCLR(Flag,bit)	(Flag &= ~(bit))	//Flag清位
#define FlagGET(Flag,bit)	(Flag & (bit))		//Flag获取
#define ALL_SATUS   0xFFUL

/**
 * @brief 延迟函数
 * 
 * @param ms 要是5的整数倍,最小为5ms
 */
void osDelayMs(uint32_t ms);

#endif
