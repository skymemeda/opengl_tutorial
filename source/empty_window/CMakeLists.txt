add_executable(empty_window WIN32)

target_sources(empty_window
PRIVATE
    main.cpp
)

target_compile_definitions(empty_window
PRIVATE
    _UNICODE
    UNICODE
)

target_link_libraries(empty_window
    SDL2
    SDL2main
    gl3w
)