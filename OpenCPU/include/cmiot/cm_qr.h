/**
 * @file        cm_qr.h
 * @brief       二维码解码接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By lixuelin
 * @date        2023/02/10
 *
 * @defgroup QR
 * @ingroup QR
 * @{
 */
 
#ifndef __CM_QR_H__
#define __CM_QR_H__

#define CM_QR_DATA_LEN_MAX   512    /*!< 解析结果数据最大长度 */
/**
* @param [in] param 传入的用户参数
* @param [in] data  二维码解析结果
*/
typedef int (*cm_qr_cb)(void *param, const char *data);

/* 解析参数 */
typedef struct
{
    void     *data;                 /*!< 图像数据*/
    int      width;                 /*!< 图像宽度*/
    int      height;                /*!< 图像高度*/
    int      format;                /*!< 图像原始格式，需转码为灰阶图像(Y800)*/
    void     *usrdata;              /*!< 扫描线程回调的参数*/
    cm_qr_cb callback;              /*!< 扫描线程回调*/
} cm_qr_param_t;

/* 图像编码格式 */
typedef enum
{
    CM_QR_FMT_UYVY,                 /*!< UYVY格式图像 */
    CM_QR_FMT_Y800,                 /*!< 灰阶图像 */
    CM_QR_FMT_OTHER,                /*!< 无效格式 */
} cm_qr_fmt_e;

/* 二维码解析结果码 */
typedef enum 
{
    CM_QR_RET_SUCCESS,              /*!< 解码成功 */
    CM_QR_RET_NOT_INIT,             /*!< 未初始化 */
    CM_QR_RET_CREATE_FAILED,        /*!< 解码器创建失败 */
    CM_QR_RET_MEMORY_ERR,           /*!< RAM剩余最大块空间不够 */
    CM_QR_RET_FMT_ERR,              /*!< 传入的图像格式错误 */
    CM_QR_RET_CONVERT_ERR,          /*!< 图像灰阶转换错误 */
    CM_QR_RET_SCAN_ERR,             /*!< 灰阶图像扫描错误 */
    CM_QR_RET_NO_DECODE_DATA,       /*!< 解码后 无有效数据 */
} cm_qr_retult_e;

/**
 * @brief 解码缓存初始化.
 * 
 * @param [in] width      图像宽度
 * @param [in] height     图像高度
 * @return 函数执行结果 非0为失败，0为成功
 * 
 * @details 需在解码前调用
 */
int cm_qr_convert_init(int width, int height);

/**
 * @brief   解码缓存回收.
 * 
 * @return 
 * 
 * @details 为避免内存碎片，请尽量减少使用频率
 */
void cm_qr_convert_destroy(void);

/**
 * @brief 二维码解码.
 * 
 * @param [in]  param      待解析图像的参数信息
 * @param [out] data       二维码解析结果
 * @param [out] datalen    解析结果数据的长度
 * 
 * @return
 *   = 0  - 成功 
 *   > 0  - 失败, 返回值为错误码，详见cm_qr_retult_e
 * 
 * @details 解码前需保证当前ram的最大块超过480*640字节作为临时调用空间
 *          如果是在线程中调用解码接口，请至少预留4KB的栈空间。
 */
int32_t cm_qr_scan(cm_qr_param_t *param, char *data, int *datalen);
#endif