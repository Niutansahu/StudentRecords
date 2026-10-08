@echo off
REM Builds student_record_system.c and runs the program.
REM Works from any folder: the script always uses its own location.

cd /d "%~dp0"

gcc -Wall -Wextra -g -o student_records.exe student_record_system.c
if errorlevel 1 (
    echo.
    echo Build failed. Check the errors above.
    pause
    exit /b 1
)

echo.
echo Build succeeded. Starting program...
echo.
student_records.exe
pause
