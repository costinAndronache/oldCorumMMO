chdir %~dp0
echo "Begin copying BaseLibrary executable"
dir 
XCOPY /i /Y /B "..\Corum Online SRC\BaseLibrary\Debug\BaseLibrary.lib" "..\Corum Online SRC\centralizedOutput\BaseLibrary\lib\BaseLibrary.lib*"

echo "copying done"