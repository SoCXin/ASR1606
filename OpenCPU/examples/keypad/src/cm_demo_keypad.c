/*********************************************************
 *  @file    cm_demo_gpio.c
 *  @brief   OpenCPU GPIO示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include "stdlib.h"
#include "cm_demo_keypad.h"
#include "cm_keypad.h"
#include "cm_common.h"
#include "cm_iomux.h"
#include "cm_demo_uart.h"
#include "cm_os.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

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
static void cm_keypad_callback(cm_keypad_map_t key, cm_keypad_event_e event)
{
    cm_demo_printf("key:%02x,state:%d\n",key,event);
}

/****************************************************************************
 * Public Functions
****************************************************************************/
void cm_test_keypad(unsigned char **cmd, int len)
{
    cm_keypad_kp_mkin_e col[CM_KP_MKI_NO_VALID] = {CM_KP_MKI_NO_VALID};
    cm_keypad_kp_mkout_e row[CM_KP_MKI_NO_VALID] = {CM_KP_MKI_NO_VALID};
    uint8_t col_len = (uint8_t)atoi((char *)cmd[2]);
    uint8_t row_len = (uint8_t)atoi((char *)cmd[2+col_len+1]);
    uint8_t i = 0;
    uint8_t keypad_value;
    cm_keypad_cfg_t cm_keypad_cfg = {0};
    
    cm_demo_printf("cm keypad config col X row(%d X %d)\n",col_len,row_len);
    
    cm_demo_printf("cm keypad config col:");
    for(i=0;i<col_len;i++)
    {
        keypad_value = (uint8_t)atoi((char *)cmd[3+i]);
        col[i] = keypad_value;
        cm_demo_printf("%d ",col[i]);
    }
    cm_demo_printf("\n");
    
    cm_demo_printf("cm keypad config row:");
    for(i=0;i<row_len;i++)
    {
        keypad_value = (uint8_t)atoi((char *)cmd[2+col_len+2+i]);
        row[i] = keypad_value;
        cm_demo_printf("%d ",row[i]);
    }
    cm_demo_printf("\n");
    
    cm_keypad_cfg.cm_col = col;
    cm_keypad_cfg.cm_row = row;
    cm_keypad_cfg.cm_col_len = col_len;
    cm_keypad_cfg.cm_row_len = row_len;
    
    cm_keypad_register(cm_keypad_callback);
    cm_keypad_config(&cm_keypad_cfg);
    
    if(0 == cm_keypad_init())
    {
        cm_demo_printf("cm_keypad_init OK\n");
    }
    else
    {
        cm_demo_printf("cm_keypad_init FAILED\n");
    }
}
