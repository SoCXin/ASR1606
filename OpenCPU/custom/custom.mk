include tools/scripts/config.mk

CUSTOM_DIR := custom

ifeq ($(strip $(DEMO_SUPPORT)),n)
include $(CUSTOM_DIR)/custom_main/custom_main.mk
endif