@echo off
setlocal

cd /d "%~dp0"

if not exist build mkdir build

g++ -std=gnu++17 -Iinclude src\main.cpp src\GirafaRosaPink.cpp -o build\VCS_alpha.exe
if errorlevel 1 (
    echo.
    echo Falha na compilacao.
    exit /b 1
)

build\VCS_alpha.exe
