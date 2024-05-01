
@echo off

del /s /q /f fbf_dfota.bin
del /s /q /f a\patchfolder\*
FBFMake_CF_V1.6-150.exe -f config -d 0x10000 -a a -b b -o fbf_dfota.bin -q
