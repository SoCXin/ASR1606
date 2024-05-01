/**
 * @file        cm_modem.h
 * @brief       Modem接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By ZH
 * @date        2022/11/18
 *
 * @defgroup modem modem
 * @ingroup DS
 * @{
 */

#ifndef __CM_MODEM_H__
#define __CM_MODEM_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include<stdbool.h>
#include "cm_modem_info.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Public Types
 ****************************************************************************/

/** 运营商信息 */
typedef struct
{
    uint8_t mode;         /**网络选择模式 */
    uint8_t format;       /**运营商信息格式 */
    uint8_t oper[17];     /**运营商信息 */
    uint8_t act;          /**网络接入技术 */
}cm_cops_info_t;

/** EDRX set配置 */
typedef struct
{
    uint8_t mode;       /**EDRX配置模式 */
    uint8_t act_type;   /**网络访问技术*/
    uint8_t requested_edrx_value; /**要配置的EDRX值*/
} cm_edrx_cfg_set_t;

/** EDRX get配置 */
typedef struct
{
    uint8_t act_type;   /**网络访问技术*/
    uint8_t requested_edrx_value; /**要配置的EDRX值*/
} cm_edrx_cfg_get_t;

/**PSM配置*/
typedef struct
{
    uint8_t mode;    /**模式*/
    uint8_t requested_periodic_tau; /**T3412值*/
    uint8_t requested_active_time;  /**T3324值*/
} cm_psm_cfg_t;

/**PS网络注册状态*/
typedef struct
{
    uint8_t n;            /**模式*/
    uint8_t state;        /**网络注册状态*/
    uint16_t lac;         /**位置区码*/
    uint32_t ci;          /**小区识别码*/
    uint8_t act;          /**网络访问技术*/
    uint8_t rac;          /**路由区域编码*/
    uint8_t cause_type;   /**reject_cause类型*/
    uint8_t reject_cause; /**注册失败原因*/
    uint8_t active_time;  /**T3324值*/
    uint8_t periodic_tau; /**T3412值*/
} cm_cereg_state_t;

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
 * @brief 获取产品型号
 * 
 * @param [out] cgmm 产品型号，长度64字节 
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_cgmm(char *cgmm);

/**
 * @brief 获取生产商信息
 * 
 * @param [out] cgmi 生产商信息，长度64字节 
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_cgmi(char *cgmi);

/**
 * @brief 获取运营商信息
 * 
 * @param [out] cops 运营商信息
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_cops(cm_cops_info_t *cops);

/**
 * @brief 获取模组信号强度
 *
 * @param [out] rssi 信号强度 
 * @param [out] ber  误码率
 *
 * @return  
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 *
 * @details 
 */
int32_t cm_modem_get_csq(char *rssi,char *ber);

/**
 * @brief 获取模组无线信息
 * 
 * @param [out] radio_info 无线信息
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 *  
 * @details More details
 */
int32_t cm_modem_get_radio_info(cm_radio_info_t *radio_info);

/**
 * @brief 获取模组小区信息
 * 
 * @param [out] cell_info[] 小区信息数组
 * @param [in] cell_info_num 欲获取小区信息个数（暂时只支持当前小区信息，该参数无效）
 * 
 * @return 
 *   >= 0  - 实际获取到的小区信息个数 \n
 *   < 0  - 失败, 返回值为错误码
 *  
 * @details More details
 */
int32_t cm_modem_get_cell_info(cm_cell_info_t cell_info[], uint16_t cell_info_num);

/**
 * @brief 设置模组功能模式
 * 
 * @param [in] fun 功能模式代码，参考AT指令文档AT+CFUN说明
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_set_cfun(uint16_t fun);

/**
 * @brief 获取模组功能模式
 * 
 * @return 
 *   >= 0  - 功能模式代码，参考AT指令文档AT+CFUN说明 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_cfun(void);

/**
 * @brief 设置模组EDRX
 * 
 * @param [in] cfg EDRX配置
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_set_edrx_cfg(const cm_edrx_cfg_set_t *cfg);

/**
 * @brief 获取模组EDRX设置
 * 
 * @param [out] cfg EDRX配置
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_edrx_cfg(cm_edrx_cfg_get_t *cfg);

/**
 * @brief 配置模组PSM
 * 
 * @param [in] cfg PSM配置
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_set_psm_cfg(const cm_psm_cfg_t* cfg);

/**
 * @brief 获取模组PSM配置
 * 
 * @param [out] cfg PSM配置
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_psm_cfg(cm_psm_cfg_t *cfg);

/**
 * @brief 获取PS网络注册状态
 * 
 * @param [out] cereg CEREG状态
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_cereg_state(cm_cereg_state_t *cereg);

/**
 * @brief 获取RRC连接状态
 * 
 * @return 
 *   >= 0  - RRC连接状态，参考AT文档AT+CSCON说明 \n
 *   < 0  - 失败, 返回值为错误码 
 * 
 * @details More details
 */
int32_t cm_modem_get_cscon(void);

/**
 * @brief 激活PDP
 * 
 * @param [in] cid PDP上下文编号
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码 
 * 
 * @details More details
 */
int32_t cm_modem_activate_pdp(uint16_t cid);

/**
 * @brief 关闭PDP
 * 
 * @param [in] cid PDP上下文编号
 * 
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_deactivate_pdp(uint16_t cid);

/**
 * @brief 查询PDP状态
 * 
 * @param [in] cid PDP上下文编号
 * 
 * @return 
 *   >= 0  - PDP状态，参考AT指令文档AT+CGACT说明 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_pdp_state(uint16_t cid);

/**
 * @brief 查询SIM卡状态
 * 
 * @return 
 *   >= 0  - SIM卡已识别到，参考AT指令文档AT+CPIN说明 \n
 *   < 0  - 失败\n
 * 
 * @details More details
 */
int32_t cm_modem_get_cpin(void);
/**
 * @brief 查询模块版本
 * 
 * @param [out] cgmr 版本号，长度64字节 
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 * 
 * @details More details
 */
int32_t cm_modem_get_cgmr(char *cgmr);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_MODEM_H__ */

/** @}*/
