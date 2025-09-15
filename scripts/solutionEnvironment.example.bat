set VSSDKDIR=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.29.30133
set DX8SDKDIR=E:\Git\oldCorumMMO\directx8-msdx8\8.1
set WINSDKDIR=C:\Program Files\Microsoft SDKs\Windows\v7.1
set UCRTSDKDIR_LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x86
set SOLUTION_PLATFORM_TOOLSET=v143

set DX8INCLUDEDIR=%DX8SDKDIR%\include
set DX8LIBDIR=%DX8SDKDIR%\lib
set SOLUTION_INCLUDES=%WINSDKDIR%\include;%VSSDKDIR%\include;%VSSDKDIR%\atlmfc\include;%DX8INCLUDEDIR%;
set SOLUTION_LIBS=%WINSDKDIR%\Lib;%DX8LIBDIR%;%VSSDKDIR%\lib\x86;%VSSDKDIR%\atlmfc\lib\x86;%WINSDKDIR%\lib;%UCRTSDKDIR_LIB%;

set CPP_RUNTIME_LIB=MultiThreadedDLL
:: DirectX8.1 SDK needed; https://archive.org/details/dx81sdk_full