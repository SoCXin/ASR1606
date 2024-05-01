
EXAMPLE_PM_DIR := examples/pm



OC_FILES +=  $(EXAMPLE_PM_DIR)/src/cm_demo_pm.c
INC      += -I'$(EXAMPLE_PM_DIR)/inc'
CDEFINES   += -DCM_SLEEP_SUPPORT
