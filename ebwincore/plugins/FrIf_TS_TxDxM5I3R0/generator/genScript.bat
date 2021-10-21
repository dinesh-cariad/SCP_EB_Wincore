@echo off

IF DEFINED FR_COVERAGE_MCG (
    SET externalGenerator=%FR_COVERAGE_MCG%/FrIf/DebugCoverage/FrIf.exe
    echo FR_ERAY_GEN_PATH was defined. Using alternative path %FR_COVERAGE_MCG%/FrIf/DebugCoverage/FrIf.exe
) ELSE (
    set externalGenerator=%1\generator\windows\FrIf_MCG.exe
)

set outputDir=%2

set moduleConfigs=
:filter_modules
if "%3"=="" (goto call_generator)
for %%i in (Fr,Fr1,FrIf,Dem,FrNm,FrTp,PduR,EcuC,FrTrcv,FrTrcv1) do (
echo.%3 > %outputDir%\FrIf_moduleConfigFilter_helper.txt
findstr /r /c:"^%%i_" %outputDir%\FrIf_moduleConfigFilter_helper.txt >nul && set moduleConfigs=%moduleConfigs% -c %outputDir%\%%i.epc
del %outputDir%\FrIf_moduleConfigFilter_helper.txt
)
shift /3
goto filter_modules


:call_generator
mkdir %outputDir%\dummy_dir_exec_gen_out
mkdir %outputDir%\src
mkdir %outputDir%\include
mkdir %outputDir%\make
mkdir %outputDir%\xgen
call %outputDir%\config\FrIf_ExtGenCfg.bat
%externalGenerator% %variantParameter% %moduleConfigs% -o %outputDir%\dummy_dir_exec_gen_out
move %outputDir%\dummy_dir_exec_gen_out\*.c %outputDir%\src
move %outputDir%\dummy_dir_exec_gen_out\*.h %outputDir%\include
move %outputDir%\dummy_dir_exec_gen_out\*.xgen %outputDir%\xgen
move %outputDir%\xgen\FrIf.xgen %outputDir%\xgen\FrIf.xgen
rmdir %outputDir%\dummy_dir_exec_gen_out

