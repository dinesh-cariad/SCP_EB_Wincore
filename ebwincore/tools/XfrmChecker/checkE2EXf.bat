@echo off

rem
rem all variables defined in this script must not propagate out to the shell
rem
SETLOCAL ENABLEDELAYEDEXPANSION

rem derive paths to tooling subfolders from path by which this .bat has been called
SET CURRFOLDER=%~dp0
SET CP=%CURRFOLDER%\E2EXfCV\E2EXfCheck.jar
SET APP=eb.tresos.e2exfcheck.E2EXfCheckerApp
call %CURRFOLDER%\checkXfrm.bat %*
exit /b %errorlevel%
