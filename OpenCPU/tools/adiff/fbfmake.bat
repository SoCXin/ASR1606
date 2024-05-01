
@echo off

del /s /q /f fbf_dfota.bin
del /s /q /f a\patchfolder\*
FBFMake_CF_V1.5.exe -o fbf_dfota.bin -f system.img -r 147456 -d 0x10000 -a a -b b
