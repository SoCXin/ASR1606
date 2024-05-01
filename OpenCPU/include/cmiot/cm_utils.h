/**
 * @file        cm_utils.h
 * @brief       通用定义
 * @copyright   Copyright @2023 China Mobile IOT. All rights reserved.
 * @author      By zhangxw
 * @date        2023/02/09
 */


#ifndef __CM_UTILS_H__
#define __CM_UTILS_H__

/*函数声明时可通过此宏定义指定加载段落，不加修饰即默认使用.text段*/
#define __RUN_AT_RAM    __attribute__((used, section(".sramtext")))
#define __RUN_AT_XIP    __attribute__((used, section(".text")))

#endif
