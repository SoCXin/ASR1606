# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2022 China Mobile IOT. All rights reserved.
#
# Date: 2022/08/05
# Author: zhangxw
# Function: 固件签名
#
# 签名流程：①提取bin文件md5码；②对md5码进行RSA签名（pkcs1_15填充）；③将签名结果及md5码放置于bin文件开头，增加固定长度256（RSA） + 16(MD5)
# 秘钥存储：公钥存储在模组中，私钥存储在编译环境中
#
# ===================================================================


import sys
import os.path
from Crypto.Signature import pkcs1_15
from Crypto.Hash import MD5
from Crypto.PublicKey import RSA


# 功能：固件签名
# private_key_fn 私钥文件
# bin_fn 待签名文件
# sign_fn 签名后文件
def package_sign(private_key_fn, bin_fn, sign_fn):
    with open(private_key_fn) as private_key_file, \
            open(bin_fn, "rb") as bin_file, \
            open(sign_fn, "wb") as sign_file:
        private_key = RSA.import_key(private_key_file.read())
        bin_data = bin_file.read()
        digest = MD5.new(bin_data)   # 生成16字节MD5码
        print("MD5:", digest.digest())
        # 使用私钥对HASH值进行签名
        signature = pkcs1_15.new(private_key).sign(digest)  # 私钥签名
        sign_file.write(signature)  # 写入256字节签名信息
        sign_file.write(digest.digest())  # 写入16字节MD5信息
        sign_file.write(bin_data)  # 写入固件信息
        return signature


# 功能：固件验签
# public_key_fn 公钥文件
# sign_fn 签名固件
def package_check(public_key_fn, sign_fn):
    with open(public_key_fn) as public_key_file, \
            open(sign_fn, "rb") as sign_file:
        public_key = RSA.import_key(public_key_file.read())
        sign_data = sign_file.read(256)  # 读取签名信息
        sign_file.read(16)  # 读取MD5信息
        bin_data = sign_file.read()  # 读取固件信息
        md5_data = MD5.new(bin_data)
        # print(sign_data)
        print("MD5:", md5_data.digest())
        try:
            pkcs1_15.new(public_key).verify(md5_data, sign_data)
            print("签名验证成功！！！")
        except:
            print("签名验证失败！！！")


# 功能：生成公私秘钥对
def pem_generate(length):
    key = RSA.generate(length)
    private_key = key.export_key()
    public_key = key.publickey().export_key()
    with open("private_key.rsa", "wb") as pri_file, \
            open("public_key.rsa", "wb") as pub_file:
        pri_file.write(private_key)
        pub_file.write(public_key)


# 传入待签名文件，在同一个文件夹下生成签名后文件
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("参数错误:", len(sys.argv))
        sys.exit()

    bin_filename = str(sys.argv[1])
    private_key_filename = str(sys.argv[2])

    if os.path.isfile(bin_filename) is not True:  # 判断bin文件是否存在
        print("bin文件不存在", bin_filename)
        sys.exit()
    if os.path.isfile(private_key_filename) is not True:  # 判断私钥文件是否存在
        print("私钥文件不存在", private_key_filename)
        sys.exit()

    bin_dir = os.path.dirname(os.path.realpath(bin_filename))
    bin_name = os.path.basename(bin_filename)
    sign_bin_file_name = bin_dir + "\sign_" + bin_name  # 签名后的文件名

    print("开始签名，文件名：", bin_name)
    # 将会在bin文件开头增加272固定长度，包括256bytes的签名信息及16bytes的md5信息
    sign = package_sign(private_key_filename, bin_filename, sign_bin_file_name)  # 生成签名包
    # print("签名信息：", sign)
    print("签名完成，文件名：", sign_bin_file_name)
    # package_check("public_key.rsa", sign_bin_file_name)  # 验签
