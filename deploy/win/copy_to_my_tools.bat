@echo off
:: Copy exe files to %1

:: Initialize
set DEFAULT_TOOLS_PATH=c:\tools
set CLEAN_EXE_PATCH=%~dp0clean_rich_signature
set ROOT_PATH=%~dp0..\..
set TOOLS_PATH=%1

:: Remove last character backslash
:remove_delimeter
if "%TOOLS_PATH:~-1%"=="\" (
    set TOOLS_PATH=%TOOLS_PATH:~0,-1%
    goto :remove_delimeter
)

:: Set default path
if "%TOOLS_PATH%"=="" (
    set TOOLS_PATH=%DEFAULT_TOOLS_PATH%
)

:: Copy and patch
copy /V %ROOT_PATH%\bin\substi.exe %TOOLS_PATH%\
spatch.exe -nobackup %TOOLS_PATH%\substi.exe %CLEAN_EXE_PATCH% 
