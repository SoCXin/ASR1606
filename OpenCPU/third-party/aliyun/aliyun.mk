
ETHIRDPARTY_CJSON_DIR := third-party/aliyun/LinkSDK



OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/aiot_state_api.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/aiot_mqtt_api.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/sysdep/core_adapter.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/sysdep/core_sysdep.c

OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_auth.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_diag.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_global.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_http.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_log.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_sha256.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/core/utils/core_string.c

OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/ali_ca_cert.c

OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/portfiles/aiot_port/freertos_port.c

OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/aes.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/asn1parse.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/base64.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/bignum.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/cipher.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/cipher_wrap.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/ctr_drbg.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/debug.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/entropy.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/error.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/md.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/md_wrap.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/oid.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/pem.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/pk.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/pk_wrap.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/pkparse.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/platform.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/platform_util.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/rsa.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/rsa_internal.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/sha1.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/sha256.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/ssl_ciphersuites.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/ssl_cli.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/ssl_cookie.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/ssl_tls.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/x509.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/library/x509_crt.c

OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/components/ota/aiot_ota_api.c
OC_FILES +=  $(ETHIRDPARTY_CJSON_DIR)/components/ota/ota_md5.c

INC      += -I'$(ETHIRDPARTY_CJSON_DIR)/core'
INC      += -I'$(ETHIRDPARTY_CJSON_DIR)/core/sysdep'
INC      += -I'$(ETHIRDPARTY_CJSON_DIR)/core/utils'
INC      += -I'$(ETHIRDPARTY_CJSON_DIR)/external/mbedtls/include'
INC      += -I'$(ETHIRDPARTY_CJSON_DIR)/components/ota'