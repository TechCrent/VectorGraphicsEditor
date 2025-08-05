cmake_minimum_required(VERSION 3.16)
project(VectorGraphicsEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific settings
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    
    # Add MSVC-specific compiler flags for C++17 support
    if(MSVC)
        add_compile_options(/Zc:__cplusplus /permissive-)
        add_definitions(-D_USE_MATH_DEFINES)
    endif()
endif()

# Find Qt6
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Include directories for local headers
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/cairo)
include_directories(${CMAKE_SOURCE_DIR}/include/libxml2)

# Library directories (for static libraries)
link_directories(${CMAKE_SOURCE_DIR}/lib/cairo)
link_directories(${CMAKE_SOURCE_DIR}/lib/libxml2)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/canvas.cpp
    src/document.cpp
    src/svg_parser.cpp
    src/shape.cpp
    src/rectangle.cpp
    src/ellipse.cpp
    src/line.cpp
    src/bezier.cpp
)

# Header files
set(HEADERS
    include/mainwindow.h
    include/canvas.h
    include/document.h
    include/svg_parser.h
    include/shape.h
    include/rectangle.h
    include/ellipse.h
    include/line.h
    include/bezier.h
)

# UI files
set(UI_FILES
    ui/mainwindow.ui
)

# Add executable
add_executable(VectorGraphicsEditor
    ${SOURCES}
    ${HEADERS}
)

# Set UI file path for AUTOUIC
set_target_properties(VectorGraphicsEditor PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui"
)

# Enable automoc for Qt
set_target_properties(VectorGraphicsEditor PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Link libraries
target_link_libraries(VectorGraphicsEditor
    Qt6::Core
    Qt6::Widgets
    cairo
    cairo-script-interpreter
)

# Windows-specific executable properties
if(WIN32)
    set_target_properties(VectorGraphicsEditor PROPERTIES
        OUTPUT_NAME "VECTOR GRAPHICS EDITOR"
        WIN32_EXECUTABLE TRUE
    )
endif()

# Install target
install(TARGETS VectorGraphicsEditor
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# Copy DLL files to build directory on Windows
if(WIN32)
    add_custom_command(TARGET VectorGraphicsEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/bin
        $<TARGET_FILE_DIR:VectorGraphicsEditor>
    )
endif() 