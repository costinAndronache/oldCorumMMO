chdir %~dp0

call .\setDirectoryVariables.bat

echo "Begin copying ModelView artifacts"

XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\ModelView\Debug\ModelView.pdb" "%CENTRALIZED_OUTPUT_DIR%\ModelView.pdb*"
XCOPY /Y /B "..\Corum Online SRC\SS3D_0719\4DYuchiGX\ModelView\Debug\ModelView.exe" "%CENTRALIZED_OUTPUT_DIR%\ModelView.exe*"

echo "copying done"

.\installProducts.bat