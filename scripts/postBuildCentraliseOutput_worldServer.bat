chdir %~dp0
echo "Begin copying WorldServer executable"

XCOPY /Y /B "..\Corum Online SRC\WorldServer\Debug\WorldServerResult.exe" "..\Corum Online SRC\centralizedOutput\WorldServerResult.exe*"

echo "copying done"

.\installProducts.bat