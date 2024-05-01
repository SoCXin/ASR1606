/**
 * @file        cm_fota.h
 * @brief       FOTA
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      
 * @date        
 *
 * @defgroup fota fota
 * @ingroup FOTA
 * @{
 */

/*----------------------------------------------------------------------------*
 **                             Dependencies                                   *
 **----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Mcaro Definitions                              *
**----------------------------------------------------------------------------*/

#define CM_FOTA_STRING_LEN        48
#define CM_FOTA_LONG_STRING_LEN   256
#define CM_FOTA_FIXED_RESERVED_LEN (512 - 8 - CM_FOTA_LONG_STRING_LEN - 2 * CM_FOTA_STRING_LEN)

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**----------------------------------------------------------------------------*/
/**
 *   FOTA相关结构体
*/
typedef enum
{
    CM_FOTA_CMIOT_ERROR_UNKNOW = 850,
    CM_FOTA_CMIOT_ERROR_CONFIG_FILE,
    CM_FOTA_CMIOT_ERROR_URL_TYPE,
    CM_FOTA_CMIOT_ERROR_WRITE_CONFIG,
    CM_FOTA_CMIOT_ERROR_UPDATE,
    CM_FOTA_CMIOT_ERROR_DOWNLOAD,
    CM_FOTA_CMIOT_ERROR_DOWNLOAD_MAX_RETRY,
    CM_FOTA_CMIOT_ERROR_NO_NETWORK
} cm_fota_error_e;


typedef enum
{
    CM_FOTA_ASR_PLAN_INVALID = 0,     /*!< 不可用 */
    CM_FOTA_ASR_PLAN_MINI_INTEGRATE,  /*!< ASR 整包 mini FOTA方案，推荐用户使用该方案 */
    CM_FOTA_ASR_PLAN_MINI,            /*!< ASR 2包 mini FOTA方案，兼容老用户方案，新用户不建议使用该方案 */
    CM_FOTA_ASR_PLAN_PRO              /*!< ASR PRO FOTA方案，不支持 */
} cm_fota_asr_plan_e;

typedef enum
{
    CM_FOTA_TYPE_INVALID = 0,
    CM_FOTA_TYPE_FTP,
    CM_FOTA_TYPE_HTTP_HTTPS
} cm_fota_url_type_e;

typedef enum
{
    CM_FOTA_STAGE_NONE = 0,            /*!< 无FOTA */
    CM_FOTA_STAGE_START,               /*!< 开始阶段,准备下载bin1文件 */
    CM_FOTA_STAGE_BIN1_OTA,            /*!< bin1文件下载完成,准备bootloader ota */
    CM_FOTA_STAGE_BIN1_SUCC,           /*!< bootloader ota完成,准备下载bin2文件 */
    CM_FOTA_STAGE_ERR_ALLOW_RETRY,     /*!< FOTA失败,重启后运行再次尝试,一般是下载出错,可能与网络相关 */
    CM_FOTA_STAGE_SUCC,                /*!< FOTA完成 */
    CM_FOTA_INTEGRATE_STAGE_START,     /*!< FOTA mini整包升级方案开始 */
    CM_FOTA_INTEGRATE_STAGE_BIN2,      /*!< FOTA mini整包升级方案开始下载bin2 */
    CM_FOTA_STAGE_ERR                  /*!< FOTA失败 */
} cm_fota_stage_e;

/** 重启时间结构体类型
 *  FOTA机制为分别升级最小系统和应用APP，共计需要四个阶段，每个阶段任务完成后系统会适当延时保存数据后自动重启并进入下一阶段，正常情况下模组一次完整的FOTA升级需要经历4次重启 \n
 *  若模组FOTA过程中任一阶段失败或超时，则需要重启模组（重启模组后模组会重新尝试完成本阶段的任务），此场景下的重启时间由cm_fota_set_reboot_time()接口设置 \n
 *  开始计时时间为尝试FOTA的开始时间，非检测到FOTA失败的时间（超时未完成该阶段升级任务也是失败）。例如，尝试重启的超时时间为15分钟，模组8:30分尝试FOTA，8:35分检测到FOTA失败，则下一次重启时间为8:45分 \n
 *  网络较差或升级包较大时，尝试重启的超时时间不宜设置过短。例如，某场景下下载升级包的时间需要5分钟，则设置的时间建议至少为6分钟 \n
 *  默认（用户未设置时）为{5，10, 20, 30, 40, 50，120}，即在一直FOTA失败的场景下，模组首次失败（或超时未完成任务）5分钟后自行重启并重新尝试FOTA，再失败则10分钟后重启，之后分别是20分钟、30分钟、40分钟、50分钟、120分钟、120分钟、120分钟...
 */
typedef struct
{
    int first_timeout;                  /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第一次尝试重启的超时时间 */
    int second_timeout;                 /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第二次尝试重启的超时时间 */
    int third_timeout;                  /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第三次尝试重启的超时时间 */
    int fourth_timeout;                 /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第四次尝试重启的超时时间 */
    int fifth_timeout;                  /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第五次尝试重启的超时时间 */
    int sixth_timeout;                  /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第六次尝试重启的超时时间 */
    int constant_timeout;               /*!< 单位：分钟；范围：3分钟至3天（3~3*24*60）；含义：一次升级期间任一阶段升级失败或超时未完成时，第七次及之后的尝试重启的超时时间 */
} cm_fota_reboot_time;

typedef struct
{
    int isSetTimeout;                    /*!< 用户是否设置重启时间。非0：用户已成功设置；0：用户未设置或设置不合法，采用默认参数 */
    int failure;                         /*!< 失败次数 */
    cm_fota_reboot_time timeout;         /*!< 用户配置时间参数 */
} cm_fota_extendible_info_t;

