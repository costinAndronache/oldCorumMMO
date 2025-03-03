chdir %~dp0
echo "Begin copying 4DyuchiGXGFunc artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFileStorage\Debug\4DyuchiFileStorage.lib" "..\Corum Online SRC\centralizedOutput\4DyuchiFileStorage.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFileStorage\Debug\4DyuchiFileStorage.pdb" "..\Corum Online SRC\centralizedOutput\4DyuchiFileStorage.pdb*"

echo "copying done"

.\installProducts.bat