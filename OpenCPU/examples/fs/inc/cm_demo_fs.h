/*********************************************************
 *  @file    cm_demo_fs.h
 *  @brief   OpenCPU FS header
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by WangPeng 2022/7/14
 ********************************************************/
 #ifndef __CM_DEMO_FS_H_
 #define __CM_DEMO_FS_H_
 
void cm_test_read_file(unsigned char **cmd,int len);
void cm_test_write_file(unsigned char **cmd,int len);
void cm_test_move_file(unsigned char **cmd,int len);
void cm_test_delete_file(unsigned char **cmd,int len);
 #endif