chdir %~dp0
call .\setDirectoryVariables.bat

echo "Begin copying 4DyuchiFileStorage artifacts"


XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFileStorage\Debug\SS3DFileStorage.lib" "%CENTRALIZED_OUTPUT_DIR%\SS3DFileStorage.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFileStorage\Debug\SS3DFileStorage.pdb" "%CENTRALIZED_OUTPUT_DIR%\SS3DFileStorage.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFileStorage\Debug\SS3DFileStorage.dll" "%CENTRALIZED_OUTPUT_DIR%\SS3DFileStorage.dll*"

echo "copying done"

.\installProducts.bat