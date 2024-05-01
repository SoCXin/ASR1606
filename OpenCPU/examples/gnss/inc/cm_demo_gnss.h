#ifndef __CM_DEMO_GNSS_H__
#define __CM_DEMO_GNSS_H__

#include <stdint.h>
 
void cm_test_gnss_enable(unsigned char **cmd,int len);
void cm_test_gnss_getlocateinfo(unsigned char **cmd,int len);
void cm_test_gnss_getrawdata(unsigned char **cmd,int len);
void cm_test_gnss_sendrawdata(unsigned char **cmd,int len);
void cm_test_gnss_config(unsigned char **cmd,int len);
void cm_test_agnss_update(unsigned char **cmd,int len);
void cm_test_gnss_reset(unsigned char **cmd,int len);
#endif

