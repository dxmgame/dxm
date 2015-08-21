@echo off


set path=%path%;"C:\Program Files (x86)\Git\bin"
echo ----------------------------------------------------------
echo -- create project
echo ----------------------------------------------------------
set /p val=-- 请输入工程git路径:

cd /d %val%

rem 创建子模块
git submodule add https://github.com/dxmgame/dxm.git dxm
if %errorlevel% neq 0 goto :cmEnd

git submodule update --init --recursive
if %errorlevel% neq 0 goto :cmEnd

rem 拷贝模板文件
xcopy /e %~dp0\templates_project\* %cd%
if %errorlevel% neq 0 goto :cmEnd

goto cmDone

:cmEnd
echo setup failed
pause
exit

:cmDone

