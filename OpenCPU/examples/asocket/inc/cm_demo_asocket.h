/*********************************************************
*  @file    cm_demo_asocket.h
*  @brief   ML302 OpenCPU asocket header file
*  Copyright (c) 2021 China Mobile IOT.
*  All rights reserved.
*  created by wangk 2021/08/10
********************************************************/
#ifndef __CM_DEMO_ASOCKET_H
#define __CM_DEMO_ASOCKET_H

void cm_test_asocket_init(void);

void cm_test_asocket_cmd(unsigned char **cmd, int len);

#endif // __CM_DEMO_ASOCKET_H