@echo off

if exist %~dp0..\config.bat call %~dp0..\config.bat
if exist %~dp0..\..\config.bat call %~dp0..\..\config.bat
if exist %~dp0..\..\..\config.bat call %~dp0..\..\..\config.bat
if exist %~dp0..\..\..\..\config.bat call %~dp0..\..\..\..\config.bat
if exist %~dp0..\..\..\..\..\config.bat call %~dp0..\..\..\..\..\config.bat
if exist %~dp0..\..\..\..\..\..\config.bat call %~dp0..\..\..\..\..\..\config.bat
if exist %~dp0..\..\..\..\..\..\..\config.bat call %~dp0..\..\..\..\..\..\..\config.bat

set CHANNEL_NAMEX=
call :GET_CHANNEL_NAME %0\..
echo %CHANNEL_NAMEX%

call "%ANTBIN%\ant" -f %~dp0build.xml release

:END
cd %ocd%
goto :eof

:GET_CHANNEL_NAME
set CHANNEL_NAMEX=%~n1
