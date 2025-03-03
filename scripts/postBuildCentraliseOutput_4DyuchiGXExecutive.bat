chdir %~dp0
echo "Begin copying SS3DExecutiveForCorum artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGXEXECUTIVE\Debug\4DyuchiGXExecutive.pdb" "..\Corum Online SRC\centralizedOutput\4DyuchiGXExecutive.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGXEXECUTIVE\Debug\4DyuchiGXExecutive.lib" "..\Corum Online SRC\centralizedOutput\4DyuchiGXExecutive.lib*"

echo "copying done"

.\installProducts.bat