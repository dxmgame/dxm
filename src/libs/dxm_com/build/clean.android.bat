set ocd=%cd%
cd /d %~dp0
cd ..

rmdir /S /Q proj.android


cd /d %ocd%
