chdir %~dp0

call .\setDirectoryVariables.bat


echo "Begin copying client executable"

XCOPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.exe" "%CENTRALIZED_OUTPUT_DIR%\CorumOnlineResult.exe*"
XCOPY /Y /B "..\Corum Online SRC\CorumOnlineProject\Debug\CorumOnlineResult.pdb" "%CENTRALIZED_OUTPUT_DIR%\CorumOnlineResult.pdb*"

XCOPY /Y /B "%LIBRARIES_DIR%\SoundLib.dll" "%CENTRALIZED_OUTPUT_DIR%\SoundLib.dll*"
XCOPY /Y /B "%LIBRARIES_DIR%\mss32.dll" "%CENTRALIZED_OUTPUT_DIR%\mss32.dll*"
echo "copying done"

.\installProducts.bat