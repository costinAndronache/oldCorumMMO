chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying DungeonServer executable"

XCOPY /Y /B "..\Corum Online SRC\DungeonServer\Debug\DungeonServerResult.exe" "%CENTRALIZED_OUTPUT_DIR%\DungeonServerResult.exe*"
echo "copying done"

.\installProducts.bat