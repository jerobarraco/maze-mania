@echo off

echo./*
echo. * Check VC++ environment...
echo. */
echo.

if defined VS110COMNTOOLS (
    set VSTOOLS="%VS110COMNTOOLS%"
    set VC_VER=110
) else if defined VS100COMNTOOLS (
    set VSTOOLS="%VS100COMNTOOLS%"
    set VC_VER=100
)



set VSTOOLS=%VSTOOLS:"=%
set "VSTOOLS=%VSTOOLS:\=/%"

set VSVARS="%VSTOOLS%vsvars32.bat"

if not defined VSVARS (
    echo Can't find VC2010 or VC2012 installed!
    goto ERROR
)

echo./*
echo. * Building cocos2d-x library binary, please wait a while...
echo. */
echo.

call %VSVARS%
if %VC_VER%==100 (
    msbuild "./HelloCpp.sln" /p:Configuration="Debug"
    msbuild "./HelloCpp.sln" /p:Configuration="Release"
) else if %VC_VER%==110 (
    msbuild HelloCpp.sln /t:Clean
    msbuild HelloCpp.sln /p:Configuration="Debug" /m
    msbuild HelloCpp.sln /p:Configuration="Release" /m
) else (
    echo Script error.
    goto ERROR
)

echo./*
echo. * Check the cocos2d-win32 application "TestCpp.exe" ...
echo. */
echo.

pushd ".\Release.win32\"

goto EOF

:ERROR
pause

:EOF
