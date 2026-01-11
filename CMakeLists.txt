cmake_minimum_required(VERSION 3.20)
project(PDFEditor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Optimization flags for maximum performance
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL /arch:AVX2")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
endif()

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/PDFViewer.cpp
    src/PDFDocument.cpp
    src/CustomTitleBar.cpp
    src/AnimationEngine.cpp
    src/EditToolbar.cpp
    src/PDFEditor.cpp
    src/TextTool.cpp
    src/HighlightTool.cpp
    src/DrawingTool.cpp
    src/AnnotationTool.cpp
    src/ImageTool.cpp
    src/PDFTextExtractor.cpp
    src/TextEditDialog.cpp
)

set(HEADERS
    include/MainWindow.h
    include/PDFViewer.h
    include/PDFDocument.h
    include/CustomTitleBar.h
    include/AnimationEngine.h
    include/EditToolbar.h
    include/PDFEditor.h
    include/TextTool.h
    include/HighlightTool.h
    include/DrawingTool.h
    include/AnnotationTool.h
    include/ImageTool.h
    include/PDFTextExtractor.h
    include/TextEditDialog.h
    include/DrawingTool.h
    include/AnnotationTool.h
    include/ImageTool.h
)

# Create executable
add_executable(PDFEditor WIN32 ${SOURCES} ${HEADERS} resources.qrc)

# Link libraries
target_link_libraries(PDFEditor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# Set properties
set_target_properties(PDFEditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Copy resources
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
