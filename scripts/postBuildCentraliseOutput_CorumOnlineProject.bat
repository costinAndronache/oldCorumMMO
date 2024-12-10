chdir %~dp0
echo "Begin copying client executable"

XCOPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.exe" "..\Corum Online SRC\Installed Client\CorumOnlineResult.exe*"

XCOPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.exe" "..\Corum Online SRC\centralizedOutput\CorumOnlineResult.exe*"

echo "copying done"

.\installProducts.bat