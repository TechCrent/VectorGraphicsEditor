@echo off
echo Deploying Vector Graphics Editor for Windows...

REM Check if executable exists
if not exist "build\VectorGraphicsEditor.exe" (
    echo Error: Executable not found. Please build the project first.
    pause
    exit /b 1
)

REM Create deployment directory
if not exist "deploy" mkdir deploy
cd deploy

REM Copy executable
copy "..\build\VectorGraphicsEditor.exe" "VECTOR GRAPHICS EDITOR.exe"
if %errorlevel% neq 0 (
    echo Error: Could not copy executable.
    pause
    exit /b 1
)

REM Find Qt installation path
for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_BINS') do set QT_BIN=%%i
echo Qt binaries found at: %QT_BIN%

REM Copy required Qt DLLs
echo Copying Qt DLLs...
copy "%QT_BIN%\Qt5Core.dll" .
copy "%QT_BIN%\Qt5Gui.dll" .
copy "%QT_BIN%\Qt5Widgets.dll" .

REM Copy platform plugin
if not exist "platforms" mkdir platforms
copy "%QT_BIN%\..\plugins\platforms\qwindows.dll" "platforms\"

REM Copy style plugins
if not exist "styles" mkdir styles
copy "%QT_BIN%\..\plugins\styles\qwindowsvistastyle.dll" "styles\"

REM Copy image format plugins
if not exist "imageformats" mkdir imageformats
copy "%QT_BIN%\..\plugins\imageformats\qico.dll" "imageformats\"
copy "%QT_BIN%\..\plugins\imageformats\qjpeg.dll" "imageformats\"
copy "%QT_BIN%\..\plugins\imageformats\qpng.dll" "imageformats\"

REM Copy Cairo DLLs (if using MSYS2)
where pkg-config >nul 2>nul
if %errorlevel% equ 0 (
    for /f "tokens=*" %%i in ('pkg-config --variable=prefix cairo') do set CAIRO_PREFIX=%%i
    if exist "%CAIRO_PREFIX%\bin\libcairo-2.dll" (
        copy "%CAIRO_PREFIX%\bin\libcairo-2.dll" .
        echo Copied Cairo DLL
    )
)

REM Copy Libxml2 DLLs (if using MSYS2)
if exist "%CAIRO_PREFIX%\bin\libxml2-2.dll" (
    copy "%CAIRO_PREFIX%\bin\libxml2-2.dll" .
    echo Copied Libxml2 DLL
)

REM Copy MinGW runtime DLLs
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    for /f "tokens=*" %%i in ('gcc -print-file-name=libgcc_s_seh-1.dll') do copy "%%i" .
    for /f "tokens=*" %%i in ('gcc -print-file-name=libstdc++-6.dll') do copy "%%i" .
    for /f "tokens=*" %%i in ('gcc -print-file-name=libwinpthread-1.dll') do copy "%%i" .
    echo Copied MinGW runtime DLLs
)

REM Create a simple launcher script
echo @echo off > "Launch Vector Graphics Editor.bat"
echo cd /d "%%~dp0" >> "Launch Vector Graphics Editor.bat"
echo start "" "VECTOR GRAPHICS EDITOR.exe" >> "Launch Vector Graphics Editor.bat"

echo.
echo Deployment completed successfully!
echo.
echo Files in deploy directory:
dir /b
echo.
echo To run the application:
echo 1. Double-click "VECTOR GRAPHICS EDITOR.exe"
echo 2. Or double-click "Launch Vector Graphics Editor.bat"
echo.
echo You can now copy the entire 'deploy' folder to any Windows computer
echo and run the application without installing additional dependencies.
echo.
pause 