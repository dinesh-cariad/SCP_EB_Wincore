@ECHO OFF
REM
REM This file is a wrapper for the call to the E2EPWCheck Tool with
REM the following two parameters:
REM    %1 ... specifies the Studio plugin path
REM    %2 ... specifies the output path for the generated files for this project
REM The Verification Report will be generated at: %2\E2EPWCheck.report
REM
REM This file calls ExecuteE2EPWCheckWrapper.bat generated within
REM a specific EB tresos Studio project.
REM
REM If generation of ExecuteE2EPWCheckWrapper.bat is skipped, then 
REM calling this wrapper file does nothing.
REM
SET StudioPluginPath=%1
SET ProjectGenOutputPath=%2
%ProjectGenOutputPath%\make\ExecuteE2EPWCheckWrapper.bat %StudioPluginPath% %ProjectGenOutputPath%