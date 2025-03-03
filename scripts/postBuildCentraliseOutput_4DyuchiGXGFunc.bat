chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying 4DyuchiGXGFunc artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGFunc\Debug\SS3DGFunc.pdb" "%CENTRALIZED_OUTPUT_DIR%\SS3DGFunc.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGFunc\Debug\SS3DGFunc.lib" "%CENTRALIZED_OUTPUT_DIR%\SS3DGFunc.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGFunc\Debug\SS3DGFunc.dll" "%CENTRALIZED_OUTPUT_DIR%\SS3DGFunc.dll*"

echo "copying done"

.\installProducts.bat