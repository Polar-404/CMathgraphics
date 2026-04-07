@echo off
cmake -S . -B build
cmake --build build
if %errorlevel% neq 0 (
    echo Build falhou!
    pause
    exit /b 1
)

cd build
math_test.exe