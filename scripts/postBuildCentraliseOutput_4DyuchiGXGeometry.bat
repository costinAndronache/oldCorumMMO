chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying SS3DGeometryForCorum artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGeometry\Debug\SS3DGeometryForCorum.pdb" "%CENTRALIZED_OUTPUT_DIR%\SS3DGeometryForCorum.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGeometry\Debug\SS3DGeometryForCorum.lib" "%CENTRALIZED_OUTPUT_DIR%\SS3DGeometryForCorum.lib*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiGXGeometry\Debug\SS3DGeometryForCorum.dll" "%CENTRALIZED_OUTPUT_DIR%\SS3DGeometryForCorum.dll*"

echo "copying done"

.\installProducts.bat