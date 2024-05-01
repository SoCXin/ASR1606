@goto start
:: ML305A OpenCPU编译脚本，请勿修改
:: 当前支持DS/DC子型号编译
:: SDK编译路径不支持中文、空格

:start

:: 参数设置
@set PROJECT_ROOT=%CD%
@set MODEL_NAME=ML305A
@set HW_REVISION=H0
@set CURRENT_HOUR=%TIME:~0,2%
@if %CURRENT_HOUR% LEQ 9 set CURRENT_HOUR=0%TIME:~1,1%
@set SW_REVISION=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%%CURRENT_HOUR%%TIME:~3,2%
@set BUILD_CMD=all
@set GNSS_OPT=NO
@set FLASH_OPT=4M

@if /i "%1" == "DC" (
    @set MODEL_TARGET=DC
    @set FLASH_OPT=4M
    @set GNSS_OPT=NO
)else if /i "%1" == "DS" (
    @set MODEL_TARGET=DS
    @set FLASH_OPT=8M
    @set GNSS_OPT=NO
)else if /i "%1" == "CLEAN" (
    @set BUILD_CMD=clean
)else if /i "%1" == "help" (
    @echo usage: build.bat version demo
    @echo     version:
    @echo       DC:compile ML305A DC H0 version
    @echo       DS:compile ML305A DS H0 version
    @echo       CLEAN:clean the project
    @exit /B
)else (
    @echo param error
    @echo example: ML305A_build.bat DC
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
