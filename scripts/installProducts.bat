chdir %~dp0
echo "Begin installing products executables & libraries from centralizedOutput"

COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\Installed Client\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\1_LoginAgent Configurations_INNER\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\2_WorldServer Configurations\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\3_DungeonServer Configurations_Beyonde\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\3_DungeonServer Configurations_Ludilos\CommonServer.dll"

COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\Installed Client\CommonServer.dll"

COPY /Y /B "..\Corum Online SRC\centralizedOutput\4DyuchiGXExecutive.pdb" "..\Corum Online SRC\Installed Client\4DyuchiGXExecutive.pdb"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\4DyuchiGXGFunc.pdb" "..\Corum Online SRC\Installed Client\4DyuchiGXGFunc.pdb"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\4DyuchiFileStorage.pdb" "..\Corum Online SRC\Installed Client\4DyuchiFileStorage.pdb"

COPY /Y /B "..\Corum Online SRC\centralizedOutput\LoginAgentResult.exe" "..\Corum Online SRC\INNER\1_LoginAgent Configurations_INNER\LoginAgentResult.exe"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\WorldServerResult.exe" "..\Corum Online SRC\INNER\2_WorldServer Configurations\WorldServerResult.exe"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\DungeonServerResult.exe" "..\Corum Online SRC\INNER\3_DungeonServer Configurations_Beyonde\DungeonServerResult.exe"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\DungeonServerResult.exe" "..\Corum Online SRC\INNER\3_DungeonServer Configurations_Ludilos\DungeonServerResult.exe"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CorumOnlineResult.exe" "..\Corum Online SRC\Installed Client\CorumOnlineResult.exe"

echo "copying done"
pause