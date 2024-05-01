/**
 * @file        cm_ssl.h
 * @brief       SSL通用API接口
 * @copyright   Copyright ? 2021 China Mobile IOT. All rights reserved.
 * @author      By YeXiangji
 * @date        2021/8/5
 *
 * @defgroup ssl SSL
 * @ingroup SSL
 * @{
 */

#ifndef _CM_API_SSL_H_
#define _CM_API_SSL_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/** SSL配置类型 */
typedef enum
{
    CM_SSL_PARAM_VERIFY,           /**< 设置验证方式, 参数(value): 类型uint8_t, 值为验证方式类型，0无身份验证，1单向验证， 2双向验证. */
	CM_SSL_PARAM_NEGOTIME,         /**< 设置超时时间, 参数(value): 类型uint16_t, 值为超时时间 */
    CM_SSL_PARAM_CA_CERT,          /**< 设置CA证书, 参数(value): 类型char*, 值为CA证书字符串指针. */
    CM_SSL_PARAM_CLI_CERT,         /**< 设置客户端证书, 参数(value): 类型char*, 值为客户端证书字符串指针. */
    CM_SSL_PARAM_CLI_KEY,          /**< 设置客户端密钥, 参数(value): 类型char*, 值为客户端密钥. */
    CM_SSL_PARAM_CA_CERT_DELETE,   /**< 删除CA证书, 参数(value): 无意义. */
    CM_SSL_PARAM_CLI_CERT_DELETE,  /**< 删除客户端证书, 参数(value): 无意义. */
    CM_SSL_PARAM_CLI_KEY_DELETE,   /**< 删除客户端密钥, 参数(value): 无意义. */
} cm_ssl_param_type_e;

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
 * @brief 配置SSL参数
 *
 * @param [in] ssl_id   	ssl通道号，范围为0-5
 * @param [in] type 		SSL配置项类型
 * @param [in] value 		SSL配置项参数
 *
 * @return 
 *   = 0  - 成功 \n
 *   = -1  - 参数错误
 *
 * @details 设置SSL配置参数，本接口配置证书时会将证书写入文件系统，故使用时需确保文件系统有足够空间
 */
int32_t cm_ssl_setopt(int32_t ssl_id,cm_ssl_param_type_e type,void * value);

/**
 * @brief SSL连接
 *
 * @param [out] cm_ssl_ctx_p  ssl上下文
 * @param [in] ssl_id   	  ssl通道号，范围为0-5
 * @param [in] socket 		  socket描述符
 * @param [in] timeout 		  读取超时时间，单位ms
 *
 * @return 
 *   = 0  - 成功 \n
 *  != 0  - 失败
 *
 * @details 
 */
int cm_ssl_conn(void **cm_ssl_ctx_p, int ssl_id, int socket, int timeout);

/**
 * @brief SSL关闭
 *
 * @param [in] cm_ssl_ctx_p  ssl上下文
 *
 * @details 
 */
void cm_ssl_close(void **cm_ssl_ctx_p);

/**
 * @brief SSL写
 *
 * @param [in] cm_ssl_ctx     ssl上下文
 * @param [in] data 		  待写数据
 * @param [in] data_len 	  数据长度
 *
 * @return 
 *   >= 0 - 发送长度 \n
 *   < 0  - 错误码
 *
 * @details 
 */
int cm_ssl_write(void *cm_ssl_ctx, void *data, int data_len);

/**
 * @brief SSL读
 *
 * @param [in] cm_ssl_ctx     ssl上下文
 * @param [in] data 		  读数据缓存区
 * @param [in] data_len 	  缓存区长度
 *
 * @return 
 *   >= 0 - 读取长度 \n
 *   < 0  - 错误码
 *
 * @details 
 */
int cm_ssl_read(void *cm_ssl_ctx, void *data, int data_len);

/**
 * @brief 缓存区是否有未读数据
 *
 * @param [in] cm_ssl_ctx     ssl上下文
 *
 * @return 
 *   = 0 - 缓存中无未读数据\n
 *   = 1 - 缓存中有数据未读
 *
 * @details 
 */
int cm_ssl_check_pending(void *cm_ssl_ctx);

/**
 * @brief 缓存区可读数据长度
 *
 * @param [in] cm_ssl_ctx     ssl上下文
 *
 * @return 
 *   缓存区可读数据长度
 *
 * @details 
 */
int cm_ssl_get_bytes_avail(void *cm_ssl_ctx);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_SSL_H__ */

/** @}*/
