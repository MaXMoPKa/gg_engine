cmake_minimum_required(VERSION 3.31.5)

project(gg_studio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(generate_resources.cmake)

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
                               ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.png
                                 ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.ico
                                 ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.bmp
)

set(SOURCES ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES} ${GENERATED_RESOURCE_H} ${GENERATED_RC_FILE})

add_executable(${PROJECT_NAME} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${HEADER_FILES} ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCE_FILES} ${GENERATED_RESOURCE_H} ${GENERATED_RC_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
    COMMENT "Copying resources directory"
)

add_custom_command(
    OUTPUT ${GENERATED_RESOURCE_H} ${GENERATED_RC_FILE}
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/generate_resources.cmake
    DEPENDS ${RESOURCE_FILES}
    COMMENT "Regenerating resource files"
)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
        COMMENT "[gg_studio] Creating symlink for compile_commands.json")
endif()

set_source_files_properties(${RESOURCE_FILES} PROPERTIES
    HEADER_FILE_ONLY TRUE
    VS_DEPLOYMENT_CONTENT 1
    VS_DEPLOYMENT_LOCATION "Resources"
)