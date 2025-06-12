chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying BaseNetwork artifacts"

XCOPY /Y /B "..\Corum Online SRC\[Lib]BaseNetwork\Debug\BaseNetwork.pdb" "%CENTRALIZED_OUTPUT_DIR%\BaseNetwork.pdb*"
XCOPY /Y /B "..\Corum Online SRC\[Lib]BaseNetwork\Debug\BaseNetwork.lib" "%CENTRALIZED_OUTPUT_DIR%\BaseNetwork.lib*"
XCOPY /Y /B "..\Corum Online SRC\[Lib]BaseNetwork\Debug\BaseNetwork.dll" "%CENTRALIZED_OUTPUT_DIR%\BaseNetwork.dll*"

echo "copying done"

.\installProducts.bat