chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying CorumPreferences artifacts" 
XCOPY /i /Y /B "..\Corum Online SRC\CorumPreferences\Debug\CorumPreferences.lib" "%CENTRALIZED_OUTPUT_DIR%\CorumPreferences.lib*"
XCOPY /i /Y /B "..\Corum Online SRC\CorumPreferences\Debug\CorumPreferences.pdb" "%CENTRALIZED_OUTPUT_DIR%\CorumPreferences.pdb*"

echo "copying done"

.\installProducts.bat