cmake_minimum_required(VERSION 3.31.5)

project(gg_studio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)

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

file(GLOB_RECURSE SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.hlsl)

set_source_files_properties( ${SHADER_FILES}
    PROPERTIES
        VS_SHADER_MODEL 6.4
        VS_SHADER_ENTRYPOINT "main"
        VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:ON>
        VS_SHADER_ENABLE_DEBUG $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ON>
        VS_SHADER_FLAGS $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-Qembed_debug>
        VS_SHADER_OBJECT_FILE_NAME "$(OutDir)/resources/shaders/%(Filename).cso"
        VS_TOOL "HLSL Compiler"
)

set_source_files_properties( ${SHADER_FILES}
PROPERTIES
    VS_SHADER_TYPE Vertex
    #HEADER_FILE_ONLY FALSE
    #VS_TOOL_OVERRIDE "HLSL Compiler"
)

set(SOURCES ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES} ${GENERATED_RESOURCE_H} ${GENERATED_RC_FILE} ${SHADER_FILES})

add_executable(${PROJECT_NAME} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${HEADER_FILES} ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCE_FILES} ${GENERATED_RESOURCE_H} ${GENERATED_RC_FILE} ${SHADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11
        dxgi
        dxguid
        d3dcompiler
        dxerr
    )
endif()

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
