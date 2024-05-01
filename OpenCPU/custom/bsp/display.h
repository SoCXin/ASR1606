#pragma once

#include "transceiver.h"
#include "drv_uart.h"

#define Display_Y(str,...) {u0_printf("\033[33;33;1m"str"\033[0;0;0m",##__VA_ARGS__);}
#define Display_R(str,...) {u0_printf("\033[31;31;1m"str"\033[0;0;0m",##__VA_ARGS__);}
#define Display_B(str,...) {u0_printf("\033[36;36;1m"str"\033[0;0;0m",##__VA_ARGS__);}
#define Display_G(str,...) {u0_printf("\033[32;32;1m"str"\033[0;0;0m",##__VA_ARGS__);}
#define Display_N(str,...) {u0_printf(str,##__VA_ARGS__);}

void screen_display(TRANSCEIVER* t);