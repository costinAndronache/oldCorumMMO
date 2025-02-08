chdir %~dp0
echo "Begin copying CommonServer library"
dir 
XCOPY /Y /B "..\Corum Online SRC\CommonServer\Debug\CommonServer.dll" "..\Corum Online SRC\centralizedOutput\CommonServer.dll*"
XCOPY /Y /B "..\Corum Online SRC\CommonServer\Debug\CommonServer.lib" "..\Corum Online SRC\centralizedOutput\CommonServer.lib*"
echo "copying done"

.\installProducts.bat