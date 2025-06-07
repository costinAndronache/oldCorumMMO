chdir %~dp0
call .\setDirectoryVariables.bat


echo "Begin copying loginAgent executable"

XCOPY /Y /B "..\Corum Online SRC\LoginAgent\Debug\LoginAgentResult.exe" "%CENTRALIZED_OUTPUT_DIR%\LoginAgentResult.exe*"
XCOPY /Y /B "%LIBRARIES_DIR%\libcomservice2.dll" "%CENTRALIZED_OUTPUT_DIR%\libcomservice2.dll*"
echo "copying done"

.\installProducts.bat