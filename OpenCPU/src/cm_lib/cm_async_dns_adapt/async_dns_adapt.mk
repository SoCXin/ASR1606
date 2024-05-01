

CM_ASYNC_DNS_ADAPT_DIR :=src/cm_lib/cm_async_dns_adapt


OC_FILES +=  $(CM_ASYNC_DNS_ADAPT_DIR)/async_dns_adapt.c
INC      += -I'$(CM_ASYNC_DNS_ADAPT_DIR)'