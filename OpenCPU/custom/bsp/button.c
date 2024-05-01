#include "button.h"
#include <stdio.h>
#include <stdlib.h>
#include "drv_gpio.h"
#include "drv_uart.h"

#define DBG_NAME "button"

void button_init(void* t, ButtonInfo info)
{
    BUTTON* this = t;
    this->status = BUTTON_RELEASED;
    memcpy(&this->info, &info, sizeof(ButtonInfo));

    cm_gpio_cfg_t cfg = {
        .direction = this->info.direction,
        .pull = this->info.mode
    };

    my_gpio_init(this->info.num, this->info.pin, cfg, CM_IOMUX_FUNC_FUNCTION2, this->info.irq, this->info.call_back);
    // my_io_init();
    DBG_F("Button init success\r\n");
}

void button_pressed(void* t)
{
    BUTTON* this = t;
    if(this->status == BUTTON_RELEASED){
        this->status = BUTTON_CLICK;
    }else if(this->status == BUTTON_CLICK){
        this->status = BUTTON_DOUBLE_CLICK;
    }else if(this->status == BUTTON_DOUBLE_CLICK){
        this->status = BUTTON_DOUBLE_CLICK;
    }

    // DBG_F("Button pressed %d\r\n", this->status);
}

buttonStatus button_check(void* t)
{
    BUTTON* this = t;
    buttonStatus ret = this->status;

    if(this->status != BUTTON_RELEASED){
        // DBG_F("Button is %d\r\n", this->status);
    }

    this->status = BUTTON_RELEASED;
    
    return ret;
}

buttonStatus button_get_status(void* t)
{
    BUTTON* this = t;
    buttonStatus ret = this->status;
    
    return ret;
}

BUTTON* BUTTON_CTOR(void)
{
    BUTTON *this = (BUTTON*)malloc(sizeof(BUTTON));
    this->init = button_init;
    this->interface.pressed = button_pressed;
    this->interface.check = button_check;
    this->interface.get_status = button_get_status;
    return (BUTTON*)this;
}

void BUTTON_DTOR(BUTTON* t)
{
    free(t);
}


