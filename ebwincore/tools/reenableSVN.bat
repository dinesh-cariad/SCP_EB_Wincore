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
  ECHO ERROR: Could not find backup, exiting!
  EXIT /B 1
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
  if not exist %TRESOS_BASE%\plugins_backup\%%p echo Missing backup for plugin %%p !?
  
  rem copy from backup (see disableSVN.bat), keep the backup
  copy %TRESOS_BASE%\plugins_backup\%%p %TRESOS_BASE%\plugins\ 1>nul 2>&1
)
   
ECHO Backup restored, SVN support is enabled.
EXIT /B 0
