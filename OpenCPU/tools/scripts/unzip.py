# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2022 China Mobile IOT. All rights reserved.
#
# Date: 2022/09/05
# Author: zhangxw
# Function: 解压缩原始固件包
#
#
# ===================================================================


import os
import sys
import glob
import zipfile


# 解压文件到指定目录
def unzip_file(file_in, path_out):
    zip_list = zipfile.ZipFile(file_in, 'r')  # 载入压缩文件

    for name in zip_list.namelist():
        file_name = path_out + '\\' + name  # 判断文件是否已解压缩
        if os.path.isfile(file_name) is not True:
            zip_list.extract(name, path_out)  # 解压位置

    zip_list.close()


# 传入压缩文件路径及输出文件路径
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("参数错误或缺失", len(sys.argv))
        sys.exit()

    zip_path = str(sys.argv[1])
    out_path = str(sys.argv[2])

    if os.path.exists(zip_path) is not True:  # 判断压缩文件文件是否存在
        print("文件不存在：", zip_path)
        sys.exit()

    if os.path.exists(out_path) is not True:  # 当输出文件夹不存在
        os.makedirs(out_path)

    file_names = glob.glob(zip_path + "/*.zip")
    
    for file in file_names:
        unzip_file(file, out_path)
