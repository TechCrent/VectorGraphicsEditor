@echo off
echo Building Vector Graphics Editor for Windows...

REM Check if CMake is available
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: CMake not found. Please install CMake and add it to PATH.
    pause
    exit /b 1
)

REM Check if Qt5 is available
where qmake >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: Qt5 not found. Please install Qt5 and add qmake to PATH.
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo Error: CMake configuration failed.
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Error: Build failed.
    pause
    exit /b 1
)

REM Check if executable was created
if not exist "VectorGraphicsEditor.exe" (
    echo Error: Executable not found after build.
    pause
    exit /b 1
)

echo Build completed successfully!
echo Executable location: build\VectorGraphicsEditor.exe

REM Create a copy with the desired name
copy "VectorGraphicsEditor.exe" "VECTOR GRAPHICS EDITOR.exe"
if %errorlevel% equ 0 (
    echo Created: VECTOR GRAPHICS EDITOR.exe
) else (
    echo Warning: Could not create copy with spaces in name.
)

echo.
echo To run the application, double-click on "VECTOR GRAPHICS EDITOR.exe"
echo.
pause 