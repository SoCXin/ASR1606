

CM_LIB_DIR :=src/cm_lib


include $(CM_LIB_DIR)/cm_async_dns_adapt/async_dns_adapt.mk
include $(CM_LIB_DIR)/modem/modem.mk
include $(CM_LIB_DIR)/cm_tts_play/cm_tts_play.mk
include $(CM_LIB_DIR)/cm_ftp/cm_ftp.mk