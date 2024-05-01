/**
 * @file        cm_lcd.h
 * @brief       内存管理
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By lixuelin
 * @date        2022/12/28
 *
 * @defgroup LCD
 * @ingroup LCD
 * @{
 */

#ifndef __CM_LCD_H__
#define __CM_LCD_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "stdint.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LCD_PIN_RST         77 // HAL_GPIO_77
#define LCD_PIN_SPI_4L_DCX  14 // HAL_GPIO_14
#define LCD_PIN_SPI_CS      13 // HAL_GPIO_13
#define LCD_PIN_BACKLIGHT   23 // HAL_GPIO_23

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief 配置时发送cmd
 *  
 * @param [in] cmd 需要发送的命令
 *
 * @return 
 *
 * @details More details
 */
void cm_lcd_write_cmd(unsigned char cmd);

/**
 * @brief 配置时发送data
 *  
 * @param [in] data 需要发送的数据
 *
 * @return 
 *
 * @details More details
 */
void cm_lcd_write_data(unsigned char data);

/**
 * @brief 发送刷屏数据
 *  
 * @param [in] data 需要发送的数据
 * @param [in] len  数据长度
 *
 * @return 
 *
 * @details More details
 */
void cm_lcd_write_buf(unsigned char *data, int len);

/**
 * @brief lcd 驱动初始化
 * 
 *
 * @return 
 *
 * @details More details
 */
void cm_lcd_init(void);

/**
 * @brief lcd 驱动去初始化
 * 
 *
 * @return 
 *
 * @details More details
 */
void cm_lcd_deinit(void);

#endif