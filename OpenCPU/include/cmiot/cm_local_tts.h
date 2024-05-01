/**
 * @file        cm_local_tts.h
 * @brief       离线TTS接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By WangPeng
 * @date        2023/02/10
 *
 * @defgroup tts tts
 * @ingroup AUDIO
 * @{
 */


#ifndef __CM_LOCAL_TTS_H__
#define __CM_LOCAL_TTS_H__

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
 * @brief 中文文本编码方式
 *
 * @details 范围根据实际情况确定
 */
typedef enum
{
    CM_LOCAL_TTS_ENCODE_TYPE_GB2312 = 1,      /*!< 普通字符为ASCII格式，汉字为GBK格式，不支持 */
    CM_LOCAL_TTS_ENCODE_TYPE_UTF8,            /*!< 普通字符为ASCII格式，汉字为UTF-8格式 */
} cm_local_tts_encode_e;

/**
 * @brief 数字播放方式
 */
typedef enum
{
    CM_LOCAL_TTS_DIGIT_AUTO = 0,              /*!< 自动模式 */
    CM_LOCAL_TTS_DIGIT_AS_NUMBER,             /*!< 数字模式 */
    CM_LOCAL_TTS_DIGIT_AS_VALUE,              /*!< 数值模式 */
} cm_local_tts_digit_e;

/**
 * @brief 音调
 *
 * @details 范围根据实际情况确定
 */
typedef enum
{
    CM_LOCAL_TTS_TONE_NORMAL = 1,             /*!< 默认音调 */
} cm_local_tts_tone_e;

/**
 * @brief 音效
 *
 * @details 范围根据实际情况确定
 */
typedef enum
{
    CM_LOCAL_TTS_EFFECT_NORMAL = 1,           /*!< 默认音效 */
} cm_local_tts_effect_e;

/**
 * @brief TTS状态
 *
 */
typedef enum
{
    CM_LOCAL_TTS_STATE_IDLE = 1,              /*!< TTS空闲 */
    CM_LOCAL_TTS_STATE_WORKING,               /*!< TTS工作中 */
} cm_local_tts_state_e;

/** TTS参数配置设置类型 */
typedef struct
{
    int32_t speed;                      /*!< 语速，范围0~15。语速设置对英文字符无效。 */
    int32_t volume;                     /*!< TTS转化时音量，范围0~15 */
    cm_local_tts_encode_e encode;       /*!< 中文文本编码方式 */
    cm_local_tts_digit_e digit;         /*!< 数字播放模式 */
    cm_local_tts_tone_e tone;           /*!< 音调，暂不支持 */
    cm_local_tts_effect_e effect;       /*!< 音效，暂不支持 */
} cm_local_tts_cfg_t;

/** TTS回调的事件类型 */
typedef enum
{
    CM_LOCAL_TTS_EVENT_SYNTH_DATA = 1,        /*!< 语音合成数据上报，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_LOCAL_TTS_EVENT_SYNTH_FAIL,            /*!< 语音合成失败，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_LOCAL_TTS_EVENT_SYNTH_INTERRUPT,       /*!< 语音合成（异常）中断，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_LOCAL_TTS_EVENT_SYNTH_FINISH,          /*!< 语音合成完成，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_LOCAL_TTS_EVENT_PLAY_FAIL,             /*!< 语音播放失败，调用cm_tts_play()接口时可能上报的事件 */
    CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT,        /*!< 语音播放中断，调用cm_tts_play()接口时可能上报的事件 */
    CM_LOCAL_TTS_EVENT_PLAY_FINISH,           /*!< 语音播放完成，调用cm_tts_play()接口时可能上报的事件 */
} cm_local_tts_event_e;

/* 
 * TTS语音合成数据结构体
 */
typedef struct
{
    uint8_t *data; /*!< 语音合成数据（PCM） */
    uint32_t len;  /*!< 语音合成数据长度 */
    void *user;    /*!< 用户传入参数 */
} cm_local_tts_synth_data_t;

/**
 * @brief TTS转码回调函数
 *
 * @param [in] event 事件类型
 * @param [in] param 事件参数（事件类型为CM_LOCAL_TTS_EVENT_SYNTH_DATA时，需要将param强转为cm_local_tts_synth_data_t型。其余情况该参数为用户传入参数）
 */
typedef void (*cm_local_tts_callback)(cm_local_tts_event_e event, void *param);

/**
 * @brief TTS初始化
 *
 * @param [in] cfg  配置参数指针
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败 \n
 *
 * @details 仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能
 */
int32_t cm_local_tts_init(cm_local_tts_cfg_t *cfg);

/**
 * @brief TTS去初始化
 *
 * @details 仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能
 */
void cm_local_tts_deinit(void);

/**
 * @brief 文字转语音PCM数据（不播放，转码后为PCM数据，采样率8k）
 *  
 * @param [in] text     将要转码的文字
 * @param [in] len      text的字符串长度
 * @param [in] cb       TTS回调函数（回调函数不能阻塞）
 * @param [in] cb_param 用户参数（参见cm_local_tts_callback回调函数中param参数描述）
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 * @details 同步函数，转码过程中该接口阻塞。仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能 \n
 *          因程序在flash中运行速度较慢，出于内存空间和转码阻塞时间平衡性考虑，部分文字转语音PCM数据接口依赖的.o文件转至RAM中运行以提升处理速度，但该接口仍存在一定的阻塞时间 \n
 *          据统计，转码约10秒播放时间长度的文字该接口阻塞时长约1.1秒 \n
 *          如应用对TTS播放实时性要求高，建议事先执行转码操作并将转码数据保存至缓存中，待需要播放时从缓存中取数据并调用音频接口进行播放
 */
int32_t cm_local_tts_synth(const void *text, int32_t len, cm_local_tts_callback cb, void *cb_param);

/**
 * @brief TTS播放文字
 *  
 * @param [in] text     将要播放的文字
 * @param [in] len      text的字符串长度
 * @param [in] cb       TTS回调函数（回调函数不能阻塞）
 * @param [in] cb_param 用户参数（参见cm_tts_callback回调函数中param参数描述）
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 * @details 转码完成后，此函数返回。函数返回不等效于播放完成 \n
 *          仅支持中文文本播放，不支持英文文本播放 \n
 *          最多支持播放10秒的TTS数据，超出10秒的TTS数据会丢弃（代码开源，用户可自行修改） \n
 *          仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能
 */
int32_t cm_local_tts_play(const char *text, int32_t len, cm_local_tts_callback cb, void *cb_param);

/**
 * @brief 停止TTS播放文字
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败 \n
 *
 * @details 仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能
 */
int32_t cm_local_tts_play_stop(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_LOCAL_TTS_H__ */

/** @}*/
