#ifndef __CM_DEMO_CAM_H__
#define __CM_DEMO_CAM_H__

#include <stdint.h>
#include <stddef.h>

void cm_test_camera_preview(unsigned char **cmd, int len);

void cm_test_camera_capture(unsigned char **cmd, int len);

void cm_test_qr_decode(unsigned char **cmd, int len);

#endif
