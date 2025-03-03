chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying WorldServer executable"
XCOPY /Y /B "..\Corum Online SRC\WorldServer\Debug\WorldServerResult.exe" "%CENTRALIZED_OUTPUT_DIR%\WorldServerResult.exe*"

echo "copying done"

.\installProducts.bat