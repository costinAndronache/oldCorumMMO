chdir %~dp0
echo "Begin copying loginAgent executable"

COPY /Y /B "..\Corum Online SRC\LoginAgent\Debug\LoginAgentResult.exe" "..\Corum Online SRC\centralizedOutput\LoginAgentResult.exe"

echo "copying done"