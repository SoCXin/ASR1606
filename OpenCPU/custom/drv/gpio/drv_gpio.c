/**
 * @file gpio.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "drv_gpio.h"
#include "drv_uart.h"
#include "app.h"

#define DBG_NAME "gpio"

Call_CB MyCall_CB;

/**
 * @brief 呼叫IO回调函数
 * 
 */
static void button_irq_call_back(void)
{
    // void* msg = cm_malloc(1);
    // my_call_irq_disable();
    // ((uint8_t*)msg)[0] = 0x01; //对应通道宏定义
    // osMessageQueuePut(call_io_queue, &msg, MyCall_CB.queue_prio, 0);
    osSemaphoreRelease(button_sem);
}

int my_gpio_init(cm_gpio_num_e gpio_num, cm_iomux_pin_e gpio_pin,cm_gpio_cfg_t cfg, cm_iomux_func_e function, cm_gpio_interrupt_e irq, void *interrupt_cb)
{
    int ret = 0;

    cm_iomux_set_pin_func(gpio_pin, function);
    ret += cm_gpio_deinit(gpio_num);
    ret += cm_gpio_init(gpio_num, &cfg);
    cm_gpio_interrupt_register(gpio_num, interrupt_cb);
    cm_gpio_interrupt_enable(gpio_num, irq);

    return ret;
}

/**
 * @brief 呼叫IO初始化
 * 
 * @return int  = 0 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 */
static int button_gpio_init(void)
{
    int ret = 0;
    cm_gpio_cfg_t cfg ={
        
        .direction = CM_GPIO_DIRECTION_INPUT,
        .pull = CM_GPIO_PULL_UP
    };

    cm_iomux_set_pin_func(BUTTON_PIN, CM_IOMUX_FUNC_FUNCTION2);
    ret += cm_gpio_deinit(BUTTON_NUM);
    ret += cm_gpio_init(BUTTON_NUM, &cfg);
    cm_gpio_interrupt_register(BUTTON_NUM, button_irq_call_back);
    cm_gpio_interrupt_enable(BUTTON_NUM, CM_GPIO_IT_EDGE_RISING);

    return ret;
}

int my_io_init(void)
{
    int ret = 0;

    ret = button_gpio_init();
    if(ret != 0){
        DBG_E("CALL I/O init failed: %d\r\n",ret);
    }else{
        DBG_I("CALL init succeeded\r\n");
    }

    // MyCall_CB.queue_prio = MY_CALL_PRIO;
    return ret;
}

// int my_network_io_init(void)
// {
//     int ret = EOF;
//     cm_gpio_cfg_t cfg ={
        
//         .direction = CM_GPIO_DIRECTION_OUTPUT,
//         .pull = CM_GPIO_PULL_DOWN
//     };
    
//     cm_iomux_set_pin_func(NETWORK_PIN, CM_IOMUX_FUNC_FUNCTION2);
//     ret += cm_gpio_deinit(NETWORK);
//     ret += cm_gpio_init(NETWORK, &cfg);

//     return ret;
// }

// int my_run_io_init(void)
// {
//     int ret = EOF;
//     cm_gpio_cfg_t cfg ={
        
//         .direction = CM_GPIO_DIRECTION_OUTPUT,
//         .pull = CM_GPIO_PULL_DOWN
//     };
    
//     cm_iomux_set_pin_func(AUDIO_PIN, CM_IOMUX_FUNC_FUNCTION2);
//     ret += cm_gpio_deinit(RUNIO);
//     ret += cm_gpio_init(RUNIO, &cfg);

//     return ret;
// }

// int my_netled_status_sw(uint8_t level)
// {
//     int ret = EOF;
//     ret = cm_gpio_set_level(NETWORK, level);
//     return ret;
// }

// int my_run_status_sw(uint8_t level)
// {
//     int ret = EOF;
//     ret = cm_gpio_set_level(RUNIO, level);
//     return ret;
// }


int my_button_irq_enable(void)
{
    cm_gpio_interrupt_enable(BUTTON_NUM, CM_GPIO_IT_EDGE_RISING);
    return 0;
}

int my_button_irq_disable(void)
{
    int ret = 0;
    ret = cm_gpio_interrupt_disable(BUTTON_NUM);
    return ret;
}

cm_gpio_level_e my_call_level_get(cm_gpio_num_e gpio_num)
{
    cm_gpio_level_e level;
    cm_gpio_get_level(gpio_num, &level);
    return level;
}

int my_key_filtering(uint8_t ch_num)
{
    int ret = 0;
    osDelayMs(200);
    /*延迟后该案件依旧为高电平*/
    /*按键IO映射为通道号 +1*/
    if(my_call_level_get(ch_num+1)==1){
        ret = 1;
    }else{
        ret = 0;
    }
    return ret;
}

int my_audio_io_init(void)
{
    int ret = EOF;
    cm_gpio_cfg_t cfg ={
        
        .direction = CM_GPIO_DIRECTION_OUTPUT,
        .pull = CM_GPIO_PULL_DOWN
    };
    
    cm_iomux_set_pin_func(AUDIO_PIN, CM_IOMUX_FUNC_FUNCTION2);
    ret += cm_gpio_deinit(AUDIO_NUM);
    ret += cm_gpio_init(AUDIO_NUM, &cfg);

    return ret;
}

int my_audio_io_sw(uint8_t level)
{
    int ret = EOF;
    ret = cm_gpio_set_level(AUDIO_NUM, level);
    return ret;
}

