chdir %~dp0
call .\setDirectoryVariables.bat


echo "Begin copying loginAgent executable"

XCOPY /Y /B "..\Corum Online SRC\LoginAgent\Debug\LoginAgentResult.exe" "%CENTRALIZED_OUTPUT_DIR%\LoginAgentResult.exe*"

echo "copying done"

.\installProducts.bat