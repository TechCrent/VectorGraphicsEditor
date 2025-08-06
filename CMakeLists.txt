cmake_minimum_required(VERSION 3.16)
project(VectorGraphicsEditor VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    if(MSVC)
        add_compile_options(/Zc:__cplusplus /permissive-)
        add_definitions(-D_USE_MATH_DEFINES)
    endif()
endif()

add_definitions(-D_USE_MATH_DEFINES)

find_package(Qt6 COMPONENTS Core Widgets Test QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets Test REQUIRED)
    message(STATUS "Using Qt5 instead of Qt6")
    set(QT_VERSION_MAJOR 5)
else()
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
endif()

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CAIRO cairo)
    if(CAIRO_FOUND)
        message(STATUS "Found Cairo via pkg-config")
    endif()
endif()
if(NOT CAIRO_FOUND)
    find_path(CAIRO_INCLUDE_DIR cairo.h
            PATHS
            ${CMAKE_SOURCE_DIR}/include/cairo
            /usr/include/cairo
            /usr/local/include/cairo
            /opt/local/include/cairo)
    find_library(CAIRO_LIBRARY
            NAMES cairo
            PATHS
            ${CMAKE_SOURCE_DIR}/lib/cairo
            /usr/lib
            /usr/local/lib
            /opt/local/lib)
    if(CAIRO_INCLUDE_DIR AND CAIRO_LIBRARY)
        set(CAIRO_FOUND TRUE)
        set(CAIRO_INCLUDE_DIRS ${CAIRO_INCLUDE_DIR})
        set(CAIRO_LIBRARIES ${CAIRO_LIBRARY})
        message(STATUS "Found Cairo manually: ${CAIRO_LIBRARY}")
    endif()
endif()
if(NOT CAIRO_FOUND)
    message(WARNING "Cairo not found - some rendering features may not work")
endif()

if(PkgConfig_FOUND)
    pkg_check_modules(LIBXML2 libxml-2.0)
    if(LIBXML2_FOUND)
        message(STATUS "Found LibXml2 via pkg-config")
    endif()
endif()
if(NOT LIBXML2_FOUND)
    find_path(LIBXML2_INCLUDE_DIR libxml/parser.h
            PATHS
            ${CMAKE_SOURCE_DIR}/include/libxml2
            /usr/include/libxml2
            /usr/local/include/libxml2
            /opt/local/include/libxml2)
    find_library(LIBXML2_LIBRARY
            NAMES xml2 libxml2
            PATHS
            ${CMAKE_SOURCE_DIR}/lib/libxml2
            /usr/lib
            /usr/local/lib
            /opt/local/lib)
    if(LIBXML2_INCLUDE_DIR AND LIBXML2_LIBRARY)
        set(LIBXML2_FOUND TRUE)
        set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR})
        set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY})
        message(STATUS "Found LibXml2 manually: ${LIBXML2_LIBRARY}")
    endif()
endif()
if(NOT LIBXML2_FOUND)
    message(WARNING "LibXml2 not found - SVG parsing may not work")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
if(CAIRO_FOUND)
    include_directories(${CAIRO_INCLUDE_DIRS})
endif()
if(LIBXML2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIRS})
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/include/cairo)
    include_directories(${CMAKE_SOURCE_DIR}/include/cairo)
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/include/libxml2)
    include_directories(${CMAKE_SOURCE_DIR}/include/libxml2)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/lib/cairo)
    link_directories(${CMAKE_SOURCE_DIR}/lib/cairo)
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/lib/libxml2)
    link_directories(${CMAKE_SOURCE_DIR}/lib/libxml2)
endif()

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

set(UI_FILES
        ui/mainwindow.ui
)

add_executable(VectorGraphicsEditor
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
)

