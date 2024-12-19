set VSSDKDIR=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133
set DX8SDKDIR=E:\Git\oldCorumMMO\directx8-msdx8
set WINSDKDIR=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A
set UCRTSDKDIR_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x86
set SOLUTION_PLATFORM_TOOLSET=v142


set SOLUTION_INCLUDES=%WINSDKDIR%\include;%VSSDKDIR%\include;%VSSDKDIR%\atlmfc\include;%DX8SDKDIR%\include;
set SOLUTION_LIBS=%WINSDKDIR%\Lib;%DX8SDKDIR%\lib;%VSSDKDIR%\lib\x86;%VSSDKDIR%\atlmfc\lib\x86;%WINSDKDIR%\lib;%UCRTSDKDIR_LIB%;