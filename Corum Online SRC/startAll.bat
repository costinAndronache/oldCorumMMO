@ECHO ON

CD ".\INNER"

START "LOGIN-AGENT" /D ".\1_LoginAgent Configurations_INNER" ".\1_LoginAgent Configurations_INNER\LoginAgentResult.exe"

timeout /t 2

START "WORLD-SERVER" /D ".\2_WorldServer Configurations\" ".\2_WorldServer Configurations\WorldServerResult.exe"

timeout /t 2

START "BEYONDE" /D ".\3_DungeonServer Configurations_Beyonde" ".\3_DungeonServer Configurations_Beyonde\DungeonServerResult.exe"

timeout /t 3