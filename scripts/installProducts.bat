chdir %~dp0
echo "Begin copying WorldServer executable"

COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\Installed Client\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\1_LoginAgent Configurations_INNER\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\2_WorldServer Configurations\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\3_DungeonServer Configurations_Beyonde\CommonServer.dll"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\INNER\3_DungeonServer Configurations_Ludilos\CommonServer.dll"

COPY /Y /B "..\Corum Online SRC\centralizedOutput\CorumOnlineResult.exe" "..\Corum Online SRC\Installed Client\CorumOnlineResult.exe"
COPY /Y /B "..\Corum Online SRC\centralizedOutput\CommonServer.dll" "..\Corum Online SRC\Installed Client\CommonServer.dll"



echo "copying done"