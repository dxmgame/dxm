set ocd=%cd%
cd /d %~dp0
cd ..

rmdir /S /Q proj.win64


cd /d %ocd%
