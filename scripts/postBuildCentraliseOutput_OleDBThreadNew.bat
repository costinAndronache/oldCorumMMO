chdir %~dp0
echo "Begin copying OleDBThreadNew library"
dir 
XCOPY /i /Y /B "..\Corum Online SRC\OleDBThreadNew\Debug\OleDBThreadNew.lib" "..\Corum Online SRC\centralizedOutput\OleDBThreadNew.lib*"

echo "copying done"

.\installProducts.bat