cmake_minimum_required(VERSION 3.18.0)

project(opengl_tutorial)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmakeconf )

include(compiler_conf)
include(building_output)

add_subdirectory(3rd/sdl2)
add_subdirectory(3rd/gl3w)

add_subdirectory(source)