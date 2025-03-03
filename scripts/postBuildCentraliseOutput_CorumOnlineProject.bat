chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying client executable"

XCOPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.exe" "%CENTRALIZED_OUTPUT_DIR%\CorumOnlineResult.exe*"

echo "copying done"

.\installProducts.bat