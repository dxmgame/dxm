set ocd=%cd%
cd /d %~dp0
cd ..

rmdir /S /Q proj.win32


cd /d %ocd%
