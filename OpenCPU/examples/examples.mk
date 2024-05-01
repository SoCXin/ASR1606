

EXAMPLE_DIR :=examples

ifeq ($(strip $(DEMO_SUPPORT)),y)
include $(EXAMPLE_DIR)/demo_main/demo_main.mk
include $(EXAMPLE_DIR)/uart/uart.mk
include $(EXAMPLE_DIR)/asocket/asocket.mk
include $(EXAMPLE_DIR)/fs/fs.mk
include $(EXAMPLE_DIR)/gpio/gpio.mk
include $(EXAMPLE_DIR)/pm/pm.mk
include $(EXAMPLE_DIR)/lbs/lbs.mk
include $(EXAMPLE_DIR)/http/http.mk
include $(EXAMPLE_DIR)/virt_at/virt_at.mk
include $(EXAMPLE_DIR)/ntp/ntp.mk
include $(EXAMPLE_DIR)/i2c/i2c.mk
include $(EXAMPLE_DIR)/adc/adc.mk
include $(EXAMPLE_DIR)/spi/spi.mk
include $(EXAMPLE_DIR)/lcd/lcd.mk
include $(EXAMPLE_DIR)/keypad/keypad.mk
include $(EXAMPLE_DIR)/pwm/pwm.mk
include $(EXAMPLE_DIR)/mqtt/mqtt.mk
include $(EXAMPLE_DIR)/audio/audio.mk
include $(EXAMPLE_DIR)/fota/fota.mk
include $(EXAMPLE_DIR)/tts/tts.mk
include $(EXAMPLE_DIR)/sd/sd.mk
include $(EXAMPLE_DIR)/cJSON/cJSON.mk
include $(EXAMPLE_DIR)/gnss/gnss.mk
include $(EXAMPLE_DIR)/aliyun/aliyun.mk
include $(EXAMPLE_DIR)/wifiscan/wifiscan.mk
include $(EXAMPLE_DIR)/camera/cam.mk
include $(EXAMPLE_DIR)/ftp/ftp.mk
endif