add_executable(empty_window WIN32)

target_sources(empty_window
PRIVATE
    main.cpp
)

target_compile_definitions(empty_window
PRIVATE
    _UNICODE
    UNICODE
    GLEW_STATIC
)

target_link_libraries(empty_window
PRIVATE
    SDL2
    SDL2main
    opengl32
    gl3w
)

# target_include_directories(empty_window
# PRIVATE
#     ${CMAKE_SOURCE_DIR}/3rd/glew/include
# )