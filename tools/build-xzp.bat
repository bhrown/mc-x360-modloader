set XDKDIR=C:\Program Files (x86)\Microsoft Xbox 360 SDK
set PATH=%XDKDIR%\bin\win32;%PATH%

cd ..\game\dump\media-dump

xuipkg.exe /O "..\mod.xzp" /R * /NOCONVERT
