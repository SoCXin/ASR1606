/**
 * @file        cm_demo_lcd.c
 * @brief       OpenCPU LCD测试例程
 * @copyright   Copyright ? 2021 China Mobile IOT. All rights reserved.
 */


 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <string.h>
#include "cm_lcd.h"
#include "cm_iomux.h"
#include "cm_gpio.h"
#include "cm_demo_uart.h"
#include "cm_mem.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CM_LCD_DEMO_LOG   cm_demo_printf



/****************************************************************************
 * Private Types
 ****************************************************************************/


 
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Data
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void cm_lcd_test_gpio_init(cm_gpio_num_e test_gpio)
{
    cm_gpio_cfg_t cfg = {
        .direction = CM_GPIO_DIRECTION_OUTPUT,
        .pull = CM_GPIO_PULL_DOWN
    };
    cm_gpio_init(test_gpio, &cfg);
}

static void cm_lcd_test_backlight_on(cm_gpio_num_e test_gpio)
{
    cm_gpio_set_level(test_gpio, CM_GPIO_LEVEL_HIGH);
}

static void cm_lcd_test_set_area(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start & 0xFF);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end & 0xFF);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start & 0xFF);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end & 0xFF);

    cm_lcd_write_cmd(0x2c);
}
uint16_t s_lcdbuffer[128 * 128];
static void cm_lcd_test_show_color(void)
{
    int i, j;
    uint8_t width = 128, height = 128;
    uint16_t const red    = (0x1F << 11) | (0x00 << 5) | 0x00;
    uint16_t const green  = (0x00 << 11) | (0x3F << 5) | 0x00;
    uint16_t const blue   = (0x00 << 11) | (0x00 << 5) | 0x1F;
    uint16_t const black  = (0x00 << 11) | (0x00 << 5) | 0x00;
    uint16_t const white  = (0x1F << 11) | (0x3F << 5) | 0x1F;
    uint16_t color_bar[] = {
        red, green, blue, black, white,
    };

    uint32_t color_number = sizeof(color_bar) / sizeof(color_bar[0]);

    
    
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            *(s_lcdbuffer + i + j * width) = color_bar[j * color_number / height];
        }
    }

    cm_lcd_test_set_area(0, 0, width - 1, height - 1);

    cm_lcd_write_buf((uint8_t *)s_lcdbuffer, height * width * 2);
}

void cm_lcd_test_init(void)
{
    cm_lcd_test_gpio_init(CM_GPIO_NUM_7); // 以GPIO7为例
    cm_lcd_init();

    cm_lcd_write_cmd(0x11); //sleep out
    osDelay(120);
    
    cm_lcd_write_cmd(0xB1);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_cmd(0xB2);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_cmd(0xB3);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_cmd(0xB4);  //column inversion
    cm_lcd_write_data(0x03);   
    cm_lcd_write_cmd(0xC0);
    cm_lcd_write_data(0x62);
    cm_lcd_write_data(0x02);
    cm_lcd_write_data(0x04);
    cm_lcd_write_cmd(0xC1);
    cm_lcd_write_data(0xC0);
    cm_lcd_write_cmd(0xC2);
    cm_lcd_write_data(0x0D);
    cm_lcd_write_data(0x00);
    cm_lcd_write_cmd(0xC3);
    cm_lcd_write_data(0x8D);
    cm_lcd_write_data(0x6A);
    cm_lcd_write_cmd(0xC4);
    cm_lcd_write_data(0x8D);
    cm_lcd_write_data(0xEE);
    cm_lcd_write_cmd(0xC5);  //vcom
    cm_lcd_write_data(0x12);
    cm_lcd_write_cmd(0x36);  
    cm_lcd_write_data(0x68);//cm_lcd_write_data(0x68);////MX,MV,RGB mode
    osDelay(20);
    cm_lcd_write_cmd(0xE0);
    cm_lcd_write_data(0x03);
    cm_lcd_write_data(0x1B);
    cm_lcd_write_data(0x12);
    cm_lcd_write_data(0x11);
    cm_lcd_write_data(0x3F);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x32);
    cm_lcd_write_data(0x34);
    cm_lcd_write_data(0x2F);
    cm_lcd_write_data(0x2B);
    cm_lcd_write_data(0x30);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(0x01);
    cm_lcd_write_data(0x02);
    cm_lcd_write_data(0x05);
    
    cm_lcd_write_cmd(0xE1);
    cm_lcd_write_data(0x03);
    cm_lcd_write_data(0x1B);
    cm_lcd_write_data(0x12);
    cm_lcd_write_data(0x11);
    cm_lcd_write_data(0x32);
    cm_lcd_write_data(0x2F);
    cm_lcd_write_data(0x2A);
    cm_lcd_write_data(0x2F);
    cm_lcd_write_data(0x2E);
    cm_lcd_write_data(0x2C);
    cm_lcd_write_data(0x35);
    cm_lcd_write_data(0x3F);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(0x01);
    cm_lcd_write_data(0x05);
    
    cm_lcd_write_cmd(0x3A); //65K mode
    cm_lcd_write_data(0x05);
    cm_lcd_write_cmd(0x35);
    cm_lcd_write_data(0x00);
    
    
    cm_lcd_test_show_color();
    cm_lcd_test_backlight_on(CM_GPIO_NUM_7);
    

    cm_lcd_write_cmd(0x29);  //display on
    
}

void cm_test_lcd(unsigned char **cmd, int len)
{
    cm_lcd_test_init();
    return;
}

