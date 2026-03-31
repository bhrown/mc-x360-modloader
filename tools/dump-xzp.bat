set XDKDIR=C:\Program Files (x86)\Microsoft Xbox 360 SDK
set PATH=%XDKDIR%\bin\win32;%PATH%

mkdir ..\game\dump 2>nul
mkdir ..\game\dump\media-dump 2>nul
cd ..\game\dump\media-dump

imagexex.exe /DUMP /SECTIONNAME:media /SECTIONFILE:"..\media.xzp" ..\..\default.xex
xuipkg.exe /U "..\media.xzp" /NOCONVERT

