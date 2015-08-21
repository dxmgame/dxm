@echo off

set ocd=%cd%
cd /d %~dp0/..

setlocal enabledelayedexpansion

rem 创建模板的cmake文件
set cmake_file=%~dp0\cmake\dxm_template\dxm_template.cmake

rem 获取模板列表
echo 获取模板列表...
for /d %%i in (templates/*) do set template_array=!template_array! %%i

:begin
echo -----------------------------------------------------------------
echo -- 创建工程类型
set index=0
for %%i in (%template_array%) do (
	set /a index=!index!+1
	set name=%%i
	for /f "delims=" %%i in (templates\%%i\template_desc.txt) do (
		echo -- !index!.!name!: %%i
	)	
)

echo -- 
echo -- 0.exit:退出
echo -----------------------------------------------------------------
set /p val= -- 请选择类型:

rem 选择模板
rem set select=1
set select=%val%
if %select%==0 ( 
	echo Exit
	goto cmdExit 
)

set template_type=null
for /f "tokens=%select% delims= " %%i in ("%template_array%") do set template_type=%%i
if %template_type%==null (
	goto begin
)
echo %template_type%

rem 选择文件夹
echo ---------------------------------------------------------------------
echo -- 模块相对路径(unix style),例如 src/apps/xxx
echo ---------------------------------------------------------------------
set /p module_name=-- 请输入：
echo %module_name%

rem 执行创建工程
cmake -DMODULE_NAME=%module_name% -DTEMPLATE_TYPE=%template_type% -P %cmake_file%

if %errorlevel% neq 0 goto :cmEnd

echo create done
pause

cls
goto begin

:cmEnd
echo create failed
pause

cls
goto begin

:cmdExit
cd %ocd%

endlocal



