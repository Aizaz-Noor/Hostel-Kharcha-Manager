@echo off
echo Compiling Hostel Kharcha Manager...
g++ main.cpp -o main.exe
if %errorlevel% neq 0 (
    echo Compilation failed! Please check if g++ is installed.
    pause
    exit /b %errorlevel%
)
echo Running program...
main.exe
pause
