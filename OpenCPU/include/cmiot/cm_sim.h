/**
 * @file        cm_sim.h
 * @brief       SIM接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By WangPeng
 * @date        2021/10/27
 *
 * @defgroup sim sim
 * @ingroup PHONE
 * @{
 */

#ifndef __CM_SIM_H__
#define __CM_SIM_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
 
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
 
#define CM_IMSI_LEN       16 /*!< IMSI存储长度 */

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
 * @brief 获取设备IMSI
 * 
 * @param [out] imsi 存储IMSI，长度16字节，申请内存后传入
 * @return  
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码.
 *  
 * @details More details
 * 
 */
int32_t cm_sim_get_imsi(char* imsi);


#undef EXTERN
#ifdef __cplusplus
}
#endif


#endif /* __CM_SIM_H__ */