typedef struct
{
    cm_fota_stage_e stage;               /*!< FOTA升级进行状态 */
    cm_fota_url_type_e fota_mode;        /*!< fota升级模式 */
    char url[CM_FOTA_LONG_STRING_LEN];   /*!< 服务器url */
    char username[CM_FOTA_STRING_LEN];   /*!< 用户名*/
    char passwd[CM_FOTA_STRING_LEN];     /*!< 密码 */
    char reserved[CM_FOTA_FIXED_RESERVED_LEN];
} cm_fota_info_fixed_t;

typedef struct
{
    cm_fota_info_fixed_t      fixed_info;   /*!< FOTA配置保存NV固定部分结构 */
    cm_fota_extendible_info_t extend_info;  /*!< FOTA配置保存NV可扩展部分结构 */
} cm_fota_info_t;                           /*!< FOTA配置相关结构体:FOTA固定配置部分+FOTA可扩展配置部分 */
/*----------------------------------------------------------------------------*
**                             Function Define                                *
**----------------------------------------------------------------------------*/


 /**
 *  @brief fota结果回调函数
 *
 *  @param [in] error 错误码
 *
 *  @details 整包 mini FOTA方案下，模组运行在大系统时，fota出现错误时才会回调
 */
 typedef void (*cm_fota_result_callback)(cm_fota_error_e error);

 /**
 *  @brief 设置FOTA服务器url
 *
 *  @param [in] url 服务器url
 *
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 字符串，可为HTTP服务器、FTP服务器，最大256字节，url中必须指定协议类型，比如"ftp://xxx.com:23/update1.bin_1"或"http://xxx.com:8080/update2.bin_1" \n
 *           ML307A-DCLN ML307A-GCLN ML305A-DC支持HTTP（含HTTPS无证书校验）下载方式，ML307A-DSLN ML307A-GSLN ML305A-DS支持HTTP（含HTTPS无证书校验）、FTP下载方式，不支持FTPS下载方式 \n
 *           本平台使用双包升级方式（存在两个升级包）时，url中下载的文件名必须以.bin_1结尾，且.bin_1与.bin_2文件的名称必须一致，比如system_patch.bin_1和system_patch.bin_2 \n
 *           本平台使用完整包升级方式（存在一个升级包）时，url中下载的文件名无要求 \n
 *           FTP下载不支持ipv6方式，不可使用ipv6地址
 */
int cm_fota_set_url(char *url);

 /**
 *  @brief 设置FOTA服务器登录名和密码（仅FTP方式）
 *
 *  @param [in] username 登录名
 *  @param [in] passwd   密码
 *
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 仅FTP下载方式适用，HTTP下载方式不适用
 */
int cm_fota_set_auth(const char *username, const char *passwd);

 /**
 *  @brief 设置FOTA失败时的下一次重启时间（重启后会继续尝试升级）
 *
 *  @param [in] time 重启时间结构体（详情见cm_fota_reboot_time结构体定义）
 *
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details FOTA过程会分别升级最小系统和应用APP，两者任一升级失败均会触发重启机制 \n
 *           定时起始时间为尝试FOTA升级的时间，非检测到FOTA失败的时间。例如，设置下一次重启时间为15分钟，模组8:30分尝试FOTA，8:35分检测到FOTA失败，则下一次重启时间为8:45分 \n
 *           下一次重启时间默认（用户未设置时）为{5，10, 20, 30, 40, 50，120}
 */
int cm_fota_set_reboot_time(cm_fota_reboot_time *time);

 /**
 *  @brief 获取FOTA服务器配置
 *
 *  @param [out] cfg 服务器配置
 *
 *  @details 该接口用于查询fota_mode、url、username、passwd，不适用于查询stage及其他
 */
void cm_fota_read_config(cm_fota_info_t *cfg);

 /**
 *  @brief  配置FOTA OTA升级方案
 *
 *  @param [in] ota_plan : CM_FOTA_ASR_PLAN_MINI_INTEGRATE - mini整包方案, CM_FOTA_ASR_PLAN_MINI - mini双包方案
 *
 *  @details 未配置时，默认为mini整包方案
 */
 void cm_fota_set_ota_plan(cm_fota_asr_plan_e ota_plan);

 /**
 *  @brief 获取FOTA OTA升级方案
 *
 *  @return OTA升级方案
 */
 cm_fota_asr_plan_e cm_fota_get_ota_plan(void);

  /**
  *  @brief SDK注册fota结果回调
  *
  *  @return void
  *
  *  @details 用于大系统执行FOTA升级任务预校验时将结果反馈给SDK用户应用程序,callback可由客户定制 \n
  *           该接口仅适用于整包 mini FOTA方案（CM_FOTA_ASR_PLAN_MINI_INTEGRATE），不适用于2包 mini FOTA方案
  */
 void cm_fota_res_callback_register(cm_fota_result_callback cb);

 /**
 *  @brief 进入mini system执行fota升级
 *
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 一次FOTA期间模组会重启4次，需要数分钟时间。若每阶段触发该阶段任务超时未完成逻辑，则重启次数会相应增加 \n
 *           请至少在OpenCPU程序运行5秒中后再执行该接口，开机立即执行可能会因底层最小系统未完成初始化造成失败 \n
 *           fota升级前，配置url会进行文件系统操作，若文件系统剩余空间较小配置url可能会失败。建议使用fota功能的用户在文件系统中至少留有4096字节剩余空间（cm_fs_getinfo()接口可查询）
 */
int cm_fota_exec_upgrade(void);
