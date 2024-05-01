#ifndef __CM_DEMO_ALIYUN_H__
#define __CM_DEMO_ALIYUN_H__

#include <stdint.h>
#include <stddef.h>

/*
 * 使用此测试函数需自行在阿里云平台创建设备，并补充本文件中的设备的三元组、接入点、topic名称
 */
void cm_test_aliyun(unsigned char **cmd, int len);

 /**
 *  @brief 阿里云OTA升级Demo
 * 
 *  @param void
 * 
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 运行该用例需要配置实例的接入点mqtt_host以及设备的product_key,device_name,device_secret,配置与阿里云平台配置保持一致,另外:
 *           1. 执行升级后模组会进行多次重启,且升级异常后无法还原至升级前的版本,请在FOTA升级前先使用单个模组调试,保证升级无异常后再进行大规模升级;
 *           2. 执行升级后url有效期为24小时,且升级异常后无法还原至升级前的版本,模组必须在升级前保证模组的网络,电源,SIM流量剩余等,防止升级失败无法恢复;
 *           3. 升级后需要客户自行版本管理并调用cm_aliyun_ota_version_report接口向阿里云平台上报版本号
 */
void cm_aliyun_ota_test(unsigned char **cmd, int len);

 /**
 *  @brief 阿里云平台上报OTA版本号,用于OTA升级完成后向服务器上报升级后版本
 *
 *  @param [in] ver : 待上报的设备版本号字符串
 * 
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 用于OTA升级完成后向服务器上报升级后版本,上报完成后即可断开连接并释放资源
 *           客户可以在初始化时版本管理过程中上报版本
 */
int cm_aliyun_ota_version_report(const char *ver);

#endif