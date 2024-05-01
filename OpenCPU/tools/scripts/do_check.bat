::created by xgl,2019/1/24
:param
@set str=%~1
@if "%str%"=="" (goto end)
@for %%a in (%str%) do @if NOT exist out\%%a (md out\%%a)
@shift /0
@goto param
:end