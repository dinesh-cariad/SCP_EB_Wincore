@ECHO OFF
REM Startup tresos ECU in commandline mode

IF "%os%"=="Windows_NT" goto START
IF "%OS%"=="Windows_NT" goto START
IF "%os%"=="WINDOWS_NT" goto START
IF "%OS%"=="WINDOWS_NT" goto START
IF "%os%"=="Windows" goto START
IF "%OS%"=="Windows" goto START
IF "%os%"=="WINDOWS" goto START
IF "%OS%"=="WINDOWS" goto START

goto NOWINDOWS

:NOJAVA
ECHO ERROR: java not found: %JAVA%
EXIT /B 1
GOTO END

:NOWINDOWS
ECHO ERROR: tresos only runs on Windows2000 and Windows/XP
EXIT 1
GOTO END

:END0
IF DEFINED MAKELEVEL EXIT 0
EXIT /B 0

:END1
IF DEFINED MAKELEVEL EXIT 1
EXIT /B 1

:START


REM TRESOS_BASE
SET TRESOS_BASE=%~dp0
SET TRESOS_BASE=%TRESOS_BASE:\bin\=%
SET TRESOS_BASE=%TRESOS_BASE: =" "%

REM JAVA
SET JAVACMD=java.exe
SET JAVA_HOME=%TRESOS_BASE%\jre_x86_64\win32
SET JAVA=%JAVA_HOME%\bin\%JAVACMD%

REM PATH
IF DEFINED SAVED_PATH PATH=%SAVED_PATH%
SET SAVED_PATH=%PATH%
PATH=%TRESOS_BASE%\lib;%PATH%

REM COMMANDLINE
SET CLASSPATH=%TRESOS_BASE%\plugins\org.eclipse.equinox.launcher_1.3.201.v20161025-1711.jar
SET APPLICATION=dreisoft.tresos.generator.ng.api.plugin.jetcompiler
SET VMARGS=-server -Dbase.path=%TRESOS_BASE% -Djava.library.path=%TRESOS_BASE%\lib
SET ARGS=-nl en -install %TRESOS_BASE%
SET LAUNCHER=org.eclipse.equinox.launcher.Main
SET CMD=%JAVA% %VMARGS% -classpath %CLASSPATH% %LAUNCHER% -application %APPLICATION% %ARGS% %*

REM Run java
IF NOT EXIST %JAVA% GOTO NOJAVA
%CMD%

IF %ERRORLEVEL%==0 goto END0
IF %ERRORLEVEL%==1 goto END1

echo Unexpected error-code: %ERRORLEVEL%
