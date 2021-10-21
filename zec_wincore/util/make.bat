:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: NAME:      $RCSfile: make.bat,v $, $Revision: 4320 $                                    
::
::
:: FUNCTION:  launches the GNU make. A user makefile and optional
::            parameter can be specified
::
:: USAGE:
::
::            make.bat [-f makefilename] [make_options] [target]
::                  -f           this parameter is optional. If -f is
::                               missing, a default makefile.mak is used
::                  make_options options of make.exe
::                  target       one of the make targets
::                               (listed when calling launch.bat)
::
::            make.bat help      print information on usage
::            make.bat -h
::            make.bat --h
::
::
:: PROJECT:   EB tresos AutoCore
::
:: TOOLCHAIN: GNU Make 4.0
::
:: Copyright (c) 2005 - 2021 Elektrobit Automotive GmbH
::  - all rights reserved -
::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@ECHO OFF

:: a help message is wanted
IF "%1"=="-h" GOTO need_help
IF "%1"=="--help" GOTO need_help
IF "%1"=="help" GOTO need_help

:: check if TRESOS_BASE is set
IF NOT DEFINED TRESOS_BASE GOTO not_tresos_base
:: check if TRESOS_BASE exists
IF NOT EXIST "%TRESOS_BASE%" GOTO no_tresos_base
:: check if PLUGINS_BASE is set
IF NOT DEFINED PLUGINS_BASE SET PLUGINS_BASE=%TRESOS_BASE%/plugins
:: set path of make executable
SET MAKE_EXE=%PLUGINS_BASE%/Make_TS_TxDxM4I0R0/tools/GNU_Make/make.exe
:: check if make executable exists
IF NOT EXIST "%MAKE_EXE%" GOTO no_make_exe

SET MAKE_OPTIONS=
:: a user makefile is specified
IF "%1"=="-f" GOTO call_make
:: no user makefile is specified, use default file
SET MAKE_OPTIONS=-f Makefile.mak
GOTO call_make

:: output a help message on usage of make.bat
:need_help
ECHO.
ECHO.
ECHO    make.bat [-f makefilename] [make_options] [target]
ECHO          -f           this parameter is optional. If -f is
ECHO                       missing, a default makefile.mak is used
ECHO          make_options options of make.exe
ECHO          target       one of the make targets
ECHO                       (listed when calling launch.bat)
ECHO.
ECHO    make.bat help      print this message
ECHO    make.bat -h
ECHO    make.bat --help
ECHO.
ECHO.
GOTO :eof

:: make executable does not exist
:no_make_exe
ECHO.
ECHO    The make executable does not exist.
ECHO    Expected to find it at %MAKE_EXE%
ECHO.
GOTO :eof

:: TRESOS_BASE is not set
:not_tresos_base
ECHO.
ECHO    The environment variable TRESOS_BASE is not set.
ECHO.
GOTO :eof

:: TRESOS_BASE does not exist
:no_tresos_base
ECHO.
ECHO    Tresos base does not exist.
ECHO    Expected to find it at %TRESOS_BASE%
ECHO.
GOTO :eof

:call_make
:: For GNU Make we must explicitly set SHELL to cmd.exe, since otherwise it would try to use sh.exe.
%MAKE_EXE% SHELL=C:/Windows/system32/cmd.exe %MAKE_OPTIONS% %*
