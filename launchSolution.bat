setlocal ENABLEDELAYEDEXPANSION

call .\scripts\setDirectoryVariables.bat
call .\scripts\solutionEnvironment.bat

echo %VSSDKDIR%
echo "%SOLUTION_INCLUDES%"

call ".\Corum Online SRC\CorumOnlineProject\CorumOnlineProject.sln"

endlocal