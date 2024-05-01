/**
 * @file        cm_pm.h
 * @brief       PM接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By zyf
 * @date        2021/04/18
 *
 * @defgroup pm pm
 * @ingroup PM
 * @{
 */

#ifndef __CM_PM_H__
#define __CM_PM_H__


/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef void (*cm_pm_event_cb_t)(void);
typedef void (*cm_pm_powerkey_cb_t)(void);

/** 上电原因 */
typedef enum
{
    CM_PM_UNKNOWN = 0,       /*!<未知原因*/
    CM_PM_CHARG_POW_OFF,     /*!<Charging in powering off*/
    CM_PM_RD_PRO_MODE     ,  /*!<Rd production mode*/
    CM_PM_RTC_ALARM,         /*!<rtc Alarm*/
    CM_PM_POWER_ON,          /*!<Normal power on*/
    CM_PM_ERROR_RESET,       /*!<Error reset (WDT)*/
}cm_pm_power_on_reason_e;

/** 配置 */
typedef struct{
    cm_pm_event_cb_t cb_enter;  /*!< 进入低功耗回调函数*/
    cm_pm_event_cb_t cb_exit;   /*!< 退出低功耗回调函数*/
} cm_pm_cfg_t;
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


/**
 * @brief 关机
 *  
 * @return 空
 *  
 * @details More details
 */
void cm_pm_poweroff(void);
/**
 * @brief 获取上电原因
 * 
 * @return 
 *   >= 0  - 上电原因\n
 *   < 0  - 失败, 返回值为错误码
 *  
 * @details More details
 */
int cm_pm_get_power_on_reason(void);


/**
 * @brief 重启
 * 
 * @param [in] void
 * 
 * @return void
 *  
 * @details More details
 */
void cm_pm_reboot(void);

/**
 * @brief pm初始化
 * 
 * @param [in] pm_cfg:参数配置
 * 
 * @return void
 *  
 * @details More details
 */
void cm_pm_init(cm_pm_cfg_t pm_cfg);


/**
 * @brief 睡眠模式上锁
 *  
 * @return 空
 *  
 * @details 执行后，模组将无法进入休眠状态
 */
void cm_pm_work_lock(void);

/**
 * @brief 睡眠模式解锁
 *  
 * @return 空
 *  
 * @details 执行后，模组将允许进入休眠状态
 */

void cm_pm_work_unlock(void);

/**
 * @brief powerkey按键回调函数注册
 *  
 * @param [in]  callback回调函数指针
 * @return
 *  = 0  - 成功 \n
 *  < 0  - 失败, 返回值为错误码
 *  
 * @details 长按(3s)无回调功能开关机；短按回调此注册函数\n
 */
int32_t cm_pm_powerkey_regist_callback(cm_pm_powerkey_cb_t callback);


#undef EXTERN
#ifdef __cplusplus
}
#endif


#endif /* __CM_PM_H__ */

/** @}*/
