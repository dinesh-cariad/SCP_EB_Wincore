@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

SET CHECK_INTEGRITY=%CURRFOLDER%\md5deep.exe
SET XFRM_CLASS_PATH=%CP%;%CURRFOLDER%\XfrmCV\antlr-4.7-complete.jar;%CURRFOLDER%\XfrmCV\jcommander-1.72.jar;%CURRFOLDER%\XfrmCV\sqlite-jdbc-3.19.3.jar;%CURRFOLDER%\XfrmCV\XfrmCheck.jar;

rem overall status
SET STATUS=0

rem this retrieves the environment variables the customer may set individually
call %CURRFOLDER%\setEnvVars.bat
if %STATUS% NEQ 0 (
    @echo Java not found, exiting checker...
    exit /b %errorlevel%
)

rem check integrity of %CURRFOLDER% and all subfolders of %CURRFOLDER%
for /r %CURRFOLDER% %%F in (.) do call :checkIntegrity %%F

if %STATUS% EQU 0 (
@echo Integrity check passed
@echo calling checker...
%JAVA% -cp %XFRM_CLASS_PATH% %APP% %*
SET STATUS=!errorlevel!
) else (
@echo Did not invoke checker since integrity check failed
)
exit /b %STATUS%

rem
rem Checks the integrity of all files contained in a handed over folder
rem if the value of STATUS is 0
rem
:checkIntegrity
if %STATUS% EQU 0 (
    pushd %1
    @echo Checking integrity of folder %1
    for %%F in (.\*) do call :checkIntegrityForFile .\hashes.txt .\setEnvVars.bat %%F
    popd
    if %errorlevel% NEQ 0 (
        @echo Checking integrity of folder %1 failed, error code: %errorlevel%
        SET STATUS=%errorlevel%
    )
)
exit /b %errorlevel%

rem
rem Checks the integrity of a handed over file using a handed over file containing hashes for the expected files
rem If the hashsum for the given file was not found, or the expected hashsum differs from the actual testsum,
rem STATUS is set to a value different to 0
rem 
rem param1 path to hash file
rem param2 path to file the integrity of which is checked
rem
:checkIntegrityForFile
SET UNKNOWN_HASH_FILE=
if %STATUS% EQU 0 (
    if %1 NEQ %3 (
        if %2 NEQ %3 (
            @echo checking integrity of file %3...
            for /f %%i in ('%CHECK_INTEGRITY% -s -x %1 %3') do set UNKNOWN_HASH_FILE=%%i
        )
    )
)
rem check whether the hashsums were different
if %errorlevel% NEQ 0 (
    @echo Error: Verification of hashsum failed for file %3.
    SET STATUS=%errorlevel%
) else (
    rem check whether the file/hashsum of the file to check was found in the hash file
    if not "%UNKNOWN_HASH_FILE%"=="" (
       @echo Error: Did not find hashsum of file %3. 
       SET STATUS=42
    )
)
exit /b %errorlevel%

