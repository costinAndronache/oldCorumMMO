chdir %~dp0
echo "Begin copying client executable"

COPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.exe" "..\Corum Online SRC\Installed Client\CorumOnlineResult.exe"

COPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.exe" "..\Corum Online SRC\centralizedOutput\CorumOnlineResult.exe"

echo "copying done"