chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying SS3DExecutiveForCorum artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGXEXECUTIVE\Debug\SS3DExecutiveForCorum.pdb" "%CENTRALIZED_OUTPUT_DIR%\SS3DExecutiveForCorum.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGXEXECUTIVE\Debug\SS3DExecutiveForCorum.lib" "%CENTRALIZED_OUTPUT_DIR%\SS3DExecutiveForCorum.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DYUCHIGXEXECUTIVE\Debug\SS3DExecutiveForCorum.dll" "%CENTRALIZED_OUTPUT_DIR%\SS3DExecutiveForCorum.dll*"

echo "copying done"

.\installProducts.bat