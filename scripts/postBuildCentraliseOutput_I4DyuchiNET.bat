chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying I4DyuchiINET library"

XCOPY /i /Y /B "..\Corum Online SRC\I4DyuchiNET\Debug\I4DyuchiNET.lib" "%CENTRALIZED_OUTPUT_DIR%\I4DyuchiNET.lib*"

echo "copying done"

.\installProducts.bat