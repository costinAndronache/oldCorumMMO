chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying 4DyuchiGXMapEditor artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXMapEditor\Debug\4DyuchiGXMapEditor.pdb" "%CENTRALIZED_OUTPUT_DIR%\4DyuchiGXMapEditor.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXMapEditor\Debug\4DyuchiGXMapEditor.exe" "%CENTRALIZED_OUTPUT_DIR%\4DyuchiGXMapEditor.exe*"

echo "copying done"

.\installProducts.bat