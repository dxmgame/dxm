@echo off

set ocd=%cd%

cd /d %~dp0
cd ..

echo ##### 提示：读取配置文件 #####
if exist ..\config.bat call ..\config.bat
if exist ..\..\config.bat call ..\..\config.bat
if exist ..\..\..\config.bat call ..\..\..\config.bat
if exist ..\..\..\..\config.bat call ..\..\..\..\config.bat
if exist ..\..\..\..\..\config.bat call ..\..\..\..\..\config.bat

echo ##### 提示：打补丁 #####
rem rmdir /s/Q %DXM_COCOS_PATH%\extensions\spine
rem xcopy /y/s patch\* %DXM_COCOS_PATH%\

md proj.android\jni
copy /y Android.mk proj.android\jni

cd %ocd%
@echo on

