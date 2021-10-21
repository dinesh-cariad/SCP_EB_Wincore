rem
rem Retrieve the JAVA within EB tresos Studio installation
rem
IF EXIST "%CURRFOLDER%\..\..\jre\bin\java.exe" (
SET JAVA="%CURRFOLDER%\..\..\jre\bin\java.exe"
) ELSE IF EXIST "%CURRFOLDER%\..\..\jre_x86_64\win32\bin\java.exe" (
SET JAVA="%CURRFOLDER%\..\..\jre_x86_64\win32\bin\java.exe"
) ELSE (
rem No JAVA was found in EB tresos Studio.
rem Please uncomment the line below to provide your own path to java.exe in any of your JAVA installations
rem SET JAVA = "<ProvideYourPathToJavaExeHere>"
)
IF NOT EXIST "%JAVA%" (
echo Please set path to JAVA in file %~f0
SET STATUS=1
)
