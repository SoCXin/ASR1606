/*********************************************************
 *  @file    cm_demo_gpio.c
 *  @brief   OpenCPU GPIO示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include "stdlib.h"
#include "cm_demo_gpio.h"
#include "cm_gpio.h"
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
static int irq_count = 0;
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Public Functions
****************************************************************************/
void cm_test_gpio_read(unsigned char **cmd, int len)
{
    cm_gpio_cfg_t cfg = {0};
    cm_gpio_level_e level;
    cm_gpio_num_e test_gpio = (cm_gpio_num_e)atoi((char *)cmd[2]);
    cm_iomux_pin_e pin;
    cm_iomux_func_e fun;
    cm_iomux_func_e fun1;
    cm_gpio_direction_e dir;
    char *stopstr;

    pin = strtol((char *)cmd[3], &stopstr, 16);
    fun = strtol((char *)cmd[4], &stopstr, 16);
    
    cfg.direction = CM_GPIO_DIRECTION_INPUT;
    cfg.pull = CM_GPIO_PULL_DOWN;
    
    cm_iomux_set_pin_func(pin, fun);//初始化之前一定要先设置引脚复用
    cm_iomux_get_pin_func(pin, &fun1);
    if(fun1 !=fun)
    {
        cm_demo_printf("read gpio_%d set iomux failed\n", test_gpio);
        return;
    }
    
    cm_gpio_init(test_gpio, &cfg);
    cm_iomux_get_pin_func(pin, &fun1);
    cm_gpio_get_level(test_gpio, &level);
    cm_demo_printf("read gpio_%d level = %d\n", test_gpio, level);
    cm_gpio_get_direction(test_gpio, &dir);
    cm_demo_printf("read gpio_%d dir = %d\n", test_gpio, dir);
    cm_gpio_ioctl(test_gpio, CM_GPIO_CMD_GET_DIRECTION, &dir);
    cm_demo_printf("read gpio_%d dir = %d\n", test_gpio, dir);
    cm_gpio_ioctl(test_gpio, CM_GPIO_CMD_GET_LEVEL, &level);
    cm_demo_printf("read gpio_%d level = %d\n", test_gpio, level);
    cm_gpio_deinit(test_gpio);
}

void cm_test_gpio_write(unsigned char **cmd, int len)
{
    cm_gpio_cfg_t cfg = {0};
    cm_gpio_level_e level;
    cm_gpio_num_e test_gpio;
    cm_iomux_pin_e pin;
    cm_iomux_func_e fun;
    char *stopstr;
    
    test_gpio = atoi((char *)cmd[2]);
    pin = strtol((char *)cmd[3], &stopstr, 16);
    fun = strtol((char *)cmd[4], &stopstr, 16);
    level = atoi((char *)cmd[5]);
    
    if((level == CM_GPIO_LEVEL_HIGH) ||(level == CM_GPIO_LEVEL_LOW))
    {
        cfg.direction = CM_GPIO_DIRECTION_OUTPUT;
        cfg.pull = CM_GPIO_PULL_UP;
        
        cm_iomux_set_pin_func(pin, fun);//初始化之前一定要先设置引脚复用
        
        cm_gpio_init(test_gpio, &cfg);
        cm_gpio_set_level(test_gpio, level);
        cm_demo_printf("set gpio_%d level = %d\n", test_gpio, level);
    }
    else
    {
        cm_demo_printf("set gpio_%d level = %d,please set 0 or 1\n", test_gpio, level);
    }
}

static void cm_test_gpio_irq_callback(void)
{
    irq_count++;//不能处理打印和耗时任务
}

void cm_test_gpio_irq(unsigned char **cmd, int len)
{
    cm_gpio_cfg_t cfg = {0};
    cm_gpio_num_e test_gpio = 0;
    cm_iomux_pin_e pin;
    cm_iomux_func_e fun;
    char *stopstr;
    
    test_gpio = atoi((char *)cmd[2]);
    pin = strtol((char *)cmd[3], &stopstr, 16);
    fun = strtol((char *)cmd[4], &stopstr, 16);
    
    cfg.direction = CM_GPIO_DIRECTION_INPUT;
    cfg.pull = CM_GPIO_PULL_DOWN;
    
    cm_iomux_set_pin_func(pin, fun);//初始化之前一定要先设置引脚复用
    
    cm_gpio_init(test_gpio, &cfg);
    cm_gpio_interrupt_register(test_gpio, cm_test_gpio_irq_callback);
    cm_gpio_interrupt_enable(test_gpio, CM_GPIO_IT_EDGE_FALLING);
}


void cm_test_gpio_deinit(unsigned char **cmd, int len)
{
    cm_gpio_num_e test_gpio;
        
    test_gpio = atoi((char *)cmd[2]);
    cm_gpio_deinit(test_gpio);
}
void cm_test_pin_cmd(unsigned char **cmd, int len)
{
        
    cm_iomux_pin_e pin = atoi((char *)cmd[2]);
    cm_iomux_pincmd_e pin_cmd = atoi((char *)cmd[3]);
    uint8_t cmd_arg = atoi((char *)cmd[4]);
    
    cm_iomux_set_pin_cmd(pin,pin_cmd,cmd_arg);
}

