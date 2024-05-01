/**
 * @file         cm_demo_tts.c
 * @brief        TTS接口
 * @copyright    Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author       WangPeng
 * @date         2021/04/22
 *
 */

#include "stdio.h"
#include "string.h"
#include "cm_tts.h"
#include "cm_demo_tts.h"
#include "cm_demo_uart.h"

//#define CM_LOCAL_TTS_SUPPORT                                          //离线TTS支持，仅适用于支持TTS的SDK版本的模组型号

#ifdef CM_LOCAL_TTS_SUPPORT
#include "cm_local_tts.h"
#endif

/* 在线TTS配置 */
static char tts_url[32] = "http://aip.baidubce.com";                  //仅支持百度"http://aip.baidubce.com"
static char tts_apiKey[32] = "xxxxxxxxxxxxxxxxxxxxxxxx";              //用户配置
static char tts_secretKey[64] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";   //用户配置

/* 在线TTS回调函数 */
static void __cm_tts_callback(cm_tts_event_e event, void *param)
{
    switch(event)
    {
        case CM_TTS_EVENT_SYNTH_DATA:
        case CM_TTS_EVENT_SYNTH_FAIL:
        case CM_TTS_EVENT_SYNTH_INTERRUPT:
        case CM_TTS_EVENT_SYNTH_FINISH:
            break;
        case CM_TTS_EVENT_PLAY_FAIL:
            cm_demo_printf("[TTS] [%s] CM_TTS_EVENT_PLAY_FAIL\n", (char *)param);
            break;
        case CM_TTS_EVENT_PLAY_INTERRUPT:
            cm_demo_printf("[TTS] [%s] CM_TTS_EVENT_PLAY_INTERRUPT\n", (char *)param);
            break;
        case CM_TTS_EVENT_PLAY_FINISH:
            break;
        default:
            break;
    }
}

#ifdef CM_LOCAL_TTS_SUPPORT
/* 离线TTS回调函数 */
static void __cm_local_tts_callback(cm_local_tts_event_e event, void *param)
{
    switch(event)
    {
        case CM_LOCAL_TTS_EVENT_SYNTH_DATA:
        {
            //cm_local_tts_synth_data_t *data = (cm_local_tts_synth_data_t *)param;

            //cm_demo_printf("[TTS] [%s] CM_LOCAL_TTS_EVENT_SYNTH_DATA [%d] \n", data->user, data->len);    //打印log操作较费时
            break;
        }
        case CM_LOCAL_TTS_EVENT_SYNTH_FAIL:
        case CM_LOCAL_TTS_EVENT_SYNTH_INTERRUPT:
        case CM_LOCAL_TTS_EVENT_SYNTH_FINISH:
            break;
        case CM_LOCAL_TTS_EVENT_PLAY_FAIL:
            cm_demo_printf("[TTS] [%s] CM_LOCAL_TTS_EVENT_PLAY_FAIL\n", (char *)param);
            break;
        case CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT:
            cm_demo_printf("[TTS] [%s] CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT\n", (char *)param);
            break;
        case CM_LOCAL_TTS_EVENT_PLAY_FINISH:
            cm_demo_printf("[TTS] [%s] CM_LOCAL_TTS_EVENT_PLAY_FINISH\n", (char *)param);
            break;
        default:
            break;
    }
}
#endif

/**
 *  UART口TTS功能调试使用示例
 *  CM:TTS:INIT                                 //初始化在线TTS配置
 *  CM:TTS:PLAY                                 //播放在线TTS语音
 *  CM:TTS:LOCAL_INIT                           //初始化离线TTS配置
 *  CM:TTS:LOCAL_SYNTH                          //转码离线TTS（中文文本-PCM数据）
 *  CM:TTS:LOCAL_PLAY                           //播放离线TTS语音
 */
