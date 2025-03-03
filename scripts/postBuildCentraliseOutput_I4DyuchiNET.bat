chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying I4DyuchiINET library"

XCOPY /i /Y /B "..\Corum Online SRC\I4DyuchiNET\Debug\I4DyuchiNET.lib" "%CENTRALIZED_OUTPUT_DIR%\I4DyuchiNET.lib*"
XCOPY /i /Y /B "..\Corum Online SRC\I4DyuchiNET\Debug\I4DyuchiNET.pdb" "%CENTRALIZED_OUTPUT_DIR%\I4DyuchiNET.pdb*"

echo "copying done"

.\installProducts.bat