/*********************************************************
*  @file    opencpu_qr.c
*  @brief   ML306 OpenCPU QR decode APIs
*  Copyright (c) 2021 China Mobile IOT.
*  All rights reserved.
*  created by lixuelin 2021/6/05
********************************************************/
#include <stddef.h>
#include <string.h>

#include "zbar.h"
#include "cm_sys.h"

#define CM_QR_DATA_LEN_MAX   512    /*!< 解析结果数据最大长度[不可更改] */

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

typedef int (*cm_qr_cb)(void *param, const char *data, int datalen);
typedef struct
{
    void     *data;                 /*!< 图像数据*/
    int      width;                 /*!< 图像宽度*/
    int      height;                /*!< 图像高度*/
    int      format;                /*!< 图像原始格式，需转码为灰阶图像(Y800)*/
    void     *usrdata;              /*!< 扫描线程回调的参数*/
    cm_qr_cb callback;              /*!< 扫描线程回调*/
} cm_qr_param_t;

/**
 * 4字符码
*/
#undef fourcc
#define fourcc(a, b, c, d)                      \
    ((uint32_t)(a) | ((uint32_t)(b) << 8) |     \
     ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

/* 图像编码格式 */
typedef enum
{
    CM_QR_FMT_UYVY,                 /*!< UYVY格式图像 */
    CM_QR_FMT_Y800,                 /*!< 灰阶图像 */
    CM_QR_FMT_OTHER,                /*!< 无效格式 */
} cm_qr_fmt_e;


struct cm_qr_fmt_table_s
{
    int      fmt_id;  /*!< 图像格式编号 对应cm_qr_image_fmt_e*/
    uint32_t fmt_val; /*!< 图像格式值*/
};

zbar_image_t *zbar_image_convert (const zbar_image_t *src,
                                  unsigned long fmt);
int cm_zbar_convert_buf_init(int width, int height);
void cm_zbar_convert_buf_destroy(void);

/**
 * 图像格式查找表
*/
static struct cm_qr_fmt_table_s s_fmt_table[] = {
    {CM_QR_FMT_UYVY, fourcc('U','Y','V','Y')},
    {CM_QR_FMT_Y800, fourcc('Y','8','0','0')},
};

/**
 * 二维码扫描函数
*/
int32_t cm_qr_scan(cm_qr_param_t *param, char *data, int *datalen)
{
    int cause = CM_QR_RET_NO_DECODE_DATA;
    zbar_image_t *image = NULL; //初始图像
    zbar_image_t *test_image = NULL; //灰阶图像
    zbar_image_scanner_t *scanner = NULL; //解码器
    cm_log_printf(0, "[qr] width=%d height=%d format=%d\n",param->width, param->height, param->format);

    do
    {
        /* 图像数据转码 */
        if(param->format >= CM_QR_FMT_OTHER)
        {
            return CM_QR_RET_FMT_ERR;
        }
        if(param->format == CM_QR_FMT_Y800) //灰阶数据不转码 直接填充数据后解码
        {
            //填充灰阶数据
            test_image = zbar_image_create();
            uint32_t fmt = s_fmt_table[CM_QR_FMT_Y800].fmt_val; //通过编号查找格式
            zbar_image_set_format(test_image, fmt); //指定格式
            zbar_image_set_size(test_image, param->width, param->height); //指定大小
            zbar_image_set_data(test_image, param->data, param->width * param->height, NULL);//填充数据
        }
        else
        {
            //填充原始数据
            image = zbar_image_create(); 
            if(image == NULL)
            {
                cause = CM_QR_RET_CONVERT_ERR;
                break;
            }
            uint32_t fmt = s_fmt_table[param->format].fmt_val; //通过编号查找格式
            zbar_image_set_format(image, fmt); //指定格式
            zbar_image_set_size(image, param->width, param->height);//指定大小
            zbar_image_set_data(image, param->data, param->width * param->height * 2, NULL);//填充数据
            /* 将原始数据转换为灰阶数据 */
            test_image = zbar_image_convert(image,*(int*)"Y800");
            //test_image = image;
        }
        if(test_image == NULL)
        {
            cm_log_printf(0, "[qr] image null\n");
            cause = CM_QR_RET_CONVERT_ERR;
            break;
        }
        /* 创建解码器 */
        scanner = zbar_image_scanner_create();
        if(scanner == NULL)
        {
            cause = CM_QR_RET_CREATE_FAILED;
            break;
        }
        /* 配置解码器 */
        if(zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1) != 0)
        {
            cause = CM_QR_RET_CREATE_FAILED;
            break;
        }
        /* 扫描图像 */
        if(zbar_scan_image(scanner, test_image) < 0)
        {
            cause = CM_QR_RET_SCAN_ERR;
            break;
        }
        /* 提取结果 */
        const zbar_symbol_t *symbol = zbar_image_first_symbol(test_image);
        for(; symbol; symbol = zbar_symbol_next((const zbar_symbol_t *)symbol)) 
        {
            /* TODO */
            const char *ret_data = zbar_symbol_get_data((const zbar_symbol_t *)symbol);
            if(ret_data)
            {   
                int length = zbar_symbol_get_data_length((const zbar_symbol_t *)symbol);
                *datalen = length < CM_QR_DATA_LEN_MAX ? length : CM_QR_DATA_LEN_MAX;
                memcpy(data, ret_data, *datalen);
                cause = CM_QR_RET_SUCCESS;
                break;
            }
        }
    } while (0);
    /* clean up */ 
    if(image)
    {
        /* 这里仅释放image资源 传入的原始数据data不做处理 */
        zbar_image_destroy(image);
    }
    if(test_image)
    {
        /* 这里image的数据为灰阶转换后的内容，destroty()会释放数据data */
        zbar_image_destroy(test_image);
    }
    if(scanner)
    {
        zbar_image_scanner_destroy(scanner);
    }
    
    return cause;
}

/* qr解码 缓存初始化 */
int cm_qr_convert_init(int width, int height)
{
    return cm_zbar_convert_buf_init(width, height);
}

/* qr解码 缓存回收 */
void cm_qr_convert_destroy(void)
{
    cm_zbar_convert_buf_destroy();
}
