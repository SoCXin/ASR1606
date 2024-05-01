#include "string.h"
#include "cm_local_tts.h"
#include "cm_os.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_audio_player.h"
#include "cm_audio_common.h"


#define CM_TTS_FRAME_BUFFER_SIZE (320 * 50 * 10)                    //支持存放10秒数据，可修改

typedef struct {
    osThreadId_t TaskHandle;                        /* TTS播放管理句柄 */
    osSemaphoreId_t SemHandle;                      /* TTS播放信号量句柄 */
    cm_local_tts_state_e state;                     /* TTS播放状态 */
    uint8_t *TtsPcmBuf;                             /* TTS转码后的数据 */
    uint32_t TtsPcmBufLen;                          /* 当前TTS已完成转码的数据长度 */
    cm_local_tts_callback user_cb;                  /* 回调函数，由上层应用设置 */
    void* user_cb_param;                            /* 回调函数中的参数，由上层应用设置 */
} cm_tts_play_cfg_t;

static cm_tts_play_cfg_t cm_tts_play_cfg = {NULL, NULL, CM_LOCAL_TTS_STATE_IDLE, NULL, 0, NULL, NULL};

static void __cm_tts_play_callback(cm_local_tts_event_e event, void *param)
{
    switch(event)
    {
        case CM_LOCAL_TTS_EVENT_SYNTH_DATA:
        {
            cm_local_tts_synth_data_t *synth_data = (cm_local_tts_synth_data_t *)param;

            if (CM_TTS_FRAME_BUFFER_SIZE > (cm_tts_play_cfg.TtsPcmBufLen + synth_data->len))
            {
                memcpy (cm_tts_play_cfg.TtsPcmBuf + cm_tts_play_cfg.TtsPcmBufLen, (uint8_t *)synth_data->data, synth_data->len);
                cm_tts_play_cfg.TtsPcmBufLen = cm_tts_play_cfg.TtsPcmBufLen + synth_data->len;
            }

            if (cm_tts_play_cfg.user_cb)
            {
                cm_tts_play_cfg.user_cb(event, param);
            }
            //cm_log_printf(0, "[TTS] [%s] CM_LOCAL_TTS_EVENT_SYNTH_DATA [%d] \n", synth_data->user, synth_data->len);
            break;
        }
        case CM_LOCAL_TTS_EVENT_SYNTH_FAIL:
        case CM_LOCAL_TTS_EVENT_SYNTH_INTERRUPT:
        case CM_LOCAL_TTS_EVENT_SYNTH_FINISH:
        case CM_LOCAL_TTS_EVENT_PLAY_FAIL:
        case CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT:
        case CM_LOCAL_TTS_EVENT_PLAY_FINISH:
        default:
            break;
    }
}

static void __cm_tts_play_task(void *param)
{
    while (1)
    {
        if (cm_tts_play_cfg.SemHandle != NULL)
        {
            osSemaphoreAcquire(cm_tts_play_cfg.SemHandle, osWaitForever);
        }

        int32_t index = 0;
        bool isError = false;               //播放过程中是否有错误

        /* 转码速度大于播放速度，故无需考虑播放速度大于转码速度情况的延迟等待问题 */
        while (index < cm_tts_play_cfg.TtsPcmBufLen && CM_LOCAL_TTS_STATE_WORKING == cm_tts_play_cfg.state)
        {
            //cm_log_printf(0, "%s() __%d__ Length is %d, index is %d", __func__, __LINE__, cm_tts_play_cfg.TtsPcmBufLen, index);

            /* 每200ms传10帧PCM数据 */
            if (index + 3200 <= cm_tts_play_cfg.TtsPcmBufLen)
            {
                if (0 != cm_audio_player_stream_push(cm_tts_play_cfg.TtsPcmBuf + index, 3200))
                {
                    isError = true;
                }

                index += 3200;
            }
            else
            {
                if (index >= cm_tts_play_cfg.TtsPcmBufLen)     //播放完成
                {
                    break;
                }
                else
                {
                    if (0 != cm_audio_player_stream_push(cm_tts_play_cfg.TtsPcmBuf + index, (cm_tts_play_cfg.TtsPcmBufLen - index)))
                    {
                        isError = true;
                    }

                    index = cm_tts_play_cfg.TtsPcmBufLen;
                }
            }

            osDelay(40);
        }

        cm_audio_player_stream_close();
        cm_tts_play_cfg.state = CM_LOCAL_TTS_STATE_IDLE;
        cm_tts_play_cfg.TtsPcmBufLen = 0;

        if (cm_tts_play_cfg.user_cb)
        {
            if (true == isError)
            {
                cm_tts_play_cfg.user_cb(CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT, cm_tts_play_cfg.user_cb_param);
            }
            else
            {
                cm_tts_play_cfg.user_cb(CM_LOCAL_TTS_EVENT_PLAY_FINISH, cm_tts_play_cfg.user_cb_param);
            }
        }

        cm_free(cm_tts_play_cfg.TtsPcmBuf);
        cm_tts_play_cfg.TtsPcmBuf = NULL;

        //cm_heap_stats_t stats = {0};
        //cm_mem_get_heap_stats(&stats);
        //cm_log_printf(0, "heap total:%d,remain:%d\n",stats.total_size,stats.free);
    }
}

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
 *          最多支持播放14秒的TTS数据，超出10秒的TTS数据会丢弃（代码开源，用户可自行修改） \n
 *          仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能
 */