set_target_properties(VectorGraphicsEditor PROPERTIES
        AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui"
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(VectorGraphicsEditor PRIVATE Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(VectorGraphicsEditor PRIVATE Qt5::Core Qt5::Widgets)
endif()
if(CAIRO_FOUND)
    target_link_libraries(VectorGraphicsEditor PRIVATE ${CAIRO_LIBRARIES})
else()
    target_link_libraries(VectorGraphicsEditor PRIVATE cairo)
endif()
if(LIBXML2_FOUND)
    target_link_libraries(VectorGraphicsEditor PRIVATE ${LIBXML2_LIBRARIES})
else()
    target_link_libraries(VectorGraphicsEditor PRIVATE xml2)
endif()

if(WIN32)
    set_target_properties(VectorGraphicsEditor PROPERTIES
            OUTPUT_NAME "VECTOR GRAPHICS EDITOR"
            WIN32_EXECUTABLE TRUE
    )
endif()

install(TARGETS VectorGraphicsEditor
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    file(GLOB RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resources/*")
    if(RESOURCE_FILES)
        file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
    else()
        message(STATUS "Resources directory is empty, skipping copy")
    endif()
endif()

if(WIN32)
    add_custom_command(TARGET VectorGraphicsEditor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin
            $<TARGET_FILE_DIR:VectorGraphicsEditor>
    )
endif()

enable_testing()

find_package(GTest QUIET)
if(GTest_FOUND OR GTEST_FOUND)
    message(STATUS "Google Test found - building unit tests")
    set(TEST_SOURCES
            tests/test_main.cpp
            tests/test_shapes.cpp
            tests/test_document.cpp
    )
    set(SHAPE_SOURCES
            src/shape.cpp
            src/rectangle.cpp
            src/ellipse.cpp
            src/line.cpp
            src/bezier.cpp
            src/document.cpp
            src/svg_parser.cpp
    )
    set(TEST_HEADERS
            include/document.h
            include/shape.h
            include/rectangle.h
            include/ellipse.h
            include/line.h
            include/bezier.h
    )
    add_executable(VectorGraphicsEditorTests
            ${TEST_SOURCES}
            ${SHAPE_SOURCES}
            ${TEST_HEADERS}
    )
    target_include_directories(VectorGraphicsEditorTests PRIVATE
            ${CMAKE_SOURCE_DIR}/include
    )
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(VectorGraphicsEditorTests PRIVATE
                Qt6::Core Qt6::Widgets Qt6::Test GTest::gtest GTest::gtest_main)
    else()
        target_link_libraries(VectorGraphicsEditorTests PRIVATE
                Qt5::Core Qt5::Widgets Qt5::Test GTest::gtest GTest::gtest_main)
    endif()
    if(CAIRO_FOUND)
        target_link_libraries(VectorGraphicsEditorTests PRIVATE ${CAIRO_LIBRARIES})
    else()
        target_link_libraries(VectorGraphicsEditorTests PRIVATE cairo)
    endif()
    if(LIBXML2_FOUND)
        target_link_libraries(VectorGraphicsEditorTests PRIVATE ${LIBXML2_LIBRARIES})
    else()
        target_link_libraries(VectorGraphicsEditorTests PRIVATE xml2)
    endif()
    set_target_properties(VectorGraphicsEditorTests PROPERTIES
            AUTOMOC ON
            AUTOUIC ON
            AUTORCC ON
    )
    if(WIN32)
        add_custom_command(TARGET VectorGraphicsEditorTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "C:/Qt/6.9.1/mingw_64/bin/Qt6Core.dll"
                "C:/Qt/6.9.1/mingw_64/bin/Qt6Gui.dll"
                "C:/Qt/6.9.1/mingw_64/bin/Qt6Widgets.dll"
                "C:/Qt/6.9.1/mingw_64/bin/Qt6Test.dll"
                "C:/msys64/mingw64/bin/libgcc_s_seh-1.dll"
                "C:/msys64/mingw64/bin/libstdc++-6.dll"
                "C:/msys64/mingw64/bin/libwinpthread-1.dll"
                "C:/vcpkg/installed/x64-mingw-dynamic/bin/libcairo-2.dll"
                "C:/vcpkg/installed/x64-mingw-dynamic/bin/libxml2.dll"
                $<TARGET_FILE_DIR:VectorGraphicsEditorTests>
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "C:/Qt/6.9.1/mingw_64/plugins/platforms"
                $<TARGET_FILE_DIR:VectorGraphicsEditorTests>/platforms
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "C:/Qt/6.9.1/mingw_64/plugins/imageformats"
                $<TARGET_FILE_DIR:VectorGraphicsEditorTests>/imageformats
        )
    endif()
    add_test(NAME VectorGraphicsEditorTests COMMAND VectorGraphicsEditorTests)
    include(GoogleTest)
    gtest_discover_tests(VectorGraphicsEditorTests
            WORKING_DIRECTORY $<TARGET_FILE_DIR:VectorGraphicsEditorTests>
    )
else()
    message(STATUS "Google Test not found - skipping unit tests")
endif()
