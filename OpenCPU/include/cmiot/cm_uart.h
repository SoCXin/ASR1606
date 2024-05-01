/**
 * @file        cm_uart.h
 * @brief       UART接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By ZHANGXW
 * @date        2021/03/09
 *
 * @defgroup uart uart
 * @ingroup PI
 * @{
 */

#ifndef __CM_UART_H__
#define __CM_UART_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/** 波特率 */
#define    CM_UART_BAUDRATE_9600       (9600U)
#define    CM_UART_BAUDRATE_19200      (19200U)
#define    CM_UART_BAUDRATE_38400      (38400U)
#define    CM_UART_BAUDRATE_57600      (57600U)
#define    CM_UART_BAUDRATE_115200     (115200U)
#define    CM_UART_BAUDRATE_256000     (256000U)

/****************************************************************************
 * Public Types
 ****************************************************************************/

 /** 设备ID ，详情参照资源综述*/
typedef enum{
    CM_UART_DEV_0,              /*!< 设备0*/
    CM_UART_DEV_1,              /*!< 设备1*/
    CM_UART_DEV_2,              /*!< 设备2*/
    CM_UART_DEV_NUM
} cm_uart_dev_e;

/** 数据位 */
typedef enum{
    CM_UART_BYTE_SIZE_8 = 8,
} cm_uart_byte_size_e;

/** 奇偶校验 */
typedef enum{
    CM_UART_PARITY_NONE,
    CM_UART_PARITY_ODD,
    CM_UART_PARITY_EVEN,
    CM_UART_PARITY_MARK,    /*!< 不支持*/
    CM_UART_PARITY_SPACE    /*!< 不支持*/
} cm_uart_parity_e;

/** 停止位 */
typedef enum{
    CM_UART_STOP_BIT_ONE,
    CM_UART_STOP_BIT_ONE_HALF,
    CM_UART_STOP_BIT_TWO
} cm_uart_stop_bit_e;

/** 流控制 */
typedef enum{
    CM_UART_FLOW_CTRL_NONE,
    CM_UART_FLOW_CTRL_HW,   /*!< 当前不支持流控*/
} cm_uart_flow_ctrl_e;

/** 结果码 */
typedef enum{
    CM_UART_RET_OK = 0,
    CM_UART_RET_INVALID_PARAM = -1,
} cm_uart_ret_e;

/** 配置 */
typedef struct{
    cm_uart_byte_size_e byte_size;      /*!< 数据位，枚举*/
    cm_uart_parity_e parity;            /*!< 校验位，枚举*/
    cm_uart_stop_bit_e stop_bit;        /*!< 停止位，枚举*/
    cm_uart_flow_ctrl_e flow_ctrl;      /*!< 流控制，枚举*/
    int baudrate;                       /*!< 波特率，枚举*/
    int is_lpuart;                      /*!< 1:低功耗串口(115200以内的标准波特率)；0:普通串口*/
} cm_uart_cfg_t;

/** 事件类型 */
typedef enum
{
    CM_UART_EVENT_TYPE_RX_ARRIVED = (1 << 0),  /*!< 接收到新的数据*/
    CM_UART_EVENT_TYPE_RX_OVERFLOW = (1 << 1), /*!< 接收FIFO缓存溢出*/
    CM_UART_EVENT_TYPE_TX_COMPLETE   = (1 << 2)/*!< 不支持发送完成事件*/
}cm_uart_event_type_e;

/** 事件 */
typedef struct{
    uint32_t event_type; /*!< 事件类型，数据可读等*/
    void *event_param;               /*!< 事件参数*/
    void *event_entry;               /*!< 事件执行入口*/
} cm_uart_event_t;

typedef void (*cm_uart_event_cb_t)(void *param, uint32_t evt);

/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
***********************UART使用注意事项*************************************
* 1、UART引脚和SD卡控制引脚所用PIN脚存在复用关系，若用户使用此PIN脚进行
*    UART功能开发，请阅知cm_sd.h中【SD卡使用注意事项】
* 2、UART引脚和SD卡控制引脚复用关系见《资源综述》
***************************************************************************/


#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/**
 *  @brief 打开串口
 *  
 *  @param [in] dev 串口设备ID
 *  @param [in] cfg 串口配置
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    < 0  - 失败, 返回值为错误码
 *  
 *  @details open之前必须先对引脚复用功能进行设置。\n
 *           低功耗串口的波特率须设置为115200以内，普通串口可配置为256000非标波特率；\n
 *           受分频时钟影响，当某串口配置为256000波特率后，其他串口仅支持普通串口256000模式或低功耗串口模式
 */
int32_t cm_uart_open(cm_uart_dev_e dev, cm_uart_cfg_t *cfg);

/**
 *  @brief 注册串口事件
 *  
 *  @param [in] dev 串口设备ID
 *  @param [in] event 串口事件
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    < 0  - 失败, 返回值为错误码
 *  
 *  @details 事件包括串口数据可读/溢出等。需在open之前注册。\n
 *           回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源。
 */
int32_t cm_uart_register_event(cm_uart_dev_e dev, void *event);

/**
 *  @brief 关闭串口
 *  
 *  @param [in] dev 串口设备ID
 *  
 *  @return
 *    = 0  - 成功 \n
 *    < 0  - 失败, 返回值为错误码
 *  
 *  @details 
 */
int32_t cm_uart_close(cm_uart_dev_e dev);

/**
 *  @brief 串口写数据
 *  
 *  @param [in] dev 串口设备ID
 *  @param [in] data 待写入数据
 *  @param [in] len 长度
 *  @param [in] timeout 超时时间(ms)(无效参数)
 *  
 *  @return 
 *    = 实际写入长度 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 *  
 *  @details 
 */
int32_t cm_uart_write(cm_uart_dev_e dev, const void *data, int32_t len, int32_t timeout);

/**
 *  @brief 串口读数据
 *  
 *  @param [in] dev 串口设备ID
 *  @param [out] data 待读数据
 *  @param [in] len 长度
 *  @param [in] timeout 超时时间(ms)(无效参数)
 *  
 *  @return 
 *    = 实际读出长度 - 成功 \n
 *    < 0 - 失败, 返回值为错误码
 *  
 *  @details 
 */
int32_t cm_uart_read(cm_uart_dev_e dev, void* data, int32_t len, int32_t timeout);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_UART_H__ */

/** @}*/