void cm_test_tts(unsigned char **cmd,int len)
{
    unsigned char operation[20] = {0};
    sprintf((char *)operation, "%s", cmd[2]);

    if (0 == strcmp((const char *)operation, "INIT"))           //在线TTS初始化
    {
        cm_tts_deinit();
        cm_tts_cfg_t cfg = {0};

        cfg.speed = 5;
        cfg.volume = 9;
        cfg.encode = CM_TTS_ENCODE_TYPE_UTF8;           //底层限制，仅可配置为CM_TTS_ENCODE_TYPE_UTF8，且该配置并不生效，实际使用百度在线TTS需使用urlencode格式编码
        cfg.rdn = CM_TTS_RDN_AUTO;                      //仅可配置为CM_TTS_RDN_AUTO
        cfg.pitch = CM_TTS_PITCH_5;
        cfg.voice = CM_TTS_VOICE_0;
        cfg.channel = CM_TTS_CHANNEL_2;                 //底层限制，仅可配置为CM_TTS_CHANNEL_2，且该配置并不生效，本平台仅一个音频输出通道
        cfg.url = tts_url;                              //仅支持百度在线TTS，配置其他URL不生效
        cfg.apiKey = tts_apiKey;
        cfg.secretKey = tts_secretKey;

        if (0 == cm_tts_init(&cfg))
        {
            cm_demo_printf("[TTS] init success");
        }
        else
        {
            cm_demo_printf("[TTS] init error\n");
        }
    }
    else if (0 == strcmp((const char *)operation, "PLAY"))      //在线TTS播放
    {
        /* 播放TTS，注意百度平台仅支持urlencode格式 */
        if (0 == cm_tts_play("%E7%99%BE%E5%BA%A6%E4%BD%A0%E5%A5%BD%EF%BC%8C%E4%BB%8A%E5%A4%A9%E6%98%AF%E4%B8%AA%E5%A5%BD%E6%97%A5%E5%AD%90", \
            strlen("%E7%99%BE%E5%BA%A6%E4%BD%A0%E5%A5%BD%EF%BC%8C%E4%BB%8A%E5%A4%A9%E6%98%AF%E4%B8%AA%E5%A5%BD%E6%97%A5%E5%AD%90"), __cm_tts_callback, "OpenCPU"))           //播放TTS语音
        {
            cm_demo_printf("[TTS]:cm_tts_play() success\n");
        }
        else
        {
            cm_demo_printf("[TTS]:cm_tts_play() fail\n");
        }
    }
#ifdef CM_LOCAL_TTS_SUPPORT
    else if (0 == strcmp((const char *)operation, "LOCAL_INIT"))   //离线TTS初始化
    {
        cm_local_tts_deinit();
        
        cm_local_tts_cfg_t cfg = {0};
        
        cfg.speed = 5;
        cfg.volume = 5;
        cfg.encode = CM_LOCAL_TTS_ENCODE_TYPE_UTF8;             //离线TTS仅支持UTF8格式
        cfg.digit = 0;

        if (0 == cm_local_tts_init(&cfg))
        {
            cm_demo_printf("[TTS] init:speed %d,volume %d,encode %d,digit %d\n" , cfg.speed, cfg.volume, cfg.encode, cfg.digit);
        }
        else
        {
            cm_demo_printf("[TTS] init error\n");
        }
    }
    else if (0 == strcmp((const char *)operation, "LOCAL_SYNTH"))   //离线TTS转码
    {
        cm_demo_printf("[TTS]:cm_local_tts_synth() begin\n");

        /* 转码TTS，离线TTS仅支持中文文本（不支持英文文本），且中文编码格式仅支持UTF8格式
           使用如下示例需注意转码的中文文本内容为UTF8格式（cm_demo_tts.c文件编码格式为UTF8时，才能按下述代码实现*/
        if (0 == cm_local_tts_synth("中文转码测试",  strlen("中文转码测试"), __cm_local_tts_callback, "Chinese"))
        {
            cm_demo_printf("[TTS]:cm_local_tts_synth() success\n");
        }
        else
        {
            cm_demo_printf("[TTS]:cm_local_tts_synth() fail\n");
        }
    }
    else if (0 == strcmp((const char *)operation, "LOCAL_PLAY"))   //离线TTS播放
    {
        /* 播放TTS，离线TTS仅支持中文文本播放（不支持英文文本播放），且中文编码格式仅支持UTF8格式
           使用如下示例需注意播放的中文文本内容为UTF8格式（cm_demo_tts.c文件编码格式为UTF8时，才能按下述代码实现*/
        if (0 == cm_local_tts_play("中文播放测试",  strlen("中文播放测试"), __cm_local_tts_callback, "Chinese"))           //播放TTS语音
        {
            cm_demo_printf("[TTS]:cm_local_tts_play() success\n");
        }
        else
        {
            cm_demo_printf("[TTS]:cm_local_tts_play() fail\n");
        }
    }
#endif
    else
    {
        cm_demo_printf("[TTS] Illegal operation\n"); 
    }
}

