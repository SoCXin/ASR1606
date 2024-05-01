/**
 * @file        cm_common.h
 * @brief       opencpu 通用头文件
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By tw
 * @date        2021/03/18
 */


#ifndef __CM_COMMON_H__
#define __CM_COMMON_H__


/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cm_os.h"
#include "cm_iomux.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef FALSE
#define FALSE (0U)
#endif

#ifndef TRUE
#define TRUE (1U)
#endif

#define UART_TASK_PRIORITY osPriorityNormal

/**
 *  测试串口宏定义，只需修改此处即可变更测试串口
 *  此处请勿使用cm_uart_dev_e枚举值，否则影响本文件后续条件判断
 *  SD功能和UART0(ML302A为UART1)功能互斥，不可同时使用
 */
#if CM_OPENCPU_MODEL_ML302A
#define OPENCPU_MAIN_URAT 1
#else
#define OPENCPU_MAIN_URAT 0
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef enum{
	RET_BUSY = -2,
	RET_ERROR =-1,
	RET_SUCCESS = 0
}CM_RET_E;


/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif


#if CM_OPENCPU_MODEL_ML302A
//uart0
#if (OPENCPU_MAIN_URAT == 0)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_15, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_14, CM_IOMUX_FUNC_FUNCTION1
#endif

//uart1
#if (OPENCPU_MAIN_URAT == 1)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_30, CM_IOMUX_FUNC_FUNCTION1
#endif

//uart2
#if (OPENCPU_MAIN_URAT == 2)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_50, CM_IOMUX_FUNC_FUNCTION2
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_51, CM_IOMUX_FUNC_FUNCTION2
#endif

#define OPENCPU_UART_WEKEUP_PIN CM_IOMUX_PIN_30


#define OPENCPU_TEST_PWM0_IOMUX CM_IOMUX_PIN_3, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM1_IOMUX CM_IOMUX_PIN_12, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM2_IOMUX CM_IOMUX_PIN_69, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM3_IOMUX 0, 0
#endif

#if CM_OPENCPU_MODEL_ML305A
//uart0
#if (OPENCPU_MAIN_URAT == 0)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_32, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_31, CM_IOMUX_FUNC_FUNCTION1
#endif

//uart1
#if (OPENCPU_MAIN_URAT == 1)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_3, CM_IOMUX_FUNC_FUNCTION2
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_2, CM_IOMUX_FUNC_FUNCTION2
#endif

//uart2
#if (OPENCPU_MAIN_URAT == 2)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_49, CM_IOMUX_FUNC_FUNCTION3
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_54, CM_IOMUX_FUNC_FUNCTION3
#endif

#define OPENCPU_UART_WEKEUP_PIN CM_IOMUX_PIN_31

#define OPENCPU_TEST_PWM0_IOMUX CM_IOMUX_PIN_69, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM1_IOMUX CM_IOMUX_PIN_70, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM2_IOMUX CM_IOMUX_PIN_56, CM_IOMUX_FUNC_FUNCTION3
#define OPENCPU_TEST_PWM3_IOMUX CM_IOMUX_PIN_57, CM_IOMUX_FUNC_FUNCTION3
#endif

#if CM_OPENCPU_MODEL_ML307A
//uart0
#if (OPENCPU_MAIN_URAT == 0)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1
#endif

//uart1
#if (OPENCPU_MAIN_URAT == 1)
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1
#endif

#if (OPENCPU_MAIN_URAT == 2)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_50, CM_IOMUX_FUNC_FUNCTION3
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_51, CM_IOMUX_FUNC_FUNCTION3
#endif

#define OPENCPU_UART_WEKEUP_PIN CM_IOMUX_PIN_17

#define OPENCPU_TEST_PWM0_IOMUX CM_IOMUX_PIN_74, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM1_IOMUX CM_IOMUX_PIN_75, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM2_IOMUX CM_IOMUX_PIN_21, CM_IOMUX_FUNC_FUNCTION2
#define OPENCPU_TEST_PWM3_IOMUX 0, 0

#endif





#undef EXTERN
#ifdef __cplusplus
}
#endif

/** @}*/
/** @}*/

#endif /* __CM_COMMON_H__ */




