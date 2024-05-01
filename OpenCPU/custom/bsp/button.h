#pragma once

#include "cm_gpio.h"
#include "cm_iomux.h"

typedef enum{
    BUTTON_RELEASED = 0x00,
    BUTTON_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_INCOMING,
}buttonStatus;

typedef struct{
    cm_gpio_num_e num;
    cm_iomux_pin_e pin;
    cm_gpio_pull_e mode;
    cm_gpio_direction_e direction;
    cm_gpio_interrupt_e irq;
    void (*call_back)(void);
}ButtonInfo;

typedef struct{
    void (*pressed)(void*);
    buttonStatus (*check)(void*);
    buttonStatus (*get_status)(void*);
}BUTTON_IMPLEMENTS;

typedef struct{
    BUTTON_IMPLEMENTS interface;
    ButtonInfo info;
    buttonStatus status; //按键状态
    void (*init)(void*, ButtonInfo); //初始化按键
}BUTTON;

BUTTON* BUTTON_CTOR(void);
void BUTTON_DTOR(BUTTON* t);
