# Vector Graphics Editor

A simple vector graphics editor built with C++, Qt5, Cairo, and Libxml2. This application provides basic vector drawing capabilities similar to a simplified Inkscape.

## Features

- **Vector Shape Creation**: Rectangle, Ellipse, Line, and Bezier curves
- **Transformations**: Move, scale, and rotate shapes
- **Layering System**: Organize shapes in layers with visibility controls
- **SVG Import/Export**: Save and load vector graphics in SVG format
- **User Interface**: Modern Qt5-based interface with toolbars and dock widgets
- **Undo/Redo**: Basic undo/redo functionality for shape operations

## Requirements

- **C++17** compatible compiler
- **Qt5** (Core and Widgets modules)
- **Cairo** graphics library
- **Libxml2** for XML/SVG parsing
- **CMake** 3.16 or higher

## Building the Application

### Prerequisites

Make sure you have the following installed on your system:

1. **Qt5 Development Libraries**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtbase5-dev qt5-qmake
   
   # Windows (using Qt installer)
   # Download and install Qt5 from https://www.qt.io/download
   ```

2. **Cairo Graphics Library**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libcairo2-dev
   
   # Windows
   # Install via MSYS2: pacman -S mingw-w64-x86_64-cairo
   ```

3. **Libxml2**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libxml2-dev
   
   # Windows
   # Install via MSYS2: pacman -S mingw-w64-x86_64-libxml2
   ```

### Build Instructions

1. **Clone or download the project**:
   ```bash
   git clone <repository-url>
   cd VectorGraphicsEditor
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   make
   # or on Windows with Visual Studio:
   # cmake --build . --config Release
   ```

5. **Run the application**:
   ```bash
   ./VectorGraphicsEditor
   ```

## Usage

### Basic Drawing

1. **Select a Tool**: Use the toolbar to select drawing tools:
   - **Select Tool**: Click and drag to select shapes
   - **Rectangle Tool**: Click and drag to create rectangles
   - **Ellipse Tool**: Click and drag to create ellipses
   - **Line Tool**: Click and drag to create lines
   - **Bezier Tool**: Click to add points, press Enter to complete the curve

2. **Drawing Shapes**:
   - Click and drag to create rectangles and ellipses
   - Click and drag to create lines
   - For Bezier curves, click to add control points, then press Enter to complete

3. **Selecting and Modifying**:
   - Use the Select tool to click on shapes
   - Selected shapes show blue selection handles
   - Press Delete to remove selected shapes

### View Controls

- **Zoom**: Use Ctrl+Mouse Wheel or toolbar buttons
- **Pan**: Use Mouse Wheel to pan the canvas
- **Fit to View**: Click the "Fit to View" button to see the entire document

### File Operations

- **New Document**: Create a new empty document
- **Open**: Load an existing SVG file
- **Save**: Save the current document
- **Export SVG**: Export the document as an SVG file
- **Import SVG**: Import shapes from an SVG file

### Layers

- **Layers Panel**: Located on the left side of the window
- **Visibility**: Check/uncheck layers to show/hide them
- **Active Layer**: The current layer where new shapes will be added

### Properties

- **Properties Panel**: Located on the right side of the window
- **Pen Properties**: Set stroke color and width
- **Fill Properties**: Set fill color for shapes

## Project Structure

```
VectorGraphicsEditor/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Application entry point
├── mainwindow.h/cpp        # Main window UI
├── canvas.h/cpp            # Drawing canvas widget
├── document.h/cpp          # Document and layer management
├── shape.h                 # Base shape class
├── rectangle.h/cpp         # Rectangle shape implementation
├── ellipse.h/cpp           # Ellipse shape implementation
├── line.h/cpp              # Line shape implementation
├── bezier.h/cpp            # Bezier curve implementation
├── svg_parser.h/cpp        # SVG import/export functionality
└── README.md               # This file
```

## Architecture

The application follows a modular architecture:

- **Shape Classes**: Inherit from base `Shape` class, each implementing specific drawing logic
- **Document Model**: Manages layers and shapes, provides undo/redo functionality
- **Canvas Widget**: Handles user interaction and rendering
- **Main Window**: Provides the user interface and coordinates components
- **SVG Parser**: Handles import/export of SVG files

## Key Learning Concepts

This project demonstrates:

1. **Vector Graphics**: Mathematical representation of shapes and curves
2. **Computational Geometry**: Algorithms for shape intersection, transformation
3. **User Interface Design**: Modern GUI with toolbars, dock widgets, and property panels
4. **File Formats**: SVG parsing and generation
5. **Object-Oriented Design**: Inheritance, polymorphism, and design patterns
6. **Graphics Programming**: Cairo integration for high-quality rendering

## Limitations

This is a simplified implementation with the following limitations:

- Basic shape transformations (full transformation matrix not implemented)
- Limited Bezier curve editing capabilities
- Simple SVG parsing (not full SVG spec compliant)
- Basic undo/redo system
- No text or image support
- Limited styling options

## Future Enhancements

Potential improvements could include:

- Advanced shape editing with control points
- Text tool and text formatting
- Image import and manipulation
- Advanced transformations (shear, skew)
- Gradient fills and patterns
- More sophisticated SVG support
- Plugin system for custom tools
- Collaborative editing features

## Troubleshooting

### Build Issues

1. **Qt5 not found**: Ensure Qt5 is properly installed and CMake can find it
2. **Cairo not found**: Install Cairo development libraries
3. **Libxml2 not found**: Install Libxml2 development libraries
4. **Compiler errors**: Ensure you're using a C++17 compatible compiler

### Runtime Issues

1. **Application crashes**: Check that all required libraries are installed
2. **Drawing issues**: Verify Cairo is working correctly
3. **SVG import/export problems**: Check file permissions and SVG format

## License

This project is provided as educational material. Feel free to use, modify, and distribute according to your needs.

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests to improve the application. 