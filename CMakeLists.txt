cmake_minimum_required(VERSION 3.16)
project(VectorGraphicsEditor VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add modern CMake features
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Math definitions for all platforms
add_definitions(-D_USE_MATH_DEFINES)

# Find Qt6 - try different approaches for better portability
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if(NOT Qt6_FOUND)
    # Try Qt5 as fallback
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    message(STATUS "Using Qt5 instead of Qt6")
    set(QT_VERSION_MAJOR 5)
else()
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
endif()

# Find Cairo - with better error handling
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CAIRO cairo)
    if(CAIRO_FOUND)
        message(STATUS "Found Cairo via pkg-config")
    endif()
endif()

# Fallback to find Cairo manually if pkg-config failed
if(NOT CAIRO_FOUND)
    find_path(CAIRO_INCLUDE_DIR cairo.h
        PATHS
            ${CMAKE_SOURCE_DIR}/include/cairo
            /usr/include/cairo
            /usr/local/include/cairo
            /opt/local/include/cairo
    )
    
    find_library(CAIRO_LIBRARY
        NAMES cairo
        PATHS
            ${CMAKE_SOURCE_DIR}/lib/cairo
            /usr/lib
            /usr/local/lib
            /opt/local/lib
    )
    
    if(CAIRO_INCLUDE_DIR AND CAIRO_LIBRARY)
        set(CAIRO_FOUND TRUE)
        set(CAIRO_INCLUDE_DIRS ${CAIRO_INCLUDE_DIR})
        set(CAIRO_LIBRARIES ${CAIRO_LIBRARY})
        message(STATUS "Found Cairo manually: ${CAIRO_LIBRARY}")
    endif()
endif()

if(NOT CAIRO_FOUND)
    message(WARNING "Cairo not found - some rendering features may not work properly")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add Cairo include directories if found
if(CAIRO_FOUND)
    include_directories(${CAIRO_INCLUDE_DIRS})
endif()

# Add local include directories as fallback
if(EXISTS ${CMAKE_SOURCE_DIR}/include/cairo)
    include_directories(${CMAKE_SOURCE_DIR}/include/cairo)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/include/libxml2)
    include_directories(${CMAKE_SOURCE_DIR}/include/libxml2)
endif()

# Library directories (for static libraries) - only if they exist
if(EXISTS ${CMAKE_SOURCE_DIR}/lib/cairo)
    link_directories(${CMAKE_SOURCE_DIR}/lib/cairo)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/lib/libxml2)
    link_directories(${CMAKE_SOURCE_DIR}/lib/libxml2)
endif()

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

# Link libraries based on Qt version
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(VectorGraphicsEditor
        Qt6::Core
        Qt6::Widgets
    )
else()
    target_link_libraries(VectorGraphicsEditor
        Qt5::Core
        Qt5::Widgets
    )
endif()

# Link Cairo if found
if(CAIRO_FOUND)
    if(CAIRO_LIBRARIES)
        target_link_libraries(VectorGraphicsEditor ${CAIRO_LIBRARIES})
    else()
        target_link_libraries(VectorGraphicsEditor cairo)
    endif()
else()
    # Try to link cairo anyway in case it's available at link time
    target_link_libraries(VectorGraphicsEditor cairo)
endif()

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

# Enable testing
enable_testing()

# Find Google Test (optional)
find_package(GTest QUIET)
if(GTest_FOUND OR GTEST_FOUND)
    message(STATUS "Google Test found - building unit tests")
    
    # Test sources
    set(TEST_SOURCES
        tests/test_shapes.cpp
        tests/test_document.cpp
    )
    
    # Shape sources needed for tests
    set(SHAPE_SOURCES
        src/shape.cpp
        src/rectangle.cpp
        src/ellipse.cpp
        src/line.cpp
        src/bezier.cpp
        src/document.cpp
    )
    
    # Create test executable
    add_executable(VectorGraphicsEditorTests
        ${TEST_SOURCES}
        ${SHAPE_SOURCES}
    )
    
    # Link test libraries
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(VectorGraphicsEditorTests
            Qt6::Core
            Qt6::Widgets
            GTest::gtest
            GTest::gtest_main
        )
    else()
        target_link_libraries(VectorGraphicsEditorTests
            Qt5::Core
            Qt5::Widgets
            GTest::gtest
            GTest::gtest_main
        )
    endif()
    
    # Link Cairo for tests
    if(CAIRO_FOUND)
        if(CAIRO_LIBRARIES)
            target_link_libraries(VectorGraphicsEditorTests ${CAIRO_LIBRARIES})
        else()
            target_link_libraries(VectorGraphicsEditorTests cairo)
        endif()
    else()
        target_link_libraries(VectorGraphicsEditorTests cairo)
    endif()
    
    # Set up test properties
    set_target_properties(VectorGraphicsEditorTests PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )
    
    # Add tests to CTest
    add_test(NAME ShapeTests COMMAND VectorGraphicsEditorTests)
    
else()
    message(STATUS "Google Test not found - skipping unit tests")
endif() 