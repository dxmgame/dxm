@echo off

set ocd=%cd%
cd /d %~dp0/..

setlocal enabledelayedexpansion

rem ����ģ���cmake�ļ�
set cmake_file=%~dp0\cmake\dxm_template\dxm_template.cmake

rem ��ȡģ���б�
echo ��ȡģ���б�...
for /d %%i in (templates/*) do set template_array=!template_array! %%i

:begin
echo -----------------------------------------------------------------
echo -- ������������
set index=0
for %%i in (%template_array%) do (
	set /a index=!index!+1
	set name=%%i
	for /f "delims=" %%i in (templates\%%i\template_desc.txt) do (
		echo -- !index!.!name!: %%i
	)	
)

echo -- 
echo -- 0.exit:�˳�
echo -----------------------------------------------------------------
set /p val= -- ��ѡ������:

rem ѡ��ģ��
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

rem ѡ���ļ���
echo ---------------------------------------------------------------------
echo -- ģ�����·��(unix style),���� src/apps/xxx
echo ---------------------------------------------------------------------
set /p module_name=-- �����룺
echo %module_name%

rem ִ�д�������
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



