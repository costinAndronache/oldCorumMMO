chdir %~dp0
echo "Begin copying I4DyuchiINET library"
dir 
XCOPY /i /Y /B "..\Corum Online SRC\I4DyuchiNET\Debug\I4DyuchiNET.lib" "..\Corum Online SRC\centralizedOutput\I4DyuchiNET\lib\I4DyuchiNET.lib*"

echo "copying done"

.\installProducts.bat