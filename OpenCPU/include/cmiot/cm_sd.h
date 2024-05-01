/**
 * @file        cm_sd.h
 * @brief       SDCARD
 * @copyright   Copyright © 2022 China Mobile IOT. All rights reserved.
 * @author      By WangPeng
 * @date        2022/12/16
 *
 * @defgroup SDCARD SDCARD
 * @ingroup SDCARD
 * @{
 */


#ifndef __CM_SDCARD_H__
#define __CM_SDCARD_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CM_SD_SEEK_SET      (0)      /*!< SEEK_SET */
#define CM_SD_SEEK_CUR      (1)      /*!< SEEK_CUR */
#define CM_SD_SEEK_END      (2)      /*!< SEEK_END */

#define CM_SD_MAX_PATH (256)

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef struct
{
    long long free_size;  /*!< 当前可用文件系统大小 */
    long long total_size; /*!< 文件系统总大小 */
} cm_sd_system_info_t;


/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 ***********************SD卡使用注意事项*************************************
 * 1、通常情况开发板的硬件设计上，SD卡不会单独供电，即SD卡硬件上下
 *    电和复位不由模组控制，故模组SD功能是在开发板上电时（也是SD卡
 *    上电时）完成SD卡初始化操作
 * 2、如1所言，模组上电时，默认情况下，SD卡控制引脚会发出一段脉冲用
 *    于初始化SD卡。发出初始化脉冲操作早于OpenCPU SDK的入口函数执行
 * 3、对于不使用SD卡功能的用户，上述2中发起的脉冲可能干扰用户硬件使
 *    用（例如用户需要将该PIN脚配置为UART用于控制挂载芯片，该PIN脚发
 *    发出的脉冲存在干扰挂载芯片工作的可能）。故SDK提供给用户禁用SD卡
 *    发出脉冲的机制，如4所言
 * 4、用户需禁止SD功能发出脉冲时，可在文件系统中创建一个内容为"0"，名称
 *    为"sd.cfg"的文件用于禁止SD卡控制引脚发出脉冲，重启生效。模组上电
 *    的逻辑为：检查文件系统是否存在"sd.cfg"的文件，若存在则读取其内容，
 *    若内容为"0"，则禁止SD卡控制引脚发出脉冲。创建文件操作参考如下：
 *      int32_t fd = cm_fs_open("sd.cfg", CM_FS_WB);
 *      int32_t f_len = cm_fs_write(fd, "0", 1);
 *      int32_t ret = cm_fs_close(fd);
 * 5、SD卡控制引脚和复用关系见《资源综述》
 ***************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/**
 *  @brief 查询（SD卡）文件系统是否格式化
 *
 *  @param [in] vol 逻辑驱动器号指针
 *
 *  @return 
 *   TRUE  - SD卡已格式化 \n
 *   FALSE - SD卡未格式化
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
bool cm_sd_is_formatted(char *vol);

/**
 *  @brief 格式化（SD卡）文件系统
 *
 *  @param [in] vol 逻辑驱动器号指针
 *
 *  @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能 \n
 *           因软件驱动特性，cm_sd_format()接口应在执行SD文件操作前调用。若需要获取SD文件系统的关键数据后格式化SD卡，应在进行完SD文件系统操作并重启模组后格式化SD卡。
 */
int32_t cm_sd_format(char *vol);

/**
 *  @brief 打开一个文件
 *
 *  @param [in] filename 文件名称
 *  @param [in] mode 参数（r w c a +）
 *
 *  @return 
 *   >= 0 - 文件描述符 \n
 *   < 0  - 失败, 返回值为错误码
 *
 *  @details r: read \n
 *           w: write \n
 *           c: create \n
 *           a: append and write \n
 *           +: read and write \n
 *           SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fopen(const char *filename, const char *mode);

/**
 *  @brief 关闭一个文件
 *
 *  @param [in] fd 文件描述符
 *
 *  @return 
 *   = 0  - 成功 \n
 *   < 0  - 失败, 返回值为错误码
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fclose(int32_t fd);

/**
 *  @brief 读取文件内容
 *
 *  @param [in]  fd   文件描述符
 *  @param [out] buf  存储数据的缓冲区指针
 *  @param [in]  size 要读取的数据长度
 *
 *  @return
 *   >= 0 - 实际的读取长度 \n
 *   <  0 - 读取失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fread(int32_t fd, void *buf, uint32_t size);

/**
 *  @brief 写入文件内容
 *
 *  @param [in] fd   文件描述符
 *  @param [in] buf  存储数据的缓冲区指针
 *  @param [in] size 要写入的数据长度
 *
 *  @return
 *   >= 0 - 实际的写入长度 \n
 *   <  0 - 写入失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fwrite(int32_t fd, const void *buf, uint32_t size);

/**
 *  @brief 文件指针定位
 *
 *  @param [in] fd 文件描述符
 *  @param [in] offset 指针偏移量
 *  @param [in] base 偏移起始点，CM_SD_SEEK_SET：文件开头 CM_SD_SEEK_CUR：当前位置 CM_SD_SEEK_END：文件末尾
 *
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details offset不支持为负数 \n
 *           SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fseek(int32_t fd, int32_t offset, int32_t base);

/**
 *  @brief 删除文件
 *
 *  @param [in] file_name 文件路径
 *
 *  @return
 *   >= 0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fdelete(const char *file_name);

/**
 *  @brief 更改文件名
 *
 *  @param [in] src  原文件名
 *  @param [in] dest 新文件名
 *
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_fmove(const char *src, const char *dest);

/**
 *  @brief 检查文件是否存在
 *
 *  @param [in] file_name 文件名称
 *
 *  @return
 *   = 1 - 存在 \n
 *   = 0 - 不存在
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_exist(const char *file_name);

/**
 *  @brief 获取文件大小
 *
 *  @param [in] file_name 文件路径
 *
 *  @return
 *   >= 0 - 文件长度 \n
 *   <  0 - 操作失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_filesize(const char *file_name);

/**
 *  @brief 创建文件夹
 *
 *  @param [in] path 文件夹路径
 *
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_mkdir(char *path);

/**
 *  @brief 删除文件夹
 *
 *  @param [in] path 文件夹路径
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_rmdir(char *path);

/**
 *  @brief 设置当前文件夹路径
 *
 *  @param [in] path 文件夹路径
 *
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_set_currentdir(char *path);

/**
 *  @brief 获取当前文件夹路径
 *
 *  @param [out] path 用于保存文件夹路径的buffer
 *  @param [in]  max_len 用于保存文件夹路径的buffer的长度
 *
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_get_currentdir(char *path, uint32_t max_len);

/**
 *  @brief 获取文件系统信息
 *
 *  @param [in] vol 逻辑驱动器号指针
 *  @param [out] info 文件系统信息
 *
 *  @return
 *   = 0 - 成功 \n
 *   < 0 - 失败
 *
 *  @details SD卡功能与ML307A uart0和ML302A uart1互斥，不可同时使用 \n
 *           仅ML307A-DSLN/ML307A-GSLN/ML302A-DSLM/ML302A-GSLM模组支持SD功能。
 */
int32_t cm_sd_getinfo(char *vol, cm_sd_system_info_t *info);


#undef EXTERN
#ifdef __cplusplus
}
#endif

/** @}*/
/** @}*/

#endif /* __CM_SDCARD_H__ */

