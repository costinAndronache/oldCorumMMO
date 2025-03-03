chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying BaseLibrary executable"
dir 
XCOPY /i /Y /B "..\Corum Online SRC\BaseLibrary\Debug\BaseLibrary.lib" "%CENTRALIZED_OUTPUT_DIR%\BaseLibrary.lib*"

echo "copying done"

.\installProducts.bat