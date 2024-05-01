::created by xgl,2019/1/24
@if %1 == 1 (goto  DELFILE)
:DELDIR
@echo delete folder
@rd /s /q  %2 > nul 2>nul
@goto end

:DELFILE
@echo delete file
@del /q %2 > nul 2>nul

:end

