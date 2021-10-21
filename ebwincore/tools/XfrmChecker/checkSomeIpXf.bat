@echo off

rem
rem all variables defined in this script must not propagate out to the shell
rem
SETLOCAL ENABLEDELAYEDEXPANSION

rem derive paths to tooling subfolders from path by which this .bat has been called
SET CURRFOLDER=%~dp0
SET CP=%CURRFOLDER%\SomeIpXfCV\SomeIpXfCheck.jar
SET APP=eb.tresos.someipxfcheck.SomeIpXfCheckerApp
call %CURRFOLDER%\checkXfrm.bat %*
exit /b %errorlevel%