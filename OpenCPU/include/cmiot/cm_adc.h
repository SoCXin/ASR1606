/**
 * @file        cm_adc.h
 * @brief       ADC设备接口
 * @copyright   Copyright @2021 China Mobile IOT. All rights reserved.
 * @author      By tw
 * @date        2021/03/18
 *  
 * @defgroup adc adc
 * @ingroup PI
 * @{
 */

#ifndef __CM_ADC_H__
#define __CM_ADC_H__

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

/**
 *  @brief ADC 通道
 */
typedef enum
{
    CM_ADC_0 = 1,   /*!< ADC0 */ 
	CM_ADC_1,   /*!< ADC1(ML307A系列不支持)*/ 
	CM_ADC_2,   /*!< ADC2(ML307A系列 && ML305A系列不支持)*/ 
	CM_ADC_3,   /*!< ADC3(ML307A系列 && ML305A系列不支持)*/ 
} cm_adc_dev_e;



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
 * @brief 读取ADC电压
 *
 * @param [in]  dev ADC设备ID
 * @param [out] voltage 电压值, 单位: mv
 *
 * @return  
 *   = 0  - 成功.
 *   < 0  - 失败, 返回值为错误码.
 *
 * @details 通过ADC设备读取测量电压值
 */
int32_t cm_adc_read(cm_adc_dev_e dev, int32_t *voltage);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_ADC_H__ */


/** @}*/
