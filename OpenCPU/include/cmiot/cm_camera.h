/**
 * @file        cm_camera.h
 * @brief       相机接口
 * @copyright   Copyright @2023 China Mobile IOT. All rights reserved.
 * @author      By chenxy
 * @date        2023/03/13
 *  
 * @defgroup camera
 * @ingroup camera
 * @{
 */

#ifndef __CM_CAMERA_H__
#define __CM_CAMERA_H__

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
 * @brief camera数据输出类型
 */
typedef enum
{
    CAM_OUT_FMT_YUV422_YUYV = 0,
    CAM_OUT_FMT_YUV422_YVYU,
    CAM_OUT_FMT_YUV422_UYVY,
    CAM_OUT_FMT_YUV422_VYUY,
    CAM_OUT_FMT_YUV420_NV12 = 6,
    CAM_OUT_FMT_RAW8 = 10,
}cm_camera_out_fmt_e;

/**
 * @brief camera预览接口配置参数
 */
typedef struct {
    cm_camera_out_fmt_e recordFormat;                               /*!< 输出数据类型 */
    void (*enqueueCamRecordBuffer)(void *pBuf, uint32_t len);       /*!< 输出数据回调函数 */
    void (*flushCamRecordBuffers)(void);                            /*!< 输出关闭回调 */
}cm_camera_preview_cfg_t;

/**
 * @brief camera拍照接口配置参数
 */
typedef struct {
    cm_camera_out_fmt_e recordFormat;                               /*!< 输出数据类型 */
    uint8_t *file_name;                                             /*!< 输出文件名 */
}cm_camera_capture_cfg_t;

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
 * @brief 获取底层驱动输出大小
 *
 * @param [out] width       宽度
 * @param [out] height      高度
 *
 * @return  0 成功；-1 失败
 *
 * @details 获取底层驱动输出大小
 */
int32_t cm_camera_get_info(uint32_t *width, uint32_t *height);

/**
 * @brief 打开相机
 *
 * @return  0 成功；-1 失败
 *
 * @details 打开相机
 */
int32_t cm_camera_open(void);

/**
 * @brief 关闭相机
 *
 * @return  0 成功；-1 失败
 *
 * @details 关闭相机
 */
int32_t cm_camera_close(void);

/**
 * @brief 开始预览
 *
 * @param [in] cfg          配置参数
 *
 * @return  0 成功；-1 失败
 *
 * @details 开始预览，数据通过cfg中设置的回调函数返回，回调函数中可
 * 调用cm_camera_preview_stop关闭预览，但不可调用cm_camera_close
 */
int32_t cm_camera_preview_start(cm_camera_preview_cfg_t *cfg);

/**
 * @brief 关闭预览
 *
 * @return  0 成功；-1 失败
 *
 * @details 关闭预览
 */
int32_t cm_camera_preview_stop(void);

/**
 * @brief 拍照
 *
 * @param [in] cfg          配置参数
 *
 * @return  0 成功；-1 失败
 *
 * @details 拍照，数据自动保存至指定文件
 */
int32_t cm_camera_capture_take(cm_camera_capture_cfg_t *cfg);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_CAMERA_H__ */


/** @}*/


