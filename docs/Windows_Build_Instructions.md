# Windows Build Instructions for Vector Graphics Editor

This guide will help you create a standalone executable file called "VECTOR GRAPHICS EDITOR.exe" that can be run with a double-click on any Windows computer.

## Prerequisites

### 1. Install Required Software

#### Option A: Using MSYS2 (Recommended)
1. **Download and install MSYS2** from https://www.msys2.org/
2. **Open MSYS2 MinGW 64-bit terminal** and install packages:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-cmake
   pacman -S mingw-w64-x86_64-qt5-base
   pacman -S mingw-w64-x86_64-cairo
   pacman -S mingw-w64-x86_64-libxml2
   pacman -S mingw-w64-x86_64-pkg-config
   ```

#### Option B: Using Qt Installer
1. **Download Qt5** from https://www.qt.io/download
2. **Install Qt5** with MinGW compiler
3. **Install MSYS2** for Cairo and Libxml2 dependencies

### 2. Add to PATH
Add the following to your Windows PATH environment variable:
- `C:\msys64\mingw64\bin` (if using MSYS2)
- `C:\Qt\5.15.2\mingw81_64\bin` (adjust path to your Qt installation)

## Building the Executable

### Method 1: Using the Build Script (Easiest)

1. **Double-click `build_windows.bat`**
   - This script will automatically check for dependencies
   - Build the project
   - Create the executable

2. **Check the output**
   - The executable will be created as `build\VECTOR GRAPHICS EDITOR.exe`
   - You can double-click this file to run the application

### Method 2: Manual Build

1. **Open Command Prompt** in the project directory

2. **Create build directory**:
   ```cmd
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```cmd
   cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
   ```

4. **Build the project**:
   ```cmd
   cmake --build . --config Release
   ```

5. **Find the executable**:
   - It will be located at `build\VECTOR GRAPHICS EDITOR.exe`

## Creating a Standalone Package

To create a package that can run on any Windows computer without installing dependencies:

### Method 1: Using the Deploy Script

1. **First build the project** using the build script
2. **Double-click `deploy_windows.bat`**
3. **Check the `deploy` folder** - it will contain:
   - `VECTOR GRAPHICS EDITOR.exe`
   - All required DLL files
   - Plugin folders (platforms, styles, imageformats)
   - `Launch Vector Graphics Editor.bat` (alternative launcher)

### Method 2: Manual Deployment

1. **Copy the executable** to a new folder
2. **Copy required DLLs**:
   - Qt5Core.dll, Qt5Gui.dll, Qt5Widgets.dll
   - libcairo-2.dll, libxml2-2.dll
   - MinGW runtime DLLs (libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll)

3. **Copy Qt plugins**:
   - `platforms/qwindows.dll`
   - `styles/qwindowsvistastyle.dll`
   - `imageformats/qico.dll`, `qjpeg.dll`, `qpng.dll`

## Running the Application

### Option 1: Direct Execution
- **Double-click** `VECTOR GRAPHICS EDITOR.exe`

### Option 2: Using the Launcher
- **Double-click** `Launch Vector Graphics Editor.bat`

### Option 3: Command Line
```cmd
cd path\to\deploy\folder
VECTOR GRAPHICS EDITOR.exe
```

## Troubleshooting

### Common Issues

1. **"Qt5 not found" error**:
   - Ensure Qt5 is installed and qmake is in PATH
   - Try: `qmake -v` to verify installation

2. **"Cairo not found" error**:
   - Install Cairo via MSYS2: `pacman -S mingw-w64-x86_64-cairo`
   - Ensure pkg-config is installed

3. **"Libxml2 not found" error**:
   - Install Libxml2 via MSYS2: `pacman -S mingw-w64-x86_64-libxml2`

4. **"Missing DLL" errors when running**:
   - Use the deploy script to bundle all dependencies
   - Ensure all DLLs are in the same folder as the executable

5. **Build fails with compiler errors**:
   - Ensure you're using a C++17 compatible compiler
   - Try updating MinGW-w64 to the latest version

### Verification Steps

1. **Check dependencies**:
   ```cmd
   qmake -v
   gcc --version
   cmake --version
   pkg-config --version
   ```

2. **Test Qt installation**:
   ```cmd
   qmake -query QT_INSTALL_BINS
   ```

3. **Test Cairo installation**:
   ```cmd
   pkg-config --modversion cairo
   ```

## Distribution

Once you have the `deploy` folder with all files:

1. **Zip the entire `deploy` folder**
2. **Share the zip file** - recipients can extract and run without installation
3. **The application will work on any Windows 7+ computer**

## File Structure After Deployment

```
deploy/
├── VECTOR GRAPHICS EDITOR.exe
├── Launch Vector Graphics Editor.bat
├── Qt5Core.dll
├── Qt5Gui.dll
├── Qt5Widgets.dll
├── libcairo-2.dll
├── libxml2-2.dll
├── libgcc_s_seh-1.dll
├── libstdc++-6.dll
├── libwinpthread-1.dll
├── platforms/
│   └── qwindows.dll
├── styles/
│   └── qwindowsvistastyle.dll
└── imageformats/
    ├── qico.dll
    ├── qjpeg.dll
    └── qpng.dll
```

## Notes

- The executable name with spaces (`VECTOR GRAPHICS EDITOR.exe`) is supported on Windows
- The application requires approximately 50-100MB of disk space
- All dependencies are included, making it truly portable
- The application works on Windows 7, 8, 10, and 11 