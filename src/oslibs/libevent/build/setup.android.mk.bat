@echo off

set ocd=%cd%

cd /d %~dp0
cd ..

echo ##### ��ʾ����ȡ�����ļ� #####
if exist ..\config.bat call ..\config.bat
if exist ..\..\config.bat call ..\..\config.bat
if exist ..\..\..\config.bat call ..\..\..\config.bat
if exist ..\..\..\..\config.bat call ..\..\..\..\config.bat
if exist ..\..\..\..\..\config.bat call ..\..\..\..\..\config.bat

echo ##### ��ʾ���򲹶� #####
rem rmdir /s/Q %DXM_COCOS_PATH%\extensions\spine
rem xcopy /y/s patch\* %DXM_COCOS_PATH%\

md proj.android\jni
copy /y Android.mk proj.android\jni

cd %ocd%
@echo on

