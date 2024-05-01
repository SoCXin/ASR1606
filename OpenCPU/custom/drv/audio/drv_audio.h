#pragma once

#include "includes.h"
#include "cm_audio_player.h"
#include "cm_audio_recorder.h"
#include "sys\types.h"
#include "amr_array.h"

/* 语音 */
/*
 * 1.正在连接网络,请稍后
 * 2.通话已结束
 * 3.呼叫已取消
 * 4.
 * 5.
 * 6.
 * 7.
 * 8.
 * 9.
 * 10.
 * 11.
 * 
 * 
 */

/**
 * @brief 音频初始化
 * 
 * @param volume 音量
 * @return uint32_t 
 */
uint32_t my_audio_init(uint8_t volume);

/**
 * @brief amr铃声播放函数
 * 
 * @param path 铃声地址
 * @return int32_t 
 */
int my_ringtone_play(const char *path);

/**
 * @brief 打开音频流传输
 * 
 * @return int =  0 - 成功 \n
 *   = -1 - 失败
 */
int my_open_audio_stream(void);

/**
 * @brief 关闭音频流传输
 * 
 */
void my_close_audio_stream(void);

/**
 * @brief 推送音频流函数
 * 
 * @param data 音频数据
 * @param size 数据大小
 * @return int =  0 - 成功 \n
 *   = -1 - 失败
 * 
 * @note 对于PCM格式，传输数据须是帧长度的整数倍（例如，8k 16bit的PCM数据，每帧为320字节，传输的数据应是320的整数倍 \n
 * 对于AMR格式，传输数据若包含不完整帧数据可能出现爆破音
 */
int my_push_audio_stream(uint8_t *data, uint32_t size);

/**
 * @brief 开始录音
 * 
 * @return int =  0 - 成功 \n
 *   = -1 - 失败
 */
int my_record_start(void);

/**
 * @brief 结束录音
 * 
 */
void my_record_stop(void);

void my_audio_play_stop(void);