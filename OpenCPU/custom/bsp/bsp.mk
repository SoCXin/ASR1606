DRV_UART_DIR := custom/bsp

INC      += -I'$(DRV_UART_DIR)'

#button
OC_FILES +=  $(DRV_UART_DIR)/button.c

#transceiver
OC_FILES +=  $(DRV_UART_DIR)/transceiver.c

#display
OC_FILES +=  $(DRV_UART_DIR)/display.c

#socket_client
OC_FILES +=  $(DRV_UART_DIR)/socket_client.c

#bsp_dial
# OC_FILES +=  $(DRV_UART_DIR)/bsp_dial.c

# #bsp_call
# OC_FILES +=  $(DRV_UART_DIR)/bsp_call.c

#bsp_socket
# OC_FILES +=  $(DRV_UART_DIR)/bsp_socket.c
