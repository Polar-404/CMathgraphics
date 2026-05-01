@echo off
cmake -S . -B build
cmake --build build
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

cd build
math_test.exe