cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
project(FrameWidgetProject
        VERSION 0.1
        LANGUAGES CXX
)
find_package(Qt6 COMPONENTS
        Core
        Widgets
)

add_subdirectory(TitleBar)
add_library(FrameWidget STATIC
        framewgt.cpp
        framewgt.h
)

target_link_libraries(FrameWidget
        PUBLIC
        Qt6::Widgets
        Qt6::Core
        TitleBarWidget
)

target_include_directories(FrameWidget PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(examples)