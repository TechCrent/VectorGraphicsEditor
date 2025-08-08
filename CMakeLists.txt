cmake_minimum_required(VERSION 3.16)
project(VectorGraphicsEditor VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------
# Options
# --------------------
option(ENABLE_TESTS "Enable unit tests with GoogleTest" ON)
option(ENABLE_CAIRO "Enable Cairo rendering" ON)
option(ENABLE_LIBXML2 "Enable LibXml2 for SVG parsing" ON)
option(ENABLE_QT_DEPLOY "Automatically run windeployqt after build" ON)

# --------------------
# Global Qt Auto Setup
# --------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --------------------
# Windows-specific settings
# --------------------
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    if(MSVC)
        add_compile_options(/Zc:__cplusplus /permissive-)
        add_definitions(-D_USE_MATH_DEFINES)
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()

add_definitions(-D_USE_MATH_DEFINES)

# --------------------
# Qt detection (Debug/Release aware)
# --------------------
find_package(Qt6 COMPONENTS Core Gui Widgets Svg Test QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Widgets Svg Test REQUIRED)
    message(STATUS "Using Qt5 instead of Qt6")
    set(QT_VERSION_MAJOR 5)
else()
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-DQT_DEBUG)
        message(STATUS "Qt Debug mode enabled: Expecting *d.dll files.")
    else()
        add_definitions(-DQT_NO_DEBUG)
        message(STATUS "Qt Release mode enabled: Expecting normal .dll files.")
    endif()
endif()

# --------------------
# Cairo detection
# --------------------
if(ENABLE_CAIRO)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(CAIRO cairo)
    endif()

    if(NOT CAIRO_FOUND)
        find_path(CAIRO_INCLUDE_DIR cairo.h PATHS C:/msys64/mingw64/include ${CMAKE_SOURCE_DIR}/include)
        find_library(CAIRO_LIBRARY NAMES cairo PATHS C:/msys64/mingw64/lib)
        if(CAIRO_INCLUDE_DIR AND CAIRO_LIBRARY)
            set(CAIRO_FOUND TRUE)
            set(CAIRO_INCLUDE_DIRS ${CAIRO_INCLUDE_DIR})
            set(CAIRO_LIBRARIES ${CAIRO_LIBRARY})
        endif()
    endif()

    if(CAIRO_FOUND)
        message(STATUS "Cairo found: ${CAIRO_INCLUDE_DIRS}")
    else()
        message(WARNING "Cairo not found - rendering will be limited")
        set(ENABLE_CAIRO OFF)
    endif()
endif()

# --------------------
# LibXml2 detection
# --------------------
if(ENABLE_LIBXML2)
    if(PkgConfig_FOUND)
        pkg_check_modules(LIBXML2 libxml-2.0)
    endif()

    if(NOT LIBXML2_FOUND)
        find_path(LIBXML2_INCLUDE_DIR libxml/parser.h PATHS C:/msys64/mingw64/include/libxml2)
        find_library(LIBXML2_LIBRARY NAMES xml2 libxml2 PATHS C:/msys64/mingw64/lib)
        if(LIBXML2_INCLUDE_DIR AND LIBXML2_LIBRARY)
            set(LIBXML2_FOUND TRUE)
            set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR})
            set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY})
        endif()
    endif()

    if(LIBXML2_FOUND)
        message(STATUS "LibXml2 found: ${LIBXML2_INCLUDE_DIRS}")
    else()
        message(WARNING "LibXml2 not found - SVG parsing will be disabled")
        set(ENABLE_LIBXML2 OFF)
    endif()
endif()

# --------------------
# Sources
# --------------------
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
        src/text.cpp
        src/colorpicker.cpp
        src/colorwheel.cpp
        src/colorstrip.cpp
)

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
        include/text.h
        include/colorpicker.h
        include/colorwheel.h
        include/colorstrip.h
)

set(UI_FILES ui/mainwindow.ui)

# --------------------
# Main executable
# --------------------
add_executable(VectorGraphicsEditor
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
)
# Tell Qt UIC where to search for .ui files
set_target_properties(VectorGraphicsEditor PROPERTIES
        AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui"
)

target_include_directories(VectorGraphicsEditor PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optional dependencies
if(ENABLE_CAIRO)
    target_include_directories(VectorGraphicsEditor PRIVATE ${CAIRO_INCLUDE_DIRS})
    target_link_libraries(VectorGraphicsEditor PRIVATE ${CAIRO_LIBRARIES})
endif()

if(ENABLE_LIBXML2)
    target_include_directories(VectorGraphicsEditor PRIVATE ${LIBXML2_INCLUDE_DIRS})
    target_link_libraries(VectorGraphicsEditor PRIVATE ${LIBXML2_LIBRARIES})
endif()

# Link Qt
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(VectorGraphicsEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg)
else()
    target_link_libraries(VectorGraphicsEditor PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
endif()

# --------------------
# Resource copying
# --------------------
file(GLOB RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resources/*")
if(RESOURCE_FILES)
    add_custom_target(copy_resources ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            ${CMAKE_BINARY_DIR}/resources
            DEPENDS ${RESOURCE_FILES}
    )
endif()

# --------------------
# Qt Auto-deployment
# --------------------
if(WIN32 AND QT_VERSION_MAJOR EQUAL 6 AND ENABLE_QT_DEPLOY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET VectorGraphicsEditor POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Deploying Qt runtime..."
                COMMAND ${WINDEPLOYQT_EXECUTABLE} --no-opengl-sw $<TARGET_FILE:VectorGraphicsEditor>
        )
    else()
        message(WARNING "windeployqt not found. You must deploy Qt manually.")
    endif()
endif()

# --------------------
# Install
# --------------------
install(TARGETS VectorGraphicsEditor
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

# --------------------
# GoogleTest (Optional)
# --------------------
if(ENABLE_TESTS)
    find_package(GTest QUIET)
    if(GTest_FOUND)
        message(STATUS "Google Test found - building tests")
        set(TEST_SOURCES tests/test_main.cpp tests/test_shapes.cpp tests/test_document.cpp)

        add_executable(VectorGraphicsEditorTests
                ${TEST_SOURCES}
                src/shape.cpp
                src/rectangle.cpp
                src/ellipse.cpp
                src/line.cpp
                src/bezier.cpp
                src/document.cpp
                src/svg_parser.cpp
        )

        target_include_directories(VectorGraphicsEditorTests PRIVATE ${CMAKE_SOURCE_DIR}/include)

        if(QT_VERSION_MAJOR EQUAL 6)
            target_link_libraries(VectorGraphicsEditorTests PRIVATE
                    Qt6::Core Qt6::Widgets Qt6::Test Qt6::Svg GTest::gtest GTest::gtest_main)
        else()
            target_link_libraries(VectorGraphicsEditorTests PRIVATE
                    Qt5::Core Qt5::Widgets Qt5::Test Qt5::Svg GTest::gtest GTest::gtest_main)
        endif()

        if(ENABLE_CAIRO)
            target_link_libraries(VectorGraphicsEditorTests PRIVATE ${CAIRO_LIBRARIES})
        endif()

        if(ENABLE_LIBXML2)
            target_link_libraries(VectorGraphicsEditorTests PRIVATE ${LIBXML2_LIBRARIES})
        endif()

        enable_testing()
        include(GoogleTest)
        gtest_discover_tests(VectorGraphicsEditorTests)
    else()
        message(STATUS "Google Test not found - skipping tests")
    endif()
endif()
