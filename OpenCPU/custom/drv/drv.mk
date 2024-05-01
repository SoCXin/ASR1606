DRV_UART_DIR := custom/drv

#uart
OC_FILES +=  $(DRV_UART_DIR)/uart/drv_virt_at.c
OC_FILES +=  $(DRV_UART_DIR)/uart/drv_uart.c
INC      += -I'$(DRV_UART_DIR)/uart'

#lw_oopc
INC      += -I'$(DRV_UART_DIR)/lw_oopc'

#gpio
OC_FILES +=  $(DRV_UART_DIR)/gpio/drv_gpio.c
INC      += -I'$(DRV_UART_DIR)/gpio'

#socket
OC_FILES +=  $(DRV_UART_DIR)/socket/drv_socket.c
INC      += -I'$(DRV_UART_DIR)/socket'

#audio
OC_FILES +=  $(DRV_UART_DIR)/audio/amr_array.c
OC_FILES +=  $(DRV_UART_DIR)/audio/amr_busy.c
OC_FILES +=  $(DRV_UART_DIR)/audio/amr_callend.c
OC_FILES +=  $(DRV_UART_DIR)/audio/amr_callstart.c
OC_FILES +=  $(DRV_UART_DIR)/audio/amr_connect.c
OC_FILES +=  $(DRV_UART_DIR)/audio/amr_timeout.c
OC_FILES +=  $(DRV_UART_DIR)/audio/drv_audio.c
INC      += -I'$(DRV_UART_DIR)/audio'
