chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying client pre launcher

XCOPY /Y /B "..\Corum Online SRC\CorumPreLauncher\Debug\CorumPreLauncher.exe" "%CENTRALIZED_OUTPUT_DIR%\CorumPreLauncher.exe*"
XCOPY /Y /B "..\Corum Online SRC\CorumPreLauncher\Debug\CorumPreLauncher.pdb" "%CENTRALIZED_OUTPUT_DIR%\CorumPreLauncher.pdb*"
echo "copying done"

.\installProducts.bat