chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying CommonServer library"
XCOPY /Y /B "..\Corum Online SRC\CommonServer\Debug\CommonServer.dll" "%CENTRALIZED_OUTPUT_DIR%\CommonServer.dll*"
XCOPY /Y /B "..\Corum Online SRC\CommonServer\Debug\CommonServer.lib" "%CENTRALIZED_OUTPUT_DIR%\CommonServer.lib*"
echo "copying done"

.\installProducts.bat