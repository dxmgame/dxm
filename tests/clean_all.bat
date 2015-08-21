REM ×¢ÊÍ
@echo off

for /f %%i in ('dir /b /s clean.win32.bat') do (
call %%i)

for /f %%i in ('dir /b /s clean.win64.bat') do (
call %%i)

@echo on
pause