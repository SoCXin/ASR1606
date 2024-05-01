#*********************************************************
#  @file    Makefile
#  @brief   ML307A OpenCPU Makefile
#  Copyright (c) 2022 China Mobile IOT.
#  All rights reserved.
#  created by XieGangLiang 2019/09/29
#  modify by zxw@20220825
#*******************************************************

.PHONY: all clean

# 配置文件
include tools/scripts/config.mk

# 库文件及用户代码
OC_RAM_FILES :=
OC_FILES :=
OC_LIBS :=
INC :=
CDEFINES :=
include src/cm_lib/cm_lib.mk
include third-party/third-party.mk
ifeq ($(DEMO_SUPPORT),y)
include examples/examples.mk
else
include custom/custom.mk
endif

# 版本控制及目标文件
ifeq ($(MODEL_TARGET),)
endif
APP_IMG_DIR := out/images/$(MODEL_NAME)/$(MODEL_TARGET)
APP_TARGET := $(MODEL_NAME)_$(MODEL_TARGET)_APP
APP_LD_OLD := prebuild/ld/$(MODEL_NAME)/$(MODEL_TARGET)/app_$(MODEL_TARGET).ld
APP_LD := $(APP_IMG_DIR)/$(APP_TARGET).ld.obj
APP_CORE_STUB := prebuild/objs/$(MODEL_NAME)/$(MODEL_TARGET)/cm_stub_$(MODEL_TARGET).o

# GCC配置
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
AR  := arm-none-eabi-ar
OBJCOPY  := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

# 编译参数
CFLAGS :=  -mthumb-interwork  -std=gnu11   -mcpu=cortex-r5     -mno-unaligned-access -g --specs=nosys.specs -Wall -ffunction-sections -fdata-sections
CDEFINES += -DCM_HARDWARE_VERSION=\"$(MODEL_TARGET)\" -DCM_HARDWARE_$(MODEL_TARGET)_SUPPORT -DCM_OPENCPU_MODEL_$(MODEL_NAME) -DCM_PROD_VERSION_$(HW_REVISION) -DCM_BUILD_TIMESTAMP=\"$(SW_REVISION)\"
LDFLAGS := -lgcc -lc $(APP_CORE_STUB) -Wl,--gc-sections  -nostartfiles --specs=nosys.specs  -mcpu=cortex-r5  -mtune=generic-armv7-r   -mno-unaligned-access  -Wl,-Map=$(APP_IMG_DIR)/$(APP_TARGET).map  -T $(APP_LD) -Wl,--start-group $(OC_LIBS) -Wl,--end-group
INC += -Iinclude\cmiot -Iinclude\platform -Iinclude\platform\lwip -Iinclude\platform\lwip\arch -Iinclude\platform\lwip\ipsec -Iinclude\platform\lwip\ipv4 -Iinclude\platform\lwip\ipv6 -Iinclude\platform\lwip\lwip -Iinclude\platform\lwip\netif

SRC_DIRS := $(sort $(dir $(OC_FILES)))
OBJS := $(addprefix out/build/, $(subst .c,.o,$(subst .cpp,.o,$(OC_FILES))))
SRC_RAM_DIRS := $(sort $(dir $(OC_RAM_FILES)))
OBJS_RAM := $(addprefix out/build/, $(subst .c,.sram.o,$(subst .cpp,.sram.o,$(OC_RAM_FILES))))
DEPS := $(subst .o,.o.d, $(OBJS))
DEPS += $(subst .sram.o,.sram.o.d, $(OBJS_RAM))

# 生成APP文件
all: check  $(OBJS) $(OBJS_RAM) $(APP_LD)
	@echo "linking $(MODEL_NAME) libs..."
	@echo "Generating $(APP_TARGET).elf..."
	@$(CXX)  $(OBJS) $(OBJS_RAM) $(LDFLAGS) -o $(APP_IMG_DIR)/$(APP_TARGET).elf
	@$(SIZE) -B $(APP_IMG_DIR)/$(APP_TARGET).elf
	@echo "Generating $(APP_TARGET).bin..."
	@$(OBJCOPY) -O binary -R .note -R .comment -S $(APP_IMG_DIR)/$(APP_TARGET).elf $(APP_IMG_DIR)/$(APP_TARGET).bin
	@echo "Done"

$(APP_LD): $(APP_LD_OLD)
	@$(CXX) -E -P -w - <$(APP_LD_OLD) -o $(APP_LD)

check:
	@do_check.bat $(addprefix build\,$(subst /,\,$(SRC_DIRS)))
	@do_check.bat $(addprefix build\,$(subst /,\,$(SRC_RAM_DIRS)))
	@do_check.bat build\$(MODEL_NAME)\$(MODEL_TARGET)   logs 
	@do_check.bat images\$(MODEL_NAME)\$(MODEL_TARGET)

out/build/%.o:%.c
	@echo 'compile $<'
	@$(CC) $(CFLAGS) $(BUILD_FLAG) $(INC) $(CDEFINES)  -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@

out/build/%.o:%.cpp
	@echo 'compile $<'
	@$(CXX) $(CFLAGS) $(BUILD_FLAG) $(INC) $(CDEFINES) -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@

out/build/%.sram.o:%.c
	@echo 'compile $<'
	@$(CC) $(CFLAGS) $(BUILD_FLAG) $(INC) $(CDEFINES)  -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@

out/build/%.sram.o:%.cpp
	@echo 'compile $<'
	@$(CXX) $(CFLAGS) $(BUILD_FLAG) $(INC) $(CDEFINES) -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@

# 清除编译文件	
clean:
	@echo 'clear compiled files'
	@clean.bat 0 out

-include $(DEPS)