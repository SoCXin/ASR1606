/**
 * @file amr_array.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __ARM_ARRAY_H
#define __ARM_ARRAY_H

/*占线语音*/
#define AMR_BUSY_PATH "amr_busy.amr"
extern const unsigned char amr_busy[3373];
/*呼叫结束语音*/
#define AMR_CALLEND_PATH "amr_callend.amr"
extern const unsigned char amr_callend[721];
/*开始呼叫语音*/
#define AMR_CALLSTART_PATH "amr_callstart.amr"
extern const unsigned char amr_callstart[3516];
/*网络连接语音*/
#define AMR_CONNECT_PATH "amr_connect.amr"
extern const unsigned char amr_connect[1956];
/*通话超时语音*/
#define AMR_TIMEOUT_PATH "amr_timeout.amr"
extern const unsigned char amr_timeout[1904];

void my_amr_load_files(void);

#endif // !__ARM_ARRAY_H