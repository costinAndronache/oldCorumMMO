chdir %~dp0
echo "Begin copying CommonServer library"
dir 
COPY /Y /B "..\Corum Online SRC\CommonServer\Debug\CommonServer.dll" "..\Corum Online SRC\centralizedOutput\CommonServer.dll"

echo "copying done"