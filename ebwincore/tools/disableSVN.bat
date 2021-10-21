@echo off
SET TRESOS_BASE=%~dp0
SET BINDIR=%TRESOS_BASE:~-5%
IF NOT "%BINDIR%"=="\bin\" (
  ECHO ERROR: Please move this script to the "bin" directory of your EB tresos Studio 
  ECHO installation first, and make sure that EB tresos Studio is stopped while 
  ECHO running this script!
  EXIT /B 1
)
SET TRESOS_BASE=%TRESOS_BASE:~0,-5%

if not exist %TRESOS_BASE%\plugins_backup\ (
  mkdir %TRESOS_BASE%\plugins_backup\
)


set PLUGINS=^
  com.collabnet.subversion.merge_3.0.12.jar^
  org.tigris.subversion.clientadapter.javahl.win32_1.7.8.jar^
  org.tigris.subversion.clientadapter.javahl.win64_1.7.8.jar^
  org.tigris.subversion.clientadapter.javahl_1.7.8.1.jar^
  org.tigris.subversion.clientadapter_1.8.3.jar^
  org.tigris.subversion.subclipse.core_1.8.16.jar^
  org.tigris.subversion.subclipse.ui_1.8.18.jar

for %%p in ( %PLUGINS% ) do (
  if not exist %TRESOS_BASE%\plugins\%%p echo Missing plugin %%p !?

  rem make a backup copy
  copy %TRESOS_BASE%\plugins\%%p %TRESOS_BASE%\plugins_backup\ 1>nul 2>&1
  rem delete SVN plugins
  del /f /q %TRESOS_BASE%\plugins\%%p 1>nul
)

ECHO Backup created, SVN support is disabled.
EXIT /B 0
