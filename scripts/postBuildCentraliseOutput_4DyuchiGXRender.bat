chdir %~dp0
call .\setDirectoryVariables.bat

echo "Begin copying 4DyuchiGXG_Render artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGX_RENDER\Debug\SS3DRendererForCorum.lib" "%CENTRALIZED_OUTPUT_DIR%\SS3DRendererForCorum.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGX_RENDER\Debug\SS3DRendererForCorum.pdb" "%CENTRALIZED_OUTPUT_DIR%\SS3DRendererForCorum.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGX_RENDER\Debug\SS3DRendererForCorum.dll" "%CENTRALIZED_OUTPUT_DIR%\SS3DRendererForCorum.dll*"

echo "copying done"

.\installProducts.bat