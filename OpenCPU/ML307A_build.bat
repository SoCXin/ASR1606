@goto start
:: ML307A OpenCPU编译脚本，请勿修改
:: 当前支持DSLN/DCLN/GSLN/GCLN子型号编译
:: SDK编译路径不支持中文、空格

:start

:: 参数设置
@set PROJECT_ROOT=%CD%
@set MODEL_NAME=ML307A
@set HW_REVISION=H0
@set CURRENT_HOUR=%TIME:~0,2%
@if %CURRENT_HOUR% LEQ 9 set CURRENT_HOUR=0%TIME:~1,1%
@set SW_REVISION=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%%CURRENT_HOUR%%TIME:~3,2%
@set BUILD_CMD=all
@set GNSS_OPT=NO
@set FLASH_OPT=4M

@if /i "%1" == "DCLN" (
    @set MODEL_TARGET=DCLN
    @set FLASH_OPT=4M
    @set GNSS_OPT=NO
)else if /i "%1" == "DSLN" (
    @set MODEL_TARGET=DSLN
    @set FLASH_OPT=8M
    @set GNSS_OPT=NO
)else if /i "%1" == "GSLN" (
    @set MODEL_TARGET=GSLN
    @set FLASH_OPT=8M
    @set GNSS_OPT=YES
)else if /i "%1" == "GCLN" (
    @set MODEL_TARGET=GCLN
    @set FLASH_OPT=4M
    @set GNSS_OPT=YES
)else if /i "%1" == "CLEAN" (
    @set BUILD_CMD=clean
)else if /i "%1" == "help" (
    @echo usage: build.bat version demo
    @echo     version:
    @echo       DCLN:compile ML307A DCLN H0 version
    @echo       DSLN:compile ML307A DSLN H0 version
    @echo       GSLN:compile ML307A GSLN H0 version
    @echo       GCLN:compile ML307A GCLN H0 version
    @echo       CLEAN:clean the project
    @exit /B
)else (
    @echo param error
    @echo example: ML307A_build.bat DCLN
    @exit /B
)

@if /i "%2" == "demo" (
    @set DEMO_SUPPORT=y
)else (
    @set DEMO_SUPPORT=n
)

:: 环境变量配置
@call :add_path %PROJECT_ROOT%\tools
@call :add_path %PROJECT_ROOT%\tools\utils\python3
@call :add_path %PROJECT_ROOT%\tools\toolchain\gcc-arm-none-eabi\bin
@call :add_path %PROJECT_ROOT%\tools\scripts
@call :add_path %PROJECT_ROOT%\tools\utils
@call :add_path %PROJECT_ROOT%\tools\aboot

:: 执行编译
@make  -f tools\scripts\Makefile %BUILD_CMD% -j4 -e ^
MODEL_NAME=%MODEL_NAME% MODEL_TARGET=%MODEL_TARGET% HW_REVISION=%HW_REVISION% SW_REVISION=%SW_REVISION% DEMO_SUPPORT=%DEMO_SUPPORT%

@if %ERRORLEVEL% neq 0 (
    @echo compile error
    @exit /B
)

:: 固件打包
@ if /i "%BUILD_CMD%" == "all" (
    @call tools\scripts\pack.bat
)

:add_path
@(echo ";%PATH%;" | find /C /I ";%1;" > nul) || set "PATH=%1;%PATH%"
@goto :eof