int32_t cm_local_tts_play(const char *text, int32_t len, cm_local_tts_callback cb, void *cb_param)
{
    //需要判断当前是否处于播放过程中，处于播放过程中报错
    if (CM_LOCAL_TTS_STATE_WORKING == cm_tts_play_cfg.state)
    {
        cm_log_printf(0, "%s() __%d__ TTS busy", __func__, __LINE__);
        return -1;
    }

    if (NULL == cm_tts_play_cfg.TtsPcmBuf)
    {
        cm_tts_play_cfg.TtsPcmBuf = cm_malloc(CM_TTS_FRAME_BUFFER_SIZE);

        if (NULL == cm_tts_play_cfg.TtsPcmBuf)
        {
            cm_log_printf(0, "%s() __%d__ cm_malloc() error", __func__, __LINE__);
            return -1;
        }
    }

    cm_audio_sample_param_t frame = {.sample_format = CM_AUDIO_SAMPLE_FORMAT_16BIT, .rate = CM_AUDIO_SAMPLE_RATE_8000HZ, .num_channels = CM_AUDIO_SOUND_MONO};
    int32_t ret = cm_audio_player_stream_open(CM_AUDIO_PLAY_FORMAT_PCM, &frame);              //从pipe中播放音频（开启）

    if (-1 == ret)
    {
        cm_log_printf(0, "%s() __%d__ cm_audio_player_stream_open() error, ret is %d", __func__, __LINE__, ret);
        return -1;
    }

    /* 创建线程资源 */
    if (NULL == cm_tts_play_cfg.TaskHandle)
    {
        osThreadAttr_t tts_task_attr = {0};
        tts_task_attr.name = "tts_play_task";
        tts_task_attr.stack_size = 2048;
        tts_task_attr.priority= osPriorityNormal;
    
        cm_tts_play_cfg.TaskHandle = osThreadNew(__cm_tts_play_task, 0, &tts_task_attr);

        if (NULL == cm_tts_play_cfg.TaskHandle)
        {
            cm_log_printf(0, "%s() __%d__ osThreadNew() error", __func__, __LINE__);
            cm_audio_player_stream_close();
            return -1;
        }
    }

    if (NULL == cm_tts_play_cfg.SemHandle)
    {
        cm_tts_play_cfg.SemHandle = osSemaphoreNew(1, 0, NULL);

        if (NULL == cm_tts_play_cfg.SemHandle)
        {
            cm_log_printf(0, "%s() __%d__ osSemaphoreNew() error", __func__, __LINE__);
            cm_audio_player_stream_close();
            return -1;
        }
    }

    cm_tts_play_cfg.user_cb = cb;
    cm_tts_play_cfg.user_cb_param = cb_param;
    cm_tts_play_cfg.TtsPcmBufLen = 0;
    cm_tts_play_cfg.state = CM_LOCAL_TTS_STATE_WORKING;

    osSemaphoreRelease(cm_tts_play_cfg.SemHandle);

    ret = cm_local_tts_synth(text, len, __cm_tts_play_callback, cb_param);

    return ret;
}

/**
 * @brief 停止TTS播放文字
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败 \n
 *
 * @details 仅8M版本（ML307A-DSLN ML307A-GSLN ML305A-DS等）支持离线TTS功能
 */
int32_t cm_local_tts_play_stop(void)
{
    if (CM_LOCAL_TTS_STATE_IDLE == cm_tts_play_cfg.state)
    {
        cm_log_printf(0, "%s() __%d__ no TTS task", __func__, __LINE__);
        return -1;
    }

    cm_tts_play_cfg.state = CM_LOCAL_TTS_STATE_IDLE;;
    return 0;
}
