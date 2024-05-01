/**
 * @file        cm_audio_recorder.h
 * @brief       Audio recorder接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By WangPeng
 * @date        2021/4/12
 *
 * @defgroup audio_recorder
 * @ingroup audio_recorder
 * @{
 */

#ifndef __OPENCPU_AUDIO_RECORDER_H__
#define __OPENCPU_AUDIO_RECORDER_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include "cm_audio_common.h"


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Public Types
 ****************************************************************************/

/** 录音通道支持 */
typedef enum
{
    CM_AUDIO_RECORD_CHANNEL_MAIN = 1,           /*!< 主MIC通道，不支持设置 */
    CM_AUDIO_RECORD_CHANNEL_HP,                 /*!< 耳机MIC通道，不支持设置 */
#if 0
    CM_AUDIO_RECORD_CHANNEL_REMOTE,             /*!< 从远端通话中录音（需建立通话） */
#endif
} cm_audio_record_channel_e;

/** 录音回调的事件类型 */
typedef enum
{
    CM_AUDIO_RECORD_EVENT_DATA = 1,             /*!< 录音数据上报 */
#if 0
    CM_AUDIO_RECORD_EVENT_FINISHED,             /*!< 录音结束 */
    CM_AUDIO_RECORD_EVENT_INTERRUPT,            /*!< 录音中断 */
#endif
} cm_audio_record_event_e;

/** 录音设置类型 */
typedef enum
{
    CM_AUDIO_RECORD_CFG_CHANNEL = 1,            /*!< 录音通道，支持范围参见cm_audio_record_channel_e枚举量，不支持设置 */
    CM_AUDIO_RECORD_CFG_GAIN,                   /*!< 录音增益，不支持设置 */
} cm_audio_record_cfg_type_e;

/* 
 * 录音数据结构体
 */
typedef struct
{
    uint8_t *data;                              /*!< 录音数据 */
    uint32_t len;                               /*!< 录音数据长度 */
    void *user;                                 /*!< 用户传入参数 */
} cm_audio_record_data_t; 

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
 * @brief 录音回调函数
 *
 * @param [in] event 事件类型
 * @param [in] param 事件参数（事件类型为CM_AUDIO_RECORD_DATA时，需要将param强转为cm_audio_record_data_t型。其余情况该参数为用户传入参数）
 *
 * @details  须在录音API中传入 \n
 *           通知事件为中断触发，不可阻塞，不可做耗时较长的操作，例如不可使用UART打印log
 */
typedef void (*cm_audio_record_callback_t)(cm_audio_record_event_e event, void *param);

/**
 * @brief 设置录音参数
 *
 * @param [in] type  设置参数类型
 * @param [in] value 设置参数数值
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 *  @details 增益调节范围为（0~48），呈线性关系；非录音状态下该参数设置后保持至本地缓存中不会立即生效，应用调用cm_audio_recorder_start()后方才生效，暂不支持 \n
 *                            0     -36dB \n
 *                            12    -24dB \n
 *                            24    -12dB \n
 *                            36     0dB \n
 *                            48     12dB \n
 *           不支持设置录音通道（模组仅一个录音通道，默认使用该通道，该通道外接mic/耳机mic由用户决定） \n
 *           仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS）支持audio功能
 */
int32_t cm_audio_record_set_cfg(cm_audio_record_cfg_type_e type, void *value);

/**
 * @brief 读取录音参数
 *
 * @param [in]  type  读取参数类型
 * @param [out] value 读取参数数值
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 *  @details 增益调节范围为（0~48），呈线性关系；录音状态下读取实际值，非录音状态下读取本地缓存中的值，暂不支持 \n
 *                            0     -36dB \n
 *                            12    -24dB \n
 *                            24    -12dB \n
 *                            36     0dB \n
 *                            48     12dB \n
 *           不支持获取录音通道（模组仅一个播放通道，默认使用该通道，该通道外接mic/耳机mic由用户决定） \n
 *           仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS）支持audio功能
 */
int32_t cm_audio_record_get_cfg(cm_audio_record_cfg_type_e type, void *value);

/**
 *  @brief 开始录音
 *  
 *  @param [in] format       录制音频格式
 *  @param [in] sample_param 录制音频PCM采样参数
 *  @param [in] cb           录音回调函数（回调函数不能阻塞）
 *  @param [in] cb_param     用户参数（参见cm_audio_record_callback回调函数中param参数描述，长度最大为8）
 *
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 *  @details 回调函数不能阻塞，支持PCM和AMR格式 \n
 *           通知事件为中断触发，不可阻塞，不可做耗时较长的操作，例如不可使用UART打印log \n
 *           仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS）支持audio功能
 */
int32_t cm_audio_recorder_start(cm_audio_record_format_e format, cm_audio_sample_param_t *sample_param, cm_audio_record_callback_t cb, void *cb_param);

/**
 *  @brief 结束录音
 *
 *  @details 仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS）支持audio功能
 */
void cm_audio_recorder_stop(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif

/** @}*/
