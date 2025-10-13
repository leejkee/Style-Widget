add_library(TitleBarWidget STATIC
        titlebar.cpp
        titlebar.h
        titlebar_res.qrc
)

target_link_libraries(TitleBarWidget PRIVATE
        Qt6::Widgets
        Qt6::Core
)

target_include_directories(TitleBarWidget PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> 
)