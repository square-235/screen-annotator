cmake_minimum_required(VERSION 3.16)
project(screen-annotator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 (fallback to Qt5 if not available)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui)

# Source files
set(CONFIG_SOURCES
    config/config.h
    config/config.cpp
)

set(MODELS_SOURCES
    models/annotation.h
    models/annotation.cpp
)

set(TOOLS_SOURCES
    tools/base_tool.h
    tools/base_tool.cpp
    tools/pen_tool.h
    tools/pen_tool.cpp
    tools/line_tool.h
    tools/line_tool.cpp
    tools/rectangle_tool.h
    tools/rectangle_tool.cpp
    tools/ellipse_tool.h
    tools/ellipse_tool.cpp
    tools/arrow_tool.h
    tools/arrow_tool.cpp
    tools/eraser_tool.h
    tools/eraser_tool.cpp
    tools/tool_factory.h
    tools/tool_factory.cpp
)

set(CORE_SOURCES
    core/history_manager.h
    core/history_manager.cpp
    core/floating_toolbar.h
    core/floating_toolbar.cpp
    core/annotation_overlay.h
    core/annotation_overlay.cpp
)

set(MAIN_SOURCES
    main.cpp
    screen_annotator.h
    screen_annotator.cpp
)

# Create executable
add_executable(${PROJECT_NAME}
    ${CONFIG_SOURCES}
    ${MODELS_SOURCES}
    ${TOOLS_SOURCES}
    ${CORE_SOURCES}
    ${MAIN_SOURCES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Gui
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
