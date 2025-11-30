@echo off
echo ====================================
echo Compiling Project Collision...
echo ====================================
echo.

REM 
set SFML_PATH=C:\SFML-2.5.1

REM 
echo [1/2] Compiling main.cpp...
g++ -c src/main.cpp -o build/main.o -Iinclude -I%SFML_PATH%/include
if errorlevel 1 goto error

REM Link
echo [2/2] Linking...
g++ build/main.o -o build/collision.exe -L%SFML_PATH%/lib -lsfml-graphics -lsfml-window -lsfml-system
if errorlevel 1 goto error

REM 
echo [3/3] Copying DLL files...
copy %SFML_PATH%\bin\sfml-graphics-2.dll build\ >nul
copy %SFML_PATH%\bin\sfml-window-2.dll build\ >nul
copy %SFML_PATH%\bin\sfml-system-2.dll build\ >nul
copy %SFML_PATH%\bin\sfml-audio-2.dll build\ >nul 2>&1

echo.
echo ====================================
echo Compilation SUCCESS!
echo ====================================
echo.
echo To run the program:
echo   cd build
echo   collision.exe
echo.
pause
exit

:error
echo.
echo ====================================
echo Compilation FAILED!
echo ====================================
pause
exit