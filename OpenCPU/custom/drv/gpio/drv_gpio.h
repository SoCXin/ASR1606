/**
 * @file gpio.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

#include "includes.h"
#include "cm_gpio.h"
#include "cm_iomux.h"

/*网络指示相关宏定义*/
#define BUTTON_NUM CM_GPIO_NUM_13
#define BUTTON_PIN CM_IOMUX_PIN_77

#define AUDIO_NUM CM_GPIO_NUM_4
#define AUDIO_PIN CM_IOMUX_PIN_51

#define NETWORK
#define NETWORK_PIN

#define RUN_NUM
#define RUN_PIN
   
/*呼叫消息队列优先级*/
#define MY_CALL_PRIO 4

typedef struct
{
    /*消息队列优先级*/
    uint8_t queue_prio;
    /*案件状态*/
    uint8_t status;
}Call_CB;

extern Call_CB MyCall_CB;

/*x消息队列*/
extern osMessageQueueId_t call_io_queue;

/**
 * @brief 设备用IO初始化
 * 
 * @return int  = 0 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 */
int my_io_init(void);
/**
 * @brief 继电器开关函数
 * 
 * @param gpio_num 继电器名
 * @param level 0:关 1:开
 * @return int = 0 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 */
int my_relay_sw(cm_gpio_num_e gpio_num, cm_gpio_level_e level);

/**
 * @brief 关闭全部呼叫按钮中断
 * 
 * @return int = 0 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 * 
 * @note 关闭中断后在按键消抖处理后一定要打开
 */
int my_button_irq_disable(void);

/**
 * @brief 开启全部呼叫按钮中断
 * 
 * @return int = 0 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 */
int my_button_irq_enable(void);

/**
 * @brief 呼叫按键电平检查
 * 
 * @param gpio_num CALLx
 * @return cm_gpio_level_e = 0 - 低电平 \n
 *    = 1 - 高电平
 */
cm_gpio_level_e my_call_level_get(cm_gpio_num_e gpio_num);

/**
 * @brief 按键消抖函数
 * 
 * @param key_num CALLx
 * @return int 消抖后电平 = 0 - 低电平 \n
 *    = 1 - 高电平
 */
int my_key_filtering(uint8_t key_num);

int my_gpio_init(cm_gpio_num_e gpio_num, cm_iomux_pin_e gpio_pin,cm_gpio_cfg_t cfg, cm_iomux_func_e function, cm_gpio_interrupt_e irq, void *interrupt_cb);

int my_audio_io_init(void);

int my_audio_io_sw(uint8_t level);

#endif // !__DRV_GPIO_H