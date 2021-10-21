@ECHO off

SET BASEDIR=%~dp0
SET BASEDIR=%BASEDIR:~0,-1%

SET PATH=%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem

SET VALIDATION_ERROR_FILE=%BASEDIR%\validationErrors.txt
DEL %VALIDATION_ERROR_FILE% 2> NUL

PUSHD %BASEDIR%\.. > NUL
FOR %%F IN (%BASEDIR%\*.sha1) DO (
  ECHO.
  ECHO * checking %%~nF
  %BASEDIR%\bin\sha1sum.exe -c %%F > %BASEDIR%\%%~nF.log 2>&1 && DEL %BASEDIR%\%%~nF.log 2> NUL
  IF EXIST %BASEDIR%\%%~nF.log FIND ": FAILED" %BASEDIR%\%%~nF.log >> %VALIDATION_ERROR_FILE%    
)

IF EXIST %VALIDATION_ERROR_FILE% (
  ECHO.
  ECHO ============================================================
  ECHO Checksum validation errors
  ECHO ============================================================
  TYPE %VALIDATION_ERROR_FILE%
) ELSE (
  ECHO.
  ECHO ============================================================
  ECHO All checksums are valid
  ECHO ============================================================
)
POPD > NUL
PAUSE