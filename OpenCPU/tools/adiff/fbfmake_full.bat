
@echo off

del /s /q /f fbf_dfota.bin
del /s /q /f a\patchfolder\*
FBFMake_CF_V1.6-150.exe -o fbf.bin -f config -a a -b a
