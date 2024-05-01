@goto start
:: ML307A/ML302A 固件打包脚本
:start

@set BASE_LINE_DIR=%PROJECT_ROOT%\prebuild\baseline\%MODEL_NAME%\%MODEL_TARGET%
@set BUILD_IMG_DIR=%PROJECT_ROOT%\out\images\%MODEL_NAME%\%MODEL_TARGET%
@set BASE_IMG_DIR=%BUILD_IMG_DIR%\DBG

@set RD_IMG=%BASE_IMG_DIR%\ReliableData.bin
@set RF_IMG=%BASE_IMG_DIR%\rf.bin
@set CP_IMG=%BASE_IMG_DIR%\cp.bin
@set APP_IMG=%BUILD_IMG_DIR%\%MODEL_NAME%_%MODEL_TARGET%_APP.bin
@set STD_IMG="cp=%CP_IMG%,rd=%RD_IMG%,rfbin=%RF_IMG%,user_app=%APP_IMG%"
@set PKG_FILE=%MODEL_NAME%_%MODEL_TARGET%_APP.zip
@set TEMPLATE=CRANEL_A0

::解压基础固件包
@python tools\scripts\unzip.py %BASE_LINE_DIR% %BASE_IMG_DIR%

@if "%GNSS_OPT%" == "YES" (
    set TEMPLATE=%TEMPLATE%_GNSS
)

@if "%FLASH_OPT%" == "4M" (
    set TEMPLATE=%TEMPLATE%_04MB
) else (
    set TEMPLATE=%TEMPLATE%_08MB
)

@if exist %BUILD_IMG_DIR% (
    @echo Generating %PKG_FILE%...
    arelease -c %PROJECT_ROOT%\tools\aboot -g -p ASR_CRANEL_EVB -v %TEMPLATE% -i %STD_IMG%  %BUILD_IMG_DIR%\%PKG_FILE% > nul
    @echo Done
) else (
    @echo directory %BUILD_IMG_DIR% not exist
)
