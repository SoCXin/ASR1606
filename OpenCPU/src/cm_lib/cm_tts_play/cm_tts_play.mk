

CM_TTS_DIR :=src/cm_lib/cm_tts_play


OC_FILES +=  $(CM_TTS_DIR)/cm_tts_play.c

OC_LIBS += prebuild/libs/cm_local_tts.a
OC_LIBS += prebuild/libs/lib_tts.a