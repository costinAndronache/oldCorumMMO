chdir %~dp0
echo "Begin copying DungeonServer executable"

XCOPY /Y /B "..\Corum Online SRC\DungeonServer\Debug\DungeonServerResult.exe" "..\Corum Online SRC\centralizedOutput\DungeonServerResult.exe*"

echo "copying done"

.\installProducts.bat