
EXAMPLE_ALIYUN_DIR := examples/aliyun



OC_FILES +=  $(EXAMPLE_ALIYUN_DIR)/src/cm_demo_aliyun.c
OC_FILES +=  $(EXAMPLE_ALIYUN_DIR)/src/cm_demo_aliyun_ota.c
INC      += -I'$(EXAMPLE_ALIYUN_DIR)/inc'