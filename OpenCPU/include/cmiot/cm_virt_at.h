/**
 * @file        cm_virt_at.h
 * @brief       虚拟AT通道接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By XGL
 * @date        2022/07/10
 *
 * @defgroup virt_at virt_at
 * @ingroup virt_at
 * @{
 */

#ifndef __CM_VIRT_AT_H__
#define __CM_VIRT_AT_H__

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


/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/****************************************************************************
 ***********************虚拟AT需注意事项*************************************
 * 1、虚拟AT不完全支持中移开发的AT手册中的指令,仅支持3GPP的AT指令，
 *    例如：
 *    AT+MUESTATS=radio  AT+MUESTATS=cell  AT+MUESTATS=sband
 *    等AT指令不支持,用户需根据实际使用测试结果为准；其中部分功能
 *    的AT指令也有所区别，虚拟AT都遵循了使用3GPP标准的AT指令，
 *    例如：
 *    (1)获取ICCID的指令为【AT*ICCID?】(2)开启拨号上网的AT指令为
 *   【AT+MDIALUPCFG="auto",1】,关闭为【AT+MDIALUPCFG="auto",0】
 * 2、为了避免虚拟AT使用频繁导致程序阻塞，暂不支持使用多线程操作虚
 *    拟AT，同时在使用虚拟AT时每次都需要初始化和去初始化。
 * 3、虚拟AT读取数据的方式是使用消息队列一次性读取固定长度的数据，
 *    当需要读取的数据过长时，需要多次发指令进行一逐读取；例如：
 *    读取短信消息。
 * 4、使用接口cm_virt_at_sms_send发送PDU格式的短信时，需要在发送
 *    的信息编码中加入：短信中心号码（SMSC）。
 ***************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif
/**来电短信等消息主动上报调函数类型*/
typedef void (*cm_at_urc_callback)(char *urc);
/**虚拟AT通道回调函数类型*/
typedef void (*cm_virt_at_cb)(void *param);

/**
 * @brief 初始化AT虚拟通道
 * 
 * @param [in] cb AT虚拟通道消息回调函数
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败
 * 
 * @details More details
 */
int32_t cm_virt_at_init(cm_virt_at_cb cb);
/**
 * @brief 通过AT虚拟通道发送数据
 * 
 * @param [in] data 数据缓冲区指针
 * @param [in] len  要发送的数据长度
 *
 * @return 
 *   >= 0  - 实际发送的长度 \n
 *   < 0   - 失败
 * 
 * @details 暂不支持发送短信
 */
int32_t cm_virt_at_send(uint8_t * data,int32_t len);
/**
 * @brief 通过AT虚拟通道接收数据
 * 
 * @param [in] data 数据缓冲区指针
 * @param [in] len  数据缓冲区长度
 *
 * @return 
 *   >= 0  - 实际接收的长度 \n
 *   < 0   - 失败
 * 
 * @details More details
 */
int32_t cm_virt_at_get(unsigned char* data,int len);

/**
 * @brief AT虚拟通道去初始化
 *
 * @return 
 *   空
 * 
 * @details More details
 */
void cm_virt_at_deinit(void);

/**
 * @brief 初始化消息主动上报通道
 * 
 * @param [in] cb消息主动上报回调函数
 *
 * @return 
 *  空
 *   
 * 
 * @details More details
 */
void cm_virt_at_urc_reg(cm_at_urc_callback cb);

/**
 * @brief 发送短信
 * 
 * @param [in] atCmgs   AT命令AT+CGMS=后的参数
 * @param [in] msgData  需要发送的短信信息
 * @param [in] simId    选择需要发送短信的SIM卡
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败
 * 
 * @details More details
 */
int cm_virt_at_sms_send(char* atCmgs, char* msgData, int simId);

/**
 * @brief 本地短信存储
 * 
 * @param [in] smsNum   接收短信的电话号码
 * @param [in] msgData  需要发送的短信信息
 * @param [in] simId    选择需要发送短信的SIM卡
 *
 * @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败
 * 
 * @details More details
 */
int cm_virt_at_msg_memory(char* smsNum, char* msgData, int simId);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_VIRT_AT_H__ */

/** @}*/
