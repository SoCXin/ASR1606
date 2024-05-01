
ETHIRDPARTY_CJSON_DIR := third-party/zbar

OC_RAM_FILES += $(ETHIRDPARTY_CJSON_DIR)/src/img_scanner.c
OC_RAM_FILES += $(ETHIRDPARTY_CJSON_DIR)/src/scanner.c
OC_RAM_FILES += $(ETHIRDPARTY_CJSON_DIR)/src/qrdec.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_binarize.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/convert.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qrdectxt.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/image.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_bch15_5.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_finder.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_isaac.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_rs.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/decoder.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_util.c
OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/symbol.c

OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/config.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/error.c
#OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/qr_scan_api.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/refcnt.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/zbar_utils.c

OC_RAM_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/src/opencpu_qr.c

INC      += -I'$(ETHIRDPARTY_CJSON_DIR)/inc'