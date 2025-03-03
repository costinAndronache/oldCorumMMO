chdir %~dp0
echo "Begin copying 4DyuchiGXG_Render artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGX_RENDER\Debug\4DyuchiGX_Render.lib" "..\Corum Online SRC\centralizedOutput\4DyuchiGX_Render.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGX_RENDER\Debug\4DyuchiGX_Render.pdb" "..\Corum Online SRC\centralizedOutput\4DyuchiGX_Render.pdb*"

echo "copying done"

.\installProducts.bat