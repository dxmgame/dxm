@echo off


set path=%path%;"C:\Program Files (x86)\Git\bin"
echo ----------------------------------------------------------
echo -- create project
echo ----------------------------------------------------------
set /p val=-- �����빤��git·��:

cd /d %val%

rem ������ģ��
git submodule add https://github.com/dxmgame/dxm.git dxm
if %errorlevel% neq 0 goto :cmEnd

git submodule update --init --recursive
if %errorlevel% neq 0 goto :cmEnd

rem ����ģ���ļ�
xcopy /e %~dp0\templates_project\* %cd%
if %errorlevel% neq 0 goto :cmEnd

goto cmDone

:cmEnd
echo setup failed
pause
exit

:cmDone

