/**
 * @file        cm_rtc.h
 * @brief       RTC管理
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By ljw
 * @date        2021/05/28
 *
 * @defgroup rtc rtc
 * @ingroup SYS
 * @{
 */

#ifndef __CM_RTC_H__
#define __CM_RTC_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <time.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Public Types
 ****************************************************************************/


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
 * @brief 设置系统时间（秒）
 * 
 * @param [in] second 时间戳(单位：秒)
  *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 *  
 * @details UTC时间
 */
int32_t cm_rtc_set_current_time(uint64_t second);

/**
 * @brief 获取系统时间（秒）
 *
 * @return 时间戳（单位：秒）
 *  
 * @details UTC时间
 */
uint64_t cm_rtc_get_current_time(void);

/**
 * @brief 设置时区
 * 
 * @param [in] timezone 时区
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 *
 * @details timezone设置范围为-12~12，-12为西十二区，0为世界时间，12为东十二区
 */
int32_t cm_rtc_set_timezone(int32_t timezone);

/**
 * @brief 获取时区
 *
 * @return 时区
 *  
 * @details timezone范围为-12~12，-12为西十二区，0为世界时间，12为东十二区
 */
int32_t cm_rtc_get_timezone(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_RTC_H__ */

/** @}*/
