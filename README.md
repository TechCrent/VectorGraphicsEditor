# Vector Graphics Editor - Professional

A modern, professional vector graphics editor built with Qt6 and Cairo, featuring a sophisticated dark theme interface and comprehensive drawing tools.

![Version](https://img.shields.io/badge/version-2.0.0-blue)
![Qt](https://img.shields.io/badge/Qt-6.x%20%7C%205.x-green)
![Cairo](https://img.shields.io/badge/Cairo-graphics-orange)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

## 🚀 Features

### Professional UI Design
- **Modern Dark Theme**: Elegant dark interface with professional styling
- **Responsive Layouts**: Fully responsive design that adapts to window resizing
- **Dockable Panels**: Movable and floating layer and property panels
- **Tool Palettes**: Organized toolbars with intuitive tool selection
- **Professional Color Scheme**: Carefully chosen colors for optimal user experience

### Advanced Drawing Tools
- **Vector Shapes**: Rectangle, Ellipse, Line, and Bezier curves
- **Shape Properties**: Configurable fill colors, stroke colors, and line weights
- **Layer Management**: Multiple layers with visibility and locking controls
- **Selection Tools**: Interactive shape selection and manipulation
- **Grid System**: Optional grid display with snap-to-grid functionality

### Enhanced Graphics Engine
- **Cairo Integration**: High-quality 2D graphics rendering with Cairo
- **Anti-aliasing**: Smooth, professional-quality shape rendering
- **Alpha Channel Support**: Transparency and alpha blending
- **Memory Management**: Proper Cairo surface and context lifecycle management

### File Operations
- **SVG Import/Export**: Full SVG file format support
- **Document Management**: New, open, save document operations
- **Layer Preservation**: Maintains layer structure in saved files

## 📋 Requirements

### Build Dependencies
- **Qt6** (or Qt5 as fallback) - Core and Widgets modules
- **Cairo** - Graphics library for 2D rendering
- **CMake** 3.16 or later
- **C++17** compatible compiler

### Optional Dependencies
- **Google Test** - For running unit tests
- **pkg-config** - For better dependency detection

## 🛠️ Building

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd VectorGraphicsEditor

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make -j4

# Run the application
./VectorGraphicsEditor
```

### Advanced Build Options
```bash
# Force Qt5 usage
cmake -DQT_VERSION_MAJOR=5 ..

# Build with tests (requires Google Test)
cmake -DBUILD_TESTING=ON ..
make -j4
ctest

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Platform-Specific Instructions

#### Windows
```bash
# Using MinGW with vcpkg
cmake --preset mingw-vcpkg
cmake --build --preset mingw-vcpkg-build
```

#### Linux
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install qt6-base-dev libcairo2-dev libgtest-dev

# Configure and build
cmake ..
make -j4
```

#### macOS
```bash
# Install dependencies via Homebrew
brew install qt6 cairo googletest

# Configure and build
cmake ..
make -j4
```

## 🎨 Usage

### Getting Started
1. **Launch** the application
2. **Create** a new document or open an existing SVG file
3. **Select** a drawing tool from the left toolbar
4. **Draw** shapes on the canvas
5. **Customize** properties using the properties panel
6. **Manage** layers using the layers panel
7. **Save** your work as an SVG file

### Interface Overview

#### Main Toolbar
- **File Operations**: New, Open, Save documents
- **Edit Operations**: Undo, Redo, Cut, Copy, Paste
- **View Controls**: Zoom in/out, fit to view

#### Tools Toolbar (Left)
- **Selection Tool** (V): Select and manipulate shapes
- **Rectangle Tool** (R): Draw rectangles and squares
- **Ellipse Tool** (E): Draw circles and ellipses
- **Line Tool** (L): Draw straight lines
- **Bezier Tool** (B): Draw curved paths

#### Properties Panel (Right)
- **Fill Settings**: Color picker with alpha channel
- **Stroke Settings**: Color and width controls
- **Shape Properties**: Tool-specific options

#### Layers Panel (Right)
- **Layer List**: Visual layer management
- **Add/Remove**: Create and delete layers
- **Visibility**: Show/hide individual layers

### Keyboard Shortcuts
- `Ctrl+N` - New document
- `Ctrl+O` - Open document
- `Ctrl+S` - Save document
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste
- `Ctrl+X` - Cut
- `V` - Selection tool
- `R` - Rectangle tool
- `E` - Ellipse tool
- `L` - Line tool
- `B` - Bezier tool
- `Ctrl+=` - Zoom in
- `Ctrl+-` - Zoom out
- `Ctrl+0` - Fit to view

## 🧪 Testing

The project includes comprehensive unit tests covering:

### Test Coverage
- **Shape Classes**: Rectangle, Ellipse, Line, Bezier functionality
- **Document Management**: Layer and shape operations
- **Memory Management**: Proper cleanup and leak detection
- **Cairo Integration**: Graphics rendering validation
- **Error Handling**: Graceful handling of edge cases

### Running Tests
```bash
# Build with tests
cmake -DBUILD_TESTING=ON ..
make -j4

# Run all tests
ctest

# Run specific test
./VectorGraphicsEditorTests --gtest_filter="ShapeTest.*"

# Verbose test output
./VectorGraphicsEditorTests --gtest_verbose
```

## 🏗️ Architecture

### Project Structure
```
VectorGraphicsEditor/
├── src/               # Source files
│   ├── main.cpp       # Application entry point
│   ├── mainwindow.cpp # Main window implementation
│   ├── canvas.cpp     # Drawing canvas with Cairo
│   ├── document.cpp   # Document and layer management
│   ├── shape.cpp      # Base shape class
│   ├── rectangle.cpp  # Rectangle shape implementation
│   ├── ellipse.cpp    # Ellipse shape implementation
│   ├── line.cpp       # Line shape implementation
│   ├── bezier.cpp     # Bezier curve implementation
│   └── svg_parser.cpp # SVG import/export
├── include/           # Header files
├── ui/                # Qt UI files
│   └── mainwindow.ui  # Main window layout
├── tests/             # Unit tests
│   ├── test_shapes.cpp    # Shape class tests
│   └── test_document.cpp  # Document class tests
├── docs/              # Documentation
├── resources/         # Application resources
└── CMakeLists.txt     # Build configuration
```

### Class Hierarchy
```
QMainWindow
└── MainWindow          # Main application window

QWidget
├── Canvas              # Drawing surface with Cairo integration
├── ColorPicker         # Advanced color selection widget
├── ColorWheel          # Color wheel component
└── ColorStrip          # Color/alpha strips

Shape                   # Abstract base class
├── Rectangle           # Rectangle and rounded rectangle
├── Ellipse            # Circle and ellipse shapes
├── Line               # Straight line segments
└── Bezier             # Bezier curve paths

Document               # Document container
└── Layer              # Layer management
```

## 🔧 Configuration

### Cairo Integration
The application uses Cairo for high-quality 2D graphics rendering:
- Hardware-accelerated rendering when available
- Anti-aliased shape drawing
- Proper alpha channel support
- Memory-efficient surface management

### Qt Integration
- Modern Qt6 support with Qt5 fallback
- Responsive layout system
- Professional dark theme styling
- Cross-platform compatibility

## 🤝 Contributing

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Install development dependencies
4. Make your changes
5. Add/update tests
6. Submit a pull request

### Code Style
- Follow Qt coding conventions
- Use modern C++17 features
- Include comprehensive documentation
- Write unit tests for new features
- Maintain backward compatibility

### Reporting Issues
- Use the GitHub issue tracker
- Include system information
- Provide reproduction steps
- Include relevant logs/screenshots

## 📝 Version History

### Version 2.0.0 (Current)
- ✅ **Fixed Cairo Integration**: Proper rendering and memory management
- ✅ **Modern UI Design**: Complete interface redesign with dark theme
- ✅ **Responsive Layouts**: Adaptive layout system
- ✅ **Advanced Components**: Professional tool palettes and property panels
- ✅ **Comprehensive Testing**: Full unit test coverage
- ✅ **Improved Build System**: Cross-platform CMake configuration
- ✅ **Memory Management**: Fixed all memory leaks
- ✅ **Professional Styling**: Modern dark theme with consistent styling

### Version 1.0.0 (Previous)
- Basic vector graphics functionality
- Simple Qt interface
- Basic SVG import/export
- Limited Cairo integration

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Qt Framework** - Cross-platform application framework
- **Cairo Graphics** - 2D graphics library
- **Google Test** - Unit testing framework
- **SVG Specification** - W3C SVG standards

## 📞 Support

For support, questions, or feature requests:
- Create an issue on GitHub
- Check the documentation
- Review existing issues for solutions

---

**Vector Graphics Editor - Professional** - Creating beautiful vector graphics with modern tools and professional quality.