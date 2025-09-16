@ECHO ON

CD ".\INNER"

START "LOGIN-AGENT" /D ".\1_LoginAgent Configurations_INNER" ".\1_LoginAgent Configurations_INNER\LoginAgentResult.exe"

timeout /t 2

START "WORLD-SERVER" /D ".\2_WorldServer Configurations\" ".\2_WorldServer Configurations\WorldServerResult.exe"

timeout /t 2

START "DUNGEONS" /D ".\3_DungeonServer Configurations_DUNGEONS" ".\3_DungeonServer Configurations_DUNGEONS\DungeonServerResult.exe"

timeout /t 2

START "PASSAGES" /D ".\3_DungeonServer Configurations_PASSAGES" ".\3_DungeonServer Configurations_PASSAGES\DungeonServerResult.exe"

timeout /t 2

exit


START "VILLAGES" /D ".\3_DungeonServer Configurations_VILLAGES" ".\3_DungeonServer Configurations_VILLAGES\DungeonServerResult.exe"


timeout /t 2

START "SIEGE1" /D ".\3_DungeonServer Configurations_SIEGE1" ".\3_DungeonServer Configurations_SIEGE1\DungeonServerResult.exe"


timeout /t 2

START "SIEGE2" /D ".\3_DungeonServer Configurations_SIEGE2" ".\3_DungeonServer Configurations_SIEGE2\DungeonServerResult.exe"


timeout /t 2

START "SIEGE3" /D ".\3_DungeonServer Configurations_SIEGE3" ".\3_DungeonServer Configurations_SIEGE3\DungeonServerResult.exe"


timeout /t 3