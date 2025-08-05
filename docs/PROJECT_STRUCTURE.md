# Vector Graphics Editor - Project Structure

This document describes the organized folder structure and architecture of the Vector Graphics Editor project.

## 📁 Folder Structure

```
VectorGraphicsEditor/
├── src/                    # Source code files (.cpp)
│   ├── main.cpp           # Application entry point
│   ├── mainwindow.cpp     # Main window implementation
│   ├── canvas.cpp         # Canvas widget implementation
│   ├── document.cpp       # Document and layer management
│   ├── svg_parser.cpp     # SVG import/export functionality
│   ├── shape.cpp          # Base shape class implementation
│   ├── rectangle.cpp      # Rectangle shape implementation
│   ├── ellipse.cpp        # Ellipse shape implementation
│   ├── line.cpp           # Line shape implementation
│   └── bezier.cpp         # Bezier curve implementation
│
├── include/               # Header files (.h)
│   ├── mainwindow.h       # Main window class declaration
│   ├── canvas.h           # Canvas widget class declaration
│   ├── document.h         # Document and layer classes
│   ├── svg_parser.h       # SVG parser class declaration
│   ├── shape.h            # Base shape class declaration
│   ├── rectangle.h        # Rectangle shape class
│   ├── ellipse.h          # Ellipse shape class
│   ├── line.h             # Line shape class
│   └── bezier.h           # Bezier curve class
│
├── ui/                    # Qt UI files (.ui)
│   └── mainwindow.ui      # Main window UI definition
│
├── lib/                   # External libraries
│   ├── cairo/             # Cairo graphics library
│   │   ├── libcairo.dll.a
│   │   └── libcairo-script-interpreter.dll.a
│   └── libxml2/           # Libxml2 library
│       └── libxml2.dll.a
│
├── docs/                  # Documentation
│   ├── README.md          # Main project documentation
│   ├── Windows_Build_Instructions.md
│   └── PROJECT_STRUCTURE.md
│
├── tests/                 # Test files and build scripts
│   ├── build_windows.bat  # Windows build script
│   ├── deploy_windows.bat # Windows deployment script
│   ├── test-qt.cpp        # Basic Qt test
│   └── test-qt-integration.cpp
│
├── resources/             # Application resources (empty)
├── build/                 # Build artifacts (generated)
├── Reference/             # Original foreign files (preserved)
├── CMakeLists.txt         # CMake build configuration
└── CMakePresets.json      # CMake presets for different configurations
```

## 🏗️ Architecture Overview

### **Core Components**

1. **Main Application** (`src/main.cpp`)
   - Application entry point
   - Qt application initialization
   - Main window creation

2. **Main Window** (`src/mainwindow.cpp`, `include/mainwindow.h`)
   - Central application window
   - Toolbar and menu management
   - Dock widget organization
   - Signal/slot connections

3. **Canvas Widget** (`src/canvas.cpp`, `include/canvas.h`)
   - Drawing surface for vector graphics
   - User interaction handling
   - Cairo rendering integration
   - Tool state management

4. **Document Model** (`src/document.cpp`, `include/document.h`)
   - Shape storage and management
   - Layer system
   - Undo/redo functionality
   - Document state management

5. **Shape Classes** (`src/shape*.cpp`, `include/shape*.h`)
   - Base Shape class with common functionality
   - Rectangle, Ellipse, Line, Bezier implementations
   - Drawing and hit-testing methods
   - Transformation support

6. **SVG Parser** (`src/svg_parser.cpp`, `include/svg_parser.h`)
   - SVG file import/export
   - XML parsing with Libxml2
   - Shape serialization/deserialization

### **UI Architecture**

- **Qt5 Integration**: Modern Qt5-based user interface
- **UI Designer**: Visual UI definition with `mainwindow.ui`
- **Dock Widgets**: Resizable panels for layers and properties
- **Toolbars**: Quick access to common tools and actions
- **Menus**: Standard application menus (File, Edit, View, Tools)

### **Graphics Architecture**

- **Cairo Graphics**: High-quality 2D vector rendering
- **Qt Integration**: Seamless integration between Qt and Cairo
- **Coordinate Systems**: World and screen coordinate conversion
- **Zoom and Pan**: View transformation support

### **Build System**

- **CMake**: Cross-platform build configuration
- **Qt5 Integration**: AUTOMOC, AUTOUIC, AUTORCC
- **Library Linking**: Cairo and Libxml2 integration
- **Windows Support**: MinGW and MSVC configurations

## 🔧 Integration from Reference

### **Useful Components Integrated:**

1. **UI File** (`ui/mainwindow.ui`)
   - Professional UI layout
   - Canvas widget integration
   - Proper widget hierarchy

2. **CMake Presets** (`CMakePresets.json`)
   - MinGW with vcpkg configuration
   - Debug and Release presets
   - Qt6 compatibility settings

3. **Library Files** (`lib/`)
   - Cairo static libraries
   - Libxml2 static libraries
   - Windows-specific builds

4. **Test Files** (`tests/`)
   - Qt integration tests
   - Build verification scripts
   - Deployment automation

### **Architecture Improvements:**

1. **Proper Separation**: Source, headers, and UI files organized
2. **Library Integration**: Static libraries for better deployment
3. **Build Automation**: Scripts for easy building and deployment
4. **Testing**: Integration tests for verification
5. **Documentation**: Comprehensive project documentation

## 🚀 Build Process

### **Prerequisites:**
- Qt5 (Core, Widgets)
- Cairo graphics library
- Libxml2 library
- CMake 3.16+
- C++17 compatible compiler

### **Build Steps:**
1. **Configure**: `cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release`
2. **Build**: `cmake --build . --config Release`
3. **Test**: Run `tests/test-qt-integration.cpp` to verify setup
4. **Deploy**: Use `tests/deploy_windows.bat` for standalone package

## 📋 Key Features

### **Vector Graphics:**
- Rectangle, Ellipse, Line, Bezier curve creation
- Shape selection and manipulation
- Layer-based organization
- Transformations (move, scale, rotate)

### **User Interface:**
- Modern Qt5-based interface
- Dockable panels for layers and properties
- Toolbar with drawing tools
- Menu system with standard operations

### **File Operations:**
- SVG import/export
- Document save/load
- Multiple format support

### **Advanced Features:**
- Undo/redo system
- Grid and snap functionality
- Zoom and pan controls
- Property editing

## 🔄 Development Workflow

1. **Source Code**: Add new features in `src/` and `include/`
2. **UI Changes**: Modify `ui/mainwindow.ui` for interface updates
3. **Testing**: Use `tests/` for verification
4. **Documentation**: Update `docs/` with changes
5. **Build**: Use CMake for compilation
6. **Deploy**: Use deployment scripts for distribution

## 📝 Notes

- **Cross-Platform**: Designed to work on Windows, Linux, macOS
- **Modular**: Easy to extend with new shapes and tools
- **Professional**: Follows Qt and C++ best practices
- **Maintainable**: Clear separation of concerns and documentation 