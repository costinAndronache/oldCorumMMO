chdir %~dp0
echo "Begin copying WorldServer executable"

COPY /Y /B "..\Corum Online SRC\WorldServer\Debug\WorldServer.exe" "..\Corum Online SRC\centralizedOutput\WorldServer.exe"

echo "copying done"