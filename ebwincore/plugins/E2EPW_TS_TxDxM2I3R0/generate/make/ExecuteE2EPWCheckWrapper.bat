[!SKIPFILE "VendorSpecific/VerifyGeneratedCode!='true'"!][!//
@ECHO OFF
REM
REM If generated, this is called by the Configuration Tool EB tresos Studio
REM in order to verify the code generated for this project.
REM
REM The following parameters are provided by the Configuration Tool:
REM    %1 ... specifies the Studio plugin path
REM    %2 ... specifies the output path for the generated files for this project
REM The Verification Report will be generated at: %2\E2EPWCheck.report
REM
REM If this file is called directly (e.g. from the Windows Explorer), then
REM default values (relative paths) are assumed for these parameters with
REM the assumption that the workspace is located at:
REM <StudioPath>\workspace\<Project>\output\generated\make\ExecuteE2EPWCheckWrapper.bat
REM
IF NOT "%1"=="" (
SET StudioPluginPath=%1
SET ProjectGenOutputPath=%2
) else (
SET StudioPluginPath=..\..\..\..\..\plugins\E2EPW_TS_TxDxM2I3R0
SET ProjectGenOutputPath=..
)
%StudioPluginPath%\check\dist\E2EPWCheck.exe +%ProjectGenOutputPath%\E2EPWCheck.cfg
IF "%1"=="" (
PAUSE
)