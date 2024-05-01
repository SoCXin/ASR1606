#ifndef __CM_DEMO_AUDIO_H__
#define __CM_DEMO_AUDIO_H__

#include <stdint.h>
#include <stddef.h>

void cm_test_audio_play(unsigned char **cmd,int len);
void cm_test_audio_record(unsigned char **cmd,int len);
#endif