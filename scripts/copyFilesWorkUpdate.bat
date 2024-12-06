echo "Copy files"

dir /s /b /o:gn %1 > ".\filesList.txt"

findstr /I /R "\.h$ \.cpp$" .\filesList.txt > ".\filteredFiles.txt"

FOR /f "tokens=1*delims=:" %%i in (.\filteredFiles.txt) do (
    echo "File is: %%j"
    echo %%j > .\fixedFiles.txt

    echo "contents of fixedFiles.txt:"
    type .\fixedFiles.txt

    FOR /F "tokens=*" %%k in (.\fixedFiles.txt) do (
       echo "Fixed is: %%k"
       echo "copying %%j to %2\%%~nxk"
       XCOPY /i /Y /B "%%j" "%2\%%~nxk*"
    )
)




