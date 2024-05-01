/**
 * @file        cm_tts_client.h
 * @brief       TTS接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By WangPeng
 * @date        2021/04/12
 *
 * @defgroup tts tts
 * @ingroup AUDIO
 * @{
 */

#ifndef __CM_TTS_CLIENT_H__
#define __CM_TTS_CLIENT_H__

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
    CM_TTS_ENCODE_TYPE_UCS2 = 0,  /*!< UCS2编码模式 */
    CM_TTS_ENCODE_TYPE_GBK,       /*!< 普通字符为ASCII格式，汉字为GBK格式 */
    CM_TTS_ENCODE_TYPE_UTF8,      /*!< 普通字符为ASCII格式，汉字为UTF-8格式 */
    CM_TTS_ENCODE_TYPE_GB2312,    /*!< 普通字符为ASCII格式，汉字为GB2312格式 */
} cm_tts_encode_e;

/**
 * @brief 数字播放方式
 */
typedef enum
{
    CM_TTS_RDN_AUTO = 0,  /*!< 自动模式 */
    CM_TTS_RDN_AS_VALUE,  /*!< 数值模式 */
    CM_TTS_RDN_AS_NUMBER, /*!< 数字模式 */
    CM_TTS_RDN_AS_STR,    /*!< 数字字符串优先 */
} cm_tts_rdn_e;

/**
 * @brief 音调
 *
 * @details 范围根据实际情况确定
 */
typedef enum
{
    CM_TTS_PITCH_0 = 0, 
    CM_TTS_PITCH_1,
    CM_TTS_PITCH_2,
    CM_TTS_PITCH_3,
    CM_TTS_PITCH_4,
    CM_TTS_PITCH_5,      /*!< 默认音调 */
    CM_TTS_PITCH_6,
    CM_TTS_PITCH_7,
    CM_TTS_PITCH_8,
    CM_TTS_PITCH_9,
    CM_TTS_PITCH_10,
    CM_TTS_PITCH_11,
    CM_TTS_PITCH_12,
    CM_TTS_PITCH_13,
    CM_TTS_PITCH_14,
    CM_TTS_PITCH_15,
} cm_tts_pitch_e;

/**
 * @brief 音效
 *
 * @details 范围根据实际情况确定
 */
typedef enum
{
    CM_TTS_VOICE_0 = 0, /*!< 默认音效,0为度小美 */
    CM_TTS_VOICE_1 = 1, /*!< 1为度小宇 */
    CM_TTS_VOICE_2 = 2, /*!< 2为度小宇 */
    CM_TTS_VOICE_3 = 3, /*!< 3为度逍遥 */
    CM_TTS_VOICE_4 = 4, /*!< 4为度丫丫 */
} cm_tts_voice_e;

/**
 * @brief 播放通道
 *
 * @details 范围根据实际情况确定
 */
typedef enum
{
    CM_TTS_CHANNEL_0 = 0, /*!< 0 - 听筒 */
    CM_TTS_CHANNEL_1 = 1, /*!< 1 - 耳机 */
    CM_TTS_CHANNEL_2 = 2, /*!< 2 - 扬声器 */
    CM_TTS_CHANNEL_3 = 3, /*!< 3 - 远端（通话过程中在对端播放） */
} cm_tts_channel_e;

/**
 * @brief TTS状态
 *
 */
typedef enum
{
    CM_TTS_STATE_IDLE = 0,  /*!< TTS空闲/播放结束（当前在线TTS版本存在缺陷，语音播放时也可能处于该状态） */
    CM_TTS_STATE_INTERRUPT, /*!< TTS播放异常中断 */
    CM_TTS_STATE_FAIL,      /*!< TTS播放失败 */
    CM_TTS_STATE_WORKING,   /*!< TTS工作中,不可使用（正在播放或其他音频功能正在使用） */
} cm_tts_state_e;

