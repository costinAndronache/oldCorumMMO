chdir %~dp0
echo "Begin copying 4DyuchiGXGeometry artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGeometry\Debug\4DyuchiGXGeometry.pdb" "..\Corum Online SRC\centralizedOutput\4DyuchiGXGeometry.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGeometry\Debug\4DyuchiGXGeometry.lib" "..\Corum Online SRC\centralizedOutput\4DyuchiGXGeometry.lib*"

echo "copying done"

.\installProducts.bat