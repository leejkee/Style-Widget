qt_add_executable(Example
        MANUAL_FINALIZATION
        main.cpp
)

target_link_libraries(Example PRIVATE
        Qt6::Widgets
        FrameWidget
)
