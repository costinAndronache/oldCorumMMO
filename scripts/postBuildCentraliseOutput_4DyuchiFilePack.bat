call .\setDirectoryVariables.bat
echo "Begin copying 4DyuchiFilePack artifacts"

XCOPY /Y /B "%~dp0\..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFilePack\Debug\4DyuchiFilePack.exe" "%CENTRALIZED_OUTPUT_DIR%\4DyuchiFilePack.exe*"
XCOPY /Y /B "%~dp0\..\Corum Online SRC\SS3D_0719\4DYuchiGX\4DyuchiFilePack\Debug\4DyuchiFilePack.exe" "%CENTRALIZED_OUTPUT_DIR%\4DyuchiFilePack.exe*"

echo "copying done"

%~dp0\installProducts.bat