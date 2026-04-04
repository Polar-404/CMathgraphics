@echo off
setlocal

echo Compiling main.c...

gcc main.c tokenize.c rpn.c -o app.exe -lm

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error while compiling!
) ELSE (
    echo.
    echo Successfully compiled!
    echo Running app.exe...
    app.exe
)

pause