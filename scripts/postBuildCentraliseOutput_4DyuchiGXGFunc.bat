chdir %~dp0
echo "Begin copying 4DyuchiGXGFunc artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGFunc\Debug\4DyuchiGXGFunc.pdb" "..\Corum Online SRC\centralizedOutput\4DyuchiGXGFunc.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGFunc\Debug\4DyuchiGXGFunc.lib" "..\Corum Online SRC\centralizedOutput\4DyuchiGXGFunc.lib*"

echo "copying done"

.\installProducts.bat