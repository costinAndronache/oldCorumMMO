chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying BaseLibrary executable" 
XCOPY /i /Y /B "..\Corum Online SRC\BaseLibrary\Debug\BaseLibrary.dll" "%CENTRALIZED_OUTPUT_DIR%\BaseLibrary.dll*"
XCOPY /i /Y /B "..\Corum Online SRC\BaseLibrary\Debug\BaseLibrary.lib" "%CENTRALIZED_OUTPUT_DIR%\BaseLibrary.lib*"
XCOPY /i /Y /B "..\Corum Online SRC\BaseLibrary\Debug\BaseLibrary.pdb" "%CENTRALIZED_OUTPUT_DIR%\BaseLibrary.pdb*"

echo "copying done"

.\installProducts.bat