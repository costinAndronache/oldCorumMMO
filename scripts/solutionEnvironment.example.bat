set VSSDKDIR=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133
set VS2010SDKDIR=E:\Program Files (x86)\Microsoft Visual Studio 10.0\VC
set DX8SDKDIR=E:\Git\oldCorumMMO\directx8-msdx8
set WINSDKDIR=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A

set SOLUTION_INCLUDES=%WINSDKDIR%\include;%VS2010SDKDIR%\include;%VSSDKDIR%\include;%VSSDKDIR%\atlmfc\include;%DX8SDKDIR%\include;
set SOLUTION_LIBS=%WINSDKDIR%\Lib;%DX8SDKDIR%\lib;%VS2010SDKDIR%\lib;%VSSDKDIR%\lib\x86;%VSSDKDIR%\atlmfc\lib\x86;%WINSDKDIR%\lib;