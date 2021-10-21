@echo off
:: Note: Project setup - user make environment

:: set PROJECT_ROOT to the current project directory (relative to location of this batch file, converted to absolute path)
for /f %%i in ("%~dp0..") do set PROJECT_ROOT=%%~fi

:: Call the optional launch_cfg.bat
:: This file contains TARGET, DERIVATE, TRESOS_BASE, and TOOLPATH_COMPILER.
:: These variables can also be provided as environment variables.
if exist "%~dp0launch_cfg.bat" call %~dp0launch_cfg
if exist "%TRESOS_BASE%\..\launch_cfg_user.bat" call %TRESOS_BASE%\..\launch_cfg_user

:: check, if the path variables for PROJECT_ROOT is set
if not defined TRESOS_BASE goto missing_TRESOS_BASE
if not exist "%TRESOS_BASE%" goto missing_TRESOS_BASE
if not exist "%PLUGINS_BASE%" goto missing_PLUGINS_BASE
:: add %TRESOS_BASE%\bin to path
set PATH=%PATH%;%TRESOS_BASE%\bin;

:: ensure working directory is util folder of current project directory (location of this batch file)
cd /D "%~dp0%"
set EXIT_CODE=0

:: check for command line parameters
if [%1]==[] goto interactive
if [%1]==[/c] goto call_command_c
if [%1]==[make] goto call_batch
if [%~x1]==[.bat] goto call_batch

:call_command
:: invoke command and terminate
cmd /c %*
set EXIT_CODE=%ERRORLEVEL%
goto finish

:call_command_c
:: invoke command and terminate
cmd %*
set EXIT_CODE=%ERRORLEVEL%
goto finish

:call_batch
:: invoke batch and terminate
call %*
set EXIT_CODE=%ERRORLEVEL%
goto finish

:interactive
:: start interactive command shell and display make rules
cmd /k make show_rules
goto :eof

:: The variable TRESOS_BASE was not specified
:missing_TRESOS_BASE
echo.
echo    The variable TRESOS_BASE was not set correctly:
echo    %TRESOS_BASE%.
echo    Please specify this variable manually by editing
echo    the batch file 'launch_cfg.bat'.
echo.
set EXIT_CODE=1
if [%1]==[] pause
goto :finish

:: The path given by PLUGINS_BASE does not exist
:missing_PLUGINS_BASE
echo.
echo    The variable PLUGINS_BASE was not set correctly:
echo    %PLUGINS_BASE%.
echo    Please substitute you EBWincore directory to S.\
echo    e.g. subst s: C:\Users\xyz\Documents\zec\eb-wincore-8.8.3 
echo.
set EXIT_CODE=1
if [%1]==[] pause

:finish
exit /B %EXIT_CODE%
