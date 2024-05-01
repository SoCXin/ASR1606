/*********************************************************
*  @file    cm_demo_pm.h
*  @brief   ML302 OpenCPU power manage header file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by CaoChi 2020/12/10
********************************************************/
#ifndef __CM_DEMO_PM_H__
#define __CM_DEMO_PM_H__

void cm_test_pm_reboot(unsigned char **cmd,int len);
void cm_test_pm_poweroff(unsigned char **cmd,int len);

void cm_test_pm_power_on_reason(unsigned char **cmd,int len);
void cm_test_sleep_or_wakeup_enable(unsigned char **cmd,int len);
void cm_test_pm_powerkey(unsigned char **cmd,int len);

#endif