/** TTS设置类型，包括在线TTS传到云上的配置 */
typedef struct
{
    int32_t speed;          /*!< 语速，范围根据实际情况确定,范围0-15 */
    int32_t volume;         /*!< TTS转化时音量，范围根据实际情况确定,范围0-15 */
    cm_tts_encode_e encode; /*!< 中文文本编码方式,（百度在线TTS）暂仅支持(且必须)设置为2（不生效，实际使用urlencode格式编码） */
    cm_tts_rdn_e rdn;       /*!< 数字播放模式,暂仅支持(且必须)设置为0 */
    cm_tts_pitch_e pitch;   /*!< 音调,范围0-15 */
    cm_tts_voice_e voice;   /*!< 音效,范围0-4 */
    int32_t  channel;       /*!< 播放通道，暂仅支持(且必须)设置为2（不生效，本平台仅有一个音频输出通道） */
    char *url;              /*!< TTS语音转换平台网址，仅支持百度在线TTS，配置其他URL不生效 */
    char *apiKey;           /*!< client_id （即注册账号的apikey) */
    char *secretKey;        /*!< client_secret (secretKey) */
} cm_tts_cfg_t;

/** TTS回调的事件类型 */
typedef enum
{
    CM_TTS_EVENT_SYNTH_DATA = 1,  /*!< 语音合成数据上报，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_TTS_EVENT_SYNTH_FAIL,      /*!< 语音合成失败，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_TTS_EVENT_SYNTH_INTERRUPT, /*!< 语音合成（异常）中断，调用cm_tts_synth()接口时可能上报的事件，不支持 */
    CM_TTS_EVENT_SYNTH_FINISH,    /*!< 语音合成完成，调用cm_tts_text_to_pcm()接口时可能上报的事件，不支持 */
    CM_TTS_EVENT_PLAY_FAIL,       /*!< 语音播放失败，调用cm_tts_play()接口时可能上报的事件 */
    CM_TTS_EVENT_PLAY_INTERRUPT,  /*!< 语音播放中断，调用cm_tts_play()接口时可能上报的事件 */
    CM_TTS_EVENT_PLAY_FINISH,     /*!< 语音播放完成，调用cm_tts_play()接口时可能上报的事件，不支持（回调通知仍会通知该事件，但该事件的通知不能正确反应播放完成，用户不应使用该事件做判断） */
} cm_tts_event_e;


/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************/


/**
 * @brief TTS回调函数
 *
 * @param [in] event 事件类型
 * @param [in] param 事件参数（该参数为用户传入参数）
 */
typedef void (*cm_tts_callback)(cm_tts_event_e event, void *param);

/**
 * @brief TTS初始化
 *
 * @param [in] cfg  配置参数指针
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 * @details 在线TTS使用百度TTS，不支持中文文本编码方式设置，百度在线TTS仅可使用urlencode格式编码 \n
 *          cfg参数设置存在较多限制条件，请参照cm_tts_cfg_t结构体中说明进行cfg参数设置
 */
int32_t cm_tts_init(cm_tts_cfg_t *cfg);

/**
 * @brief TTS去初始化
 *
 */
void cm_tts_deinit(void);

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
 * @details 异步函数，播放任务建立后，此函数会立即返回 \n
 *          暂仅支持百度在线TTS，使用在线TTS功能需要占用一路HTTP通道 \n
 *          在线TTS使用百度TTS，text不支持中文文本编码方式设置，百度在线TTS中text仅可使用urlencode格式编码，仅支持播放不超时10秒时长的数据
 */
int32_t cm_tts_play(const void *text, int32_t len, cm_tts_callback cb, void *cb_param);

/**
 * @brief 停止TTS播放文字
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 */
int32_t cm_tts_play_stop(void);

/**
 * @brief 获取TTS状态
 *
 * @return
 *   当前TTS状态
 */
cm_tts_state_e cm_tts_get_state(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_TTS_CLIENT_H__ */

/** @}*/
