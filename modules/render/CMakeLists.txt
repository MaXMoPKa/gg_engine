cmake_minimum_required(VERSION 3.31.5)

project(render LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-windows-msvc)
set(CMAKE_C_COMPILER_TARGET x86_64-pc-windows-msvc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE INTERFACE_MODULES interface/*.cppm)
file(GLOB_RECURSE IMPLEMENTATION_MODULES implementation/*.cpp)

set(MODULE_SOURCES ${INTERFACE_MODULES} ${IMPLEMENTATION_MODULES})

add_library(${PROJECT_NAME} MODULE)

#target_sources(${PROJECT_NAME} PUBLIC FILE_SET interface_modules TYPE CXX_MODULES FILES ${MODULE_SOURCES})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET interface_modules TYPE CXX_MODULES FILES ${INTERFACE_MODULES})
target_sources(${PROJECT_NAME} PRIVATE ${IMPLEMENTATION_MODULES})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++23
                                                   /experimental:module)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23 -fmodules)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23 -fmodules-ts)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE d3d12 dxgi DirectXTex)

add_library(gg::render ALIAS render)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INTERFACE_MODULES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${IMPLEMENTATION_MODULES})

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
        COMMENT "[gg_studio] Creating symlink for compile_commands.json")
endif(CMAKE_EXPORT_COMPILE_COMMANDS)
