/*********************************************************
 *  @file    cm_demo_pwm.c
 *  @brief   OpenCPU PWM示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdlib.h>
#include "cm_demo_pwm.h"
#include "cm_pwm.h"
#include "cm_common.h"
#include "cm_iomux.h"
#include "cm_demo_uart.h"
#include "cm_os.h"
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
void cm_test_pwm_start(unsigned char **cmd,int len)
{
    uint32_t period,period_h;
    uint8_t dev = CM_PWM_DEV_2;
    
    cm_demo_printf("pwm test start... ...\n");

    //使用PWM0测试
    dev = atoi((char *)cmd[2]);
    period = atoi((char *)cmd[3]);
    period_h = atoi((char *)cmd[4]);
    
    cm_demo_printf("pwm=%d period=%d,period_h =%d\n", dev,period,period_h);
    
    if(dev==0)
    {
        cm_iomux_set_pin_func(OPENCPU_TEST_PWM0_IOMUX);
    }
    else if(dev==1)
    {
        cm_iomux_set_pin_func(OPENCPU_TEST_PWM1_IOMUX);
    }
    else if(dev==2)
    {
        cm_iomux_set_pin_func(OPENCPU_TEST_PWM2_IOMUX);
    }
    else if(dev==3)
    {
        cm_iomux_set_pin_func(OPENCPU_TEST_PWM3_IOMUX);
    }
    
    if(0 == cm_pwm_open_ns(dev, period,period_h))
    {
        ;
    }
    else
    {
        cm_demo_printf("pwm%d open error\n", dev);
    }

}

void cm_test_pwm_close(unsigned char **cmd,int len)
{
    uint8_t dev = CM_PWM_DEV_2;
    
    cm_demo_printf("pwm test close... ...\n");

    //使用PWM0测试
    dev = atoi((char *)cmd[2]);
    
    if(cm_pwm_close(dev) == 0)
    {
        cm_demo_printf("[PWM]Stop Success\n");
    }
    else
    {
        cm_demo_printf("[PWM]Stop Fail\n");
    }
}


