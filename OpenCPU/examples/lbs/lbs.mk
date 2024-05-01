
EXAMPLE_LBS_DIR := examples/lbs



OC_FILES +=  $(EXAMPLE_LBS_DIR)/src/cm_demo_lbs.c
INC      += -I'$(EXAMPLE_LBS_DIR)/inc'
CDEFINES   += -DCM_LBS_SUPPORT
