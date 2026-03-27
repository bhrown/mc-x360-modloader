set VSDIR=C:\Program Files (x86)\Microsoft Visual Studio 11.0
set XDKDIR=C:\Program Files (x86)\Microsoft Xbox 360 SDK
set IDADIR=F:\Program Files\IDA Professional 9.0

set PATH=%XDKDIR%\bin\win32;%VSDIR%\VC\bin;%VSDIR%\Common7\IDE;%PATH%
set INCLUDE=%XDKDIR%\include\xbox;%VSDIR%\VC\include
set LIB=%XDKDIR%\lib\xbox;%VSDIR%\VC\lib

mkdir build 2>nul

cl.exe ^ /c /Od /GS- /TP ^ /D _XBOX ^ /MT ^ /Fo"build\module.obj" ^ src\module.cpp
link.exe ^ build\module.obj ^ /DLL ^ /DEF:src\module.def ^ /OUT:build\module.xex

cl.exe ^ /c /Od /GS- /TP ^ /D _XBOX ^ /MT ^ /Fo"build\loader.obj" ^ src\loader.cpp
REM link.exe ^ build\loader.obj ^ /OUT:build\loader.xex

"%IDADIR%\ida.exe" -A -S"F:\minecraft-lce\patch\ida\1_dump_fun.py" "F:\minecraft-lce\patch\build\loader.obj"
